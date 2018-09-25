#ifndef __extraPins_h__
#define __extraPins_h__

#include <Arduino.h>
#include <inttypes.h>

#define MAX_EXTRA_PINS 16

extern void extraDigitalWrite(uint8_t pin, uint8_t value);
extern uint8_t extraDigitalRead(uint8_t pin);
extern void extraPinMode(uint8_t pin, uint8_t mode);
extern void registerPin(uint8_t pin, void *object);

class ExtraPins {
public:
    void digitalWrite(uint8_t pin, uint8_t value);
    uint8_t digitalRead(uint8_t pin);
    void pinMode(uint8_t pin, uint8_t mode);
};


#endif

// vim:ts=4:sw=4:ai:et:si:sts=4
