#include "SPI-74HC597D.h"

SPI_74HC597D::SPI_74HC597D(uint8_t ld_pin, uint8_t latch_pin, uint8_t cs_pin)
{
    p_ld_pin = ld_pin;
    p_latch_pin = latch_pin;
    p_cs_pin = cs_pin;
    p_pulled = 0;
    p_value = 0;

    digitalWrite(p_ld_pin, LOW);
    digitalWrite(p_latch_pin, HIGH);
    digitalWrite(p_cs_pin, HIGH);

    pinMode(p_ld_pin, OUTPUT);
    pinMode(p_latch_pin, OUTPUT);
    pinMode(p_cs_pin, OUTPUT);

    SPI.begin();
}

uint8_t SPI_74HC597D::getValue(void)
{
    if (!p_pulled) {
        loadValue();
        latchValue();
        startSPI();
        p_value = SPI.transfer(0);
        endSPI();
        p_pulled = 1;
    }
    return p_value;
}

void SPI_74HC597D::loadValue(void)
{
    digitalWrite(p_ld_pin, LOW);
    digitalWrite(p_ld_pin, HIGH);
    digitalWrite(p_ld_pin, LOW);
}

void SPI_74HC597D::latchValue(void)
{
    digitalWrite(p_latch_pin, HIGH);
    digitalWrite(p_latch_pin, LOW);
    digitalWrite(p_latch_pin, HIGH);
}

void SPI_74HC597D::startSPI(void)
{
    SPI.beginTransaction(SPISettings(SPI_CLOCK, MSBFIRST, SPI_MODE0));
    digitalWrite(p_cs_pin, LOW);
}

void SPI_74HC597D::endSPI(void)
{
    digitalWrite(p_cs_pin, HIGH);
    SPI.endTransaction();
}

// vim:ts=4:sw=4:ai:et:si:sts=4
