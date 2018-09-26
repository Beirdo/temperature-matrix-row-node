#ifndef __vectoredInterrupt_h__
#define __vectoredInterrupt_h__

#include <Arduino.h>
#include <inttypes.h>

extern VectoredInterruptHandler *vectorManager;

typedef struct {
    uint8_t pin;
    void (*handler)(void);
    uint8_t mode;
} handler_map_t;

class VectoredInterruptHandler 
{
public:
    VectoredInterruptHandler(uint8_t count);
    void interruptHandler(uint8_t pin);
    void registerHandler(uint8_t pin, void (*handler)(void), uint8_t mode);
    void unregisterHandler(uint8_t pin);

protected:
    uint8_t getPinIndex(uint8_t pin);

private:
    uint8_t p_count;
    handler_map_t *p_handlerMap;
};

extern void attachVectoredInterrupt(uint8_t pin, void (*handler)(void), uint8_t mode);
extern void detachVectoredInterrupt(uint8_t pin);

#endif

// vim:ts=4:sw=4:ai:et:si:sts=4
