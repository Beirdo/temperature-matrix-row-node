#include <EnableInterrupt.h>
#include "TMP05.h"
#include "mcp2515.h"
#include "SPI-74HC597D.h"

TMP05 sensors(4, 8, 9);
MCP2515 mcp2515(10);
SPI_74HC597D shiftReg(5, 4, 7);
uint8_t canID;

void setup() {
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS, MCP_20MHZ);
  mcp2515.setNormalMode();

  canID = ~(shiftReg.getValue());
}

void loop() {
  uint8_8t reading;
  
  if (sensors.getState() == -1) {
    for (uint8_t i=0; i < 4; i++) {
      reading = sensors.getReading(i);
      sensors.startConversion();
    }
  }
  delay(1);
}  
