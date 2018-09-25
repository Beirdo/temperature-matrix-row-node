#ifndef __SPI_74HC597D_h__
#define __SPI_74HC597D_h__

#include <Arduino.h>
#include <inttypes.h>
#include <SPI.h>
#include <extraPins.h>

class SPI_74HC597D : protected ExtraPins {
public:
    SPI_74HC597D(uint8_t ld_pin, uint8_t latch_pin, uint8_t cs_pin);
    uint8_t getValue(void);

    static const uint32_t SPI_CLOCK = 1000000; // 1MHz

protected:
    void loadValue(void);
    void latchValue(void);
    void startSPI(void);
    void endSPI(void);

private:
    uint8_t p_ld_pin;
    uint8_t p_latch_pin;
    uint8_t p_cs_pin;

    uint8_t p_pulled;
    uint8_t p_value;
};

#endif

// vim:ts=4:sw=4:ai:et:si:sts=4

