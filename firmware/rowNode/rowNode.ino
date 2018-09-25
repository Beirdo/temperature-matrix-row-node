#include <EnableInterrupt.h>
#include "TMP05.h"
#include "mcp2515.h"
#include "SPI-74HC597D.h"
#include "Beirdo-CANBUS.h"

TMP05 sensors(4, 8, 9);
MCP2515 mcp2515(10);
SPI_74HC597D shiftReg(5, 4, 7);
Beirdo_CANBUS *canbus;
uint8_t canID;

void setup() {
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS, MCP_20MHZ);
  mcp2515.setNormalMode();

  canID = ~(shiftReg.getValue());
  canbus = new Beirdo_CANBUS(&mcp2515, canID);
}

void loop() {
  uint8_8t reading;
  uint8_t i;
  
  if (sensors.getState() == -1) {
    for (i=0; i < 4; i++) {
      reading = sensors.getReading(i);
      canbus->addReading(i, reading);
    }
    canbus->sendAllReadings();
    sensors.startConversion();
  }
  delay(1);
}  
