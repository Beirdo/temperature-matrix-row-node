#include "Beirdo-CANBUS.h"
#include <extraPins.h>
#include <vectoredInterrupt.h>


Beirdo_CANBUS::Beirdo_CANBUS(MCP2515 *controller, uint8_t canID,
                             uint8_t stbyPin, uint8_t intPin) : ExtraPins()
{
    p_controller = controller;
    p_canID = canID;
    p_stbyPin = stbyPin;
    p_intPin = intPin;
    p_writeIndex = 0;
    p_readIndex = 0;
    p_interrupt = false;

    digitalWrite(p_stdbyPin, LOW);  // Always in Normal mode
    pinMode(p_stbyPin, OUTPUT);

    if (p_intPin == 0 || p_intPin == 1) { // External Interrupt lines
        attachInterrupt(p_intPin, canBusInterruptHandler, FALLING);
        pinMode(p_intPin + 2, INPUT_PULLUP);
    } else {
        attachVectoredInterrupt(p_intPin, canBusInterruptHandler, FALLING);
        pinMode(p_intPin, INPUT_PULLUP);
    }

    // Enable the interrupts in the buffer control registers
    p_controller->modifyRegister(MCP2515::MCP_TXB0CTRL, MCP2515::TXB_TXIE,
                                 MCP2515::TXB_TXIE);
    p_controller->modifyRegister(MCP2515::MCP_TXB1CTRL, MCP2515::TXB_TXIE,
                                 MCP2515::TXB_TXIE);
    p_controller->modifyRegister(MCP2515::MCP_TXB2CTRL, MCP2515::TXB_TXIE,
                                 MCP2515::TXB_TXIE);

    // Clear the transmit interrupts just in case
    p_controller->clearTXInterrupts();
    
    // Enable transmit interrupts
    uint8_t mask = MCP2515::CANINTF_TX0IF | MCP2515::CANINTF_TX1IF |
                   MCP2515::CANINTF_TX2IF;
    p_controller->modifyRegister(MCP_CANINTE, mask, mask);
}

void Beirdo_CANBUS::canbusInterruptHandler(void)
{
    p_interrupt = true;
}

void Beirdo_CANBUS::canbusInterruptSlowHandler(void)
{
    if (!p_interrupt || !p_controller) {
        return;
    }

    p_interrupt = false;

    uint8_t irq = p_controller->getInterrupts();

    // deal with the various IRQ bits...
    if ((irq & MCP2515::CANINTF_TX0IF) || (irq & MCP2515::CANINTF_TX1IF) ||
        (irq & MCP2515::CANINTF_TX2IF)) {
        sendAllReadings();
    }

    p_controller->clearTXInterrupts();
    p_controller->clearInterrupts();
}

void Beirdo_CANBUS::clearReadings(void)
{
    p_writeIndex = 0;
    p_readIndex = 0;
}

void Beirdo_CANBUS::queueReading(uint8_t *buffer, uint8_t buflen)
{
    if (buflen > 8) {
        return;
    }

    if (readingCount() >= MAX_READINGS - 1) {
        return;
    }

    uint8_t index = p_writeIndex;
    p_readings[index].buffer = buffer;
    p_readings[index].buflen = buflen;
    p_writeIndex = (p_writeIndex + 1) % MAX_READINGS;
}

uint8_t Beirdo_CANBUS::readingCount(void)
{
    return((p_writeIndex + MAX_READINGS - p_readIndex) % MAX_READINGS);
}

void Beirdo_CANBUS::sendAllReadings(void)
{
    uint8_t index;
    MCP2515::ERROR status = CAN2515::ERROR_OK;

    while(readingCount() && status == CAN2515::ERROR_OK) {
        index = p_readIndex;
        p_frame.can_dlc = p_readings[index].buflen;
        p_frame.can_id = p_canID;
        memcpy(p_frame.data, p_readings[index].buffer, p_frame.can_dlc);
        status = p_controller->sendMessage(&p_frame);
        if (status == CAN2515::ERROR_OK) {
            p_readIndex = (p_readIndex + 1) % MAX_READINGS;
        }
    }
}


// vim:ts=4:sw=4:ai:et:si:sts=4
