#ifndef __Beirdo_CANBUS_h__
#define __Beirdo_CANBUS_h__

#include <Arduino.h>
#include <inttypes.h>
#include <mcp2515.h>

#ifndef uint8_8t
#define uint8_8t uint16_t
#endif

#define MAX_READINGS 8

class Beirdo_CANBUS {
public:
    Beirdo_CANBUS(MCP2515 *controller, uint8_t canID);
    void clearReadings(void);
    void addReading(uint8_t index, uint8_8t reading);
    void sendAllReadings(void);
    uint8_t readingCount(void);

protected:

private:
    MCP2515 *p_controller;
    uint8_t p_canID;
    struct can_frame p_frame;
    struct {
        uint8_t index;
        uint8_8t reading;
    } p_readings[MAX_READINGS];
    uint8_t p_writeIndex;
    uint8_t p_readIndex;
};

#endif

// vim:ts=4:sw=4:ai:et:si:sts=4

