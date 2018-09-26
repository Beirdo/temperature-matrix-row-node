#include <Arduino.h>
#include "vectoredInterrupt.h"

#define LIBCALL_ENABLEINTERRUPT
#define EI_ARDUINO_INTERRUPTED_PIN
#include <EnableInterrupt.h>


void attachVectoredInterrupt(uint8_t pin, void (*handler)(void), uint8_t mode)
{
    if (!vectorManager) {
        return;
    }

    vectorManager->registerHandler(pin, handler, mode)
}

void detachVectoredInterrupt(uint8_t pin)
{
    if (!vectorManager) {
        return;
    }

    vectorManager->unregisterHandler(pin)
}

void vectoredHandler(void)
{
    if (!vectorHandler) {
        return;
    }

    vectorHandler->interruptHandler(arduinoInterruptedPin);
}


VectoredInterruptHandler::VectoredInterruptHandler(uint8_t count)
{
    p_count = count;
    p_handlerMap = new handler_map_t[count];
    memset((uint8_t *)p_handlerMap, 0xFF, sizeof(handler_map_t) * count);
}

uint8_t VectoredInterruptHandler::getPinIndex(uint8_t pin)
{
    uint8_t i;
    uint8_t index = 0xFF;
    uint8_t emptyIndex = 0xFF;

    for (i = 0; i < p_count; i++) {
        if (p_handlerMap[i].pin == 0xFF && emptyIndex == 0xFF) {
            emptyIndex = i;
        } else if (p_handlerMap[i].pin == pin) {
            index = i;
            break;
        }
    }

    if (index == 0xFF) {
        if (emptyIndex == 0xFF) {
            return 0xFF;
        }
        index = emptyIndex;
    }

    return index;
}

void VectoredInterruptHandler::registerHandler(uint8_t pin, void (*handler)(void), uint8_t mode)
{
    uint8_t index = getPinIndex(pin);
    
    if (index == 0xFF) {
        return;
    }

    p_handlerMap[index].pin = pin;
    p_handlerMap[index].handler = handler;
    p_handlerMap[index].mode = mode;

    pinMode(pin, INPUT_PULLUP);
    attachPinChangeInterrupt(pin, vectoredHandler, mode);
}

void VectoredInterruptHandler::unregisterHandler(uint8_t pin)
{
    uint8_t index = getPinIndex(pin);
    
    if (index == 0xFF) {
        return;
    }

    p_handlerMap[index].pin = 0xFF;

    detachPinChangeInterrupt(pin);
}

void VectoredInterruptHandler::interruptHandler(uint8_t pin)
{
    uint8_t index = getPinIndex(pin);
    
    if (index == 0xFF) {
        return;
    }

    void (*handler)(void) = p_handlerMap[index].handler;
    if (!handler) {
        return;
    }
    handler();
}



// vim:ts=4:sw=4:ai:et:si:sts=4
