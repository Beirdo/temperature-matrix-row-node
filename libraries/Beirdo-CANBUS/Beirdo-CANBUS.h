#ifndef __Beirdo_CANBUS_h__
#define __Beirdo_CANBUS_h__

#include <Arduino.h>
#include <inttypes.h>
#include <mcp2515.h>
#include <extraPins.h>

#define MAX_READINGS 16

typedef struct {
    uint8_t *buffer;
    uint8_t buflen;
} canbus_buffer_t;

class Beirdo_CANBUS : protected ExtraPins {
public:
    Beirdo_CANBUS(MCP2515 *controller, uint8_t canID, uint8_t stbyPin, uint8_t intPin);
    void clearReadings(void);
    void queueReading(uint8_t *buffer, uint8_t buflen);
    void sendAllReadings(void);
    uint8_t readingCount(void);
    void canbusInterruptHandler(void);
    void canbusInterruptSlowHandler(void);

protected:

private:
    MCP2515 *p_controller;
    uint8_t p_canID;
    uint8_t p_stbyPin;
    uint8_t p_intPin;
    struct can_frame p_frame;
    canbus_buffer_t *p_readings[MAX_READINGS];
    uint8_t p_writeIndex;
    uint8_t p_readIndex;
    uint8_t p_interrupt;
};

#endif

// vim:ts=4:sw=4:ai:et:si:sts=4

