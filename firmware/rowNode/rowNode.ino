#include <EnableInterrupt.h>
#include "TMP05.h"
#include "mcp2515.h"
#include "SPI-74HC597D.h"

TMP05 sensors(4, 8, 9);
MCP2515 mcp2515(10);
SPI_74HC597D shiftReg(5, 4, 7);
uint8_t canID;
uint8_8t reading[4];
struct can_frame frame;

void setup() {
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS, MCP_20MHZ);
  mcp2515.setNormalMode();

  canID = ~(shiftReg.getValue());
}

void loop() {
  uint8_t i;
  
  if (sensors.getState() == -1) {
    for (i=0; i < 4; i++) {
      reading[i] = sensors.getReading(i);
    }

    frame.can_id = canID & 0x7FF;
    frame.can_dlc = 8;
    for (i=0; i < 4; i++) {
      frame.data[2*i] = uint8_t((reading[i] & 0xFF00) >> 8);
      frame.data[2*i + 1] = uint8_t(reading[i] & 0x00FF);
    }
    mcp2515.sendMessage(&frame);
    
    sensors.startConversion();
  }
  delay(1);
}  
