#include <EnableInterrupt.h>
#include "TMP05.h"
#include "mcp2515.h"

TMP05 sensors(4, 8, 9);
MCP2515 mcp2515(10);

void setup() {
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS, MCP_20MHZ);
  mcp2515.setNormalMode();
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
