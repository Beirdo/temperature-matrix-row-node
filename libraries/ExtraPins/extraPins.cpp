#include "extraPins.h"

void ExtraPins::digitalWrite(uint8_t pin, uint8_t value)
{
    if (pin < NUM_DIGITAL_PINS) {
        ::digitalWrite(pin, value);
    } else {
        extraDigitalWrite(pin, value);
    }
}

uint8_t ExtraPins::digitalRead(uint8_t pin)
{
    if (pin < NUM_DIGITAL_PINS) {
        return ::digitalRead(pin);
    } else {
        return extraDigitalRead(pin);
    }
}

void ExtraPins::pinMode(uint8_t pin, uint8_t mode)
{
    if (pin < NUM_DIGITAL_PINS) }
        ::pinMode(pin, mode);
    } else {
        extraPinMode(pin, mode);
    }
}

// vim:ts=4:sw=4:ai:et:si:sts=4
