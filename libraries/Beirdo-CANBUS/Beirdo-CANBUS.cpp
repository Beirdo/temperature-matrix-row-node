#include "Beirdo-CANBUS.h"


Beirdo_CANBUS::Beirdo_CANBUS(MCP2515 *controller, uint8_t canID)
{
    p_controller = controller;
    p_canID = canID;
    p_writeIndex = 0;
    p_readIndex = 0;
}

void Beirdo_CANBUS::clearReadings(void)
{
    p_writeIndex = 0;
    p_readIndex = 0;
}

void Beirdo_CANBUS::addReading(uint8_t index, uint8_8t reading)
{
    if (readingCount() >= MAX_READINGS - 1) {
        sendAllReadings();
    }

    uint8_t _index = p_writeIndex;

    p_writeIndex = (p_writeIndex + 1) % MAX_READINGS;

    p_readings[_index].index = index;
    p_readings[_index].reading = reading;
}

uint8_t Beirdo_CANBUS::readingCount(void)
{
    return((p_writeIndex + MAX_READINGS - p_readIndex) % MAX_READINGS);
}

void Beirdo_CANBUS::sendAllReadings(void)
{
    uint8_t index;
    uint8_t count = 0;
    uint8_t offset;
    uint8_t i;

    while(readingCount()) {
        offset = count * 3;
        index = p_readIndex;
        p_readIndex = (p_readIndex + 1) % MAX_READINGS;
        p_frame.data[offset] = p_readings[index].index;
        p_frame.data[offset + 1] = uint8_t((p_readings[index].reading & 0xFF00) >> 8);
        p_frame.data[offset + 2] = uint8_t(p_readings[index].reading & 0x00FF);
        count++;
        offset = count * 3;

        if (offset >= 5 || readingCount() == 0) { // next reading would overfill the buffer (8 - 3 = 5)
            p_frame.can_dlc = offset;
            p_frame.can_id = p_canID;
            p_controller->sendMessage(&p_frame);
            count = 0;
        }
    }
}


// vim:ts=4:sw=4:ai:et:si:sts=4
