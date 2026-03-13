#include <freertos/FreeRTOS.h>
#include "comms.h"
#include "pump.h"
#include "scanneri2c.h"
#include "sensors.h"
#include "system.h"

void app_main() {

  /*
  while (1) {
    scanner_scan();
    vTaskDelay(pdMS_TO_TICKS(1000));
  }*/

  vTaskDelay(pdMS_TO_TICKS(3000));

  SystemDevs* sysDevs = system_init();

  SensorConfig sensorsconfig;
  sensorsconfig.bmeDev = sysDevs->bme;
  sensorsconfig.adsDev = sysDevs->ads;
  sensors_init(&sensorsconfig);

  SensorData data;
  for (;;) {
    sensors_update(&data);

    //printf("P: %.2f H: %.2f T: %.2f\n", data.bme.pressure, data.bme.humidity, data.bme.airTemp);
    printf("LUZ: %2.4f | HUM: %2.4f\n", data.adcLDR,data.adcHumidity);
    //..

    comms_send();
    pump_actuate();

    system_sleep();

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}