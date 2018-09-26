#include <EnableInterrupt.h>
#include "TMP05.h"
#include "mcp2515.h"
#include "SPI-74HC597D.h"
#include "Beirdo-CANBUS.h"
#include "PCA9536.h"
#include "extraPins.h"
#include "extraPin_PCA9536.h"
#include "vectoredInterrupt.h"

typedef struct {
    uint8_t start;
    uint8_t count;
    uint8_t outpin;
    uint8_t inpin;
} sensor_config_t;

sensor_config_t sensor_config[5] = {
    {0, 4, 4, 3},
    {4, 4, 6, 5},
    {8, 4, 8, 7},
    {12, 4, 17, 16},
    {16, 4, 15, 14}
};


TMP05 *sensors[5];
MCP2515 mcp2515(10);
SPI_74HC597D shiftReg(20, 21, 9);
Beirdo_CANBUS *canbus;
uint8_t canID;
PCA9536 pca9536;
ExtraPinManager *pinManager = NULL;
ExtraPin_PCA9536 pins_pca9536(&pca9536);
VectoredInterruptHandler *vectorManager;

void setup() {
    pinManager = ExtraPinManager.getInstance();
    vectorManager = new VectoredInterruptHandler(6);

    uint8_t i;
    for (i = 0; i < 5; i++) {
        sensor_config_t *config = &sensor_config[i];
        sensors[i] = new TMP05(config->count, config->outpin, config->inpin);
    }

    mcp2515.reset();
    mcp2515.setBitrate(CAN_125KBPS, MCP_20MHZ);
    mcp2515.setNormalMode();

    Wire.begin();
    pca9536.reset();

    pinManager->registerPin(20, &pins_pca9536, 0);
    pinManager->registerPin(21, &pins_pca9536, 1);
    pinManager->registerPin(22, &pins_pca9536, 2);
    pinManager->registerPin(23, &pins_pca9536, 3);

    canID = ~(shiftReg.getValue());
    canbus = new Beirdo_CANBUS(&mcp2515, canID, 22, 0);
}

void loop() {
    uint8_8t reading;
    uint8_t i;
    uint8_t j;
    
    for (i = 0; i < 5; i++) {
        TMP05 *sensor = sensors[i];
        if (sensor->getState() == -1) {
            sensor_config_t *config = &sensor_config[i];

            for (j = 0; j < config->count; j++) {
                reading = sensor->getReading(j);
                canbus->addReading(config->start + j, reading);
            }
            sensor->startConversion();
        }
    }

    canbus->sendAllReadings();
    delay(1);
}  

// vim:ts=4:sw=4:ai:et:si:sts=4
