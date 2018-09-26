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

ExtraPinSource::ExtraPinSource(uint8_t count)
{
    p_count = count;
    p_pinReverse = new uint8_t[count];
    memset(p_pinReverse, 0xFF, count);
}

void ExtraPinSource::registerPin(uint8_t pin, uint8_t localPin)
{
    if (localPin >= p_count) {
        return;
    }
    p_pinReverse[localPin] = pin;
}

uint8_t ExtraPinSource::getLocalPin(uint8_t pin)
{
    uint8_t i;

    for (i = 0; i < p_count; i++) {
        if (p_pinReverse[i] == pin) {
            return i;
        }
    }
    return 0xFF;
}

ExtraPinManager::ExtraPinManager()
{
    memset((uint8_t)p_sources, 0x00, sizeof(p_sources));
    memset((uint8_t)p_sourceMap, 0xFF, sizeof(p_sourceMap));
}

static ExtraPinManager *ExtraPinManager::getInstance(void)
{
    static ExtraPinManager instance;

    return &instance;
}

void ExtraPinManager::registerPin(uint8_t pin, ExtraPinSource *source, uint8_t localPin)
{
    uint8_t i;
    uint8_t sourceIndex = 0xFF;
    uint8_t emptyIndex = 0xFF;
    uint8_t mapIndex = 0xFF;

    if (pin < NUM_DIGITAL_PINS) {
        return;
    }

    for (i = 0; i < MAX_PIN_SOURCES; i++) {
        if (!p_sources[i]) {
            emptyIndex = i;
            break;
        }
        
        if (p_sources[i] == source) {
            sourceIndex = i;
            break;
        }
    }

    if (emptyIndex != 0xFF) {
        p_sources[emptyIndex] = source;
        sourceIndex = emptyIndex;
    }

    if (sourceIndex == 0xFF) {
        return;
    }

    emptyIndex = 0xFF;
    for (i = 0; i < MAX_EXTRA_PINS; i++) {
        if (p_sourceMap[i].pin == 0xFF) {
            emptyIndex = i;
            break;
        }

        if (p_sourceMap[i].pin == pin) {
            mapIndex = i;
            break;
        }
    }

    if (emptyIndex != 0xFF) {
        p_sourceMap[emptyIndex].pin = pin;
        mapIndex = emptyIndex;
    }

    if (mapIndex != 0xFF) {
        p_sourceMap[mapIndex].sourceIndex = sourceIndex;
    }

    source->registerPin(pin, localPin);
}

ExtraPinSource *ExtraPinManager::getPinSource(uint8_t pin)
{
    uint8_t i;
    uint8_t sourceIndex = 0xFF;
    uint8_t mapIndex = 0xFF;

    if (pin < NUM_DIGITAL_PINS) {
        return NULL;
    }

    for (i = 0; i < MAX_EXTRA_PINS; i++) {
        if (p_sourceMap[i].pin == pin) {
            mapIndex = i;
            break;
        }
    }

    if (mapIndex == 0xFF) {
        return NULL;
    }

    sourceIndex = p_sourceMap[mapIndex].sourceIndex;
    if (sourceIndex == 0xFF) {
        return NULL;
    }

    return p_sources[sourceIndex];
}

void extraDigitalWrite(uint8_t pin, uint8_t value)
{
    ExtraPinManager *manager = ExtraPinManager::getInstance();
    ExtraPinSource *source = manager->getPinSource(pin);

    if (!source) {
        return;
    }

    source->digitalWrite(pin, value);
}

uint8_t extraDigitalRead(uint8_t pin)
    ExtraPinManager *manager = ExtraPinManager::getInstance();
    ExtraPinSource *source = manager->getPinSource(pin);

    if (!source) {
        return 0xFF;
    }

    return source->digitalRead(pin);
}

void extraPinMode(uint8_t pin, uint8_t mode)
{
    ExtraPinManager *manager = ExtraPinManager::getInstance();
    ExtraPinSource *source = manager->getPinSource(pin);

    if (!source) {
        return;
    }

    source->pinMode(pin, mode);
}

// vim:ts=4:sw=4:ai:et:si:sts=4
