#ifndef __extraPins_h__
#define __extraPins_h__

#include <Arduino.h>
#include <inttypes.h>

#define MAX_EXTRA_PINS 16
#define MAX_PIN_SOURCES 8

class ExtraPinSource {
public:
    ExtraPinSource(uint8_t count);
    virtual void digitalWrite(uint8_t pin, uint8_t value) = 0;
    virtual uint8_t digitalRead(uint8_t pin) = 0;
    virtual void pinMode(uint8_t pin, uint8_t mode) = 0;
    void registerPin(uint8_t pin, uint8_t localPin);
protected:
    uint8_t p_count;
    uint8_t *p_pinReverse;

    uint8_t getLocalPin(uint8_t pin);
};

extern void extraDigitalWrite(uint8_t pin, uint8_t value);
extern uint8_t extraDigitalRead(uint8_t pin);
extern void extraPinMode(uint8_t pin, uint8_t mode);

class ExtraPins {
public:
    void digitalWrite(uint8_t pin, uint8_t value);
    uint8_t digitalRead(uint8_t pin);
    void pinMode(uint8_t pin, uint8_t mode);
};

class ExtraPinManager {
public:
    static ExtraPinManager *getInstance(void);
    void registerPin(uint8_t pin, ExtraPinSource *source, uint8_t localPin);
    ExtraPinSource *getPinSource(uint8_t pin);
private:
    ExtraPinManager();
    ExtraPinSource *p_sources[MAX_PIN_SOURCES] = {0};
    struct {
        uint8_t pin;
        uint8_t sourceIndex;
    } p_sourceMap[MAX_EXTRA_PINS];
};


#endif

// vim:ts=4:sw=4:ai:et:si:sts=4
