#ifndef __extraPin_PCA9536_h__
#define __extraPin_PCA9536_h__

#include <Arduino.h>
#include <inttypes.h>
#include "extraPins.h"
#include <PCA9536.h>

class ExtraPin_PCA9536 : public ExtraPinSource {
public:
    ExtraPin_PCA9536(PCA9536 *device);
    virtual void digitalWrite(uint8_t pin, uint8_t value);
    virtual uint8_t digitalRead(uint8_t pin);
    virtual void pinMode(uint8_t pin, uint8_t mode);
protected:
    PCA9536 *p_device;
};

#endif

// vim:ts=4:sw=4:ai:et:si:sts=4
