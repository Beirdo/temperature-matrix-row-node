#include "extraPin_PCA9536.h"

ExtraPin_PCA9536::ExtraPin_PCA9536(PCA9536 *device) : ExtraPinSource(4), p_device(device)
{
}

void ExtraPin_PCA9536::digitalWrite(uint8_t pin, uint8_t value)
{
    uint8_t localPin = getLocalPin(pin);
    if (localPin == 0xFF) {
        return;
    }

    pin_t pcaPin = (pin_t)localPin;
    state_t pcaValue = (value == LOW ? IO_LOW : IO_HIGH);
    p_device->setState(pcaPin, pcaValue);
}

uint8_t ExtraPin_PCA9536::digitalRead(uint8_t pin)
{
    uint8_t localPin = getLocalPin(pin);
    if (localPin == 0xFF) {
        return 0xFF;
    }

    pin_t pcaPin = (pin_t)localPin;
    uint8_t pcaValue = p_device->getState(pcaPin);
    return (pcaValue == IO_LOW ? LOW : HIGH);
}

void ExtraPin_PCA9536::pinMode(uint8_t pin, uint8_t mode)
{
    uint8_t localPin = getLocalPin(pin);
    if (localPin == 0xFF) {
        return;
    }

    pin_t pcaPin = (pin_t)localPin;
    mode_t pcaMode = (mode == OUTPUT ? IO_OUTPUT : IO_INPUT);
    p_device->setPolarity(pcaPin, IO_NON_INVERTED);
    p_device->setMode(pcaPin, pcaMode);
}


// vim:ts=4:sw=4:ai:et:si:sts=4
