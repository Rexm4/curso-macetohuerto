#include <freertos/FreeRTOS.h>
#include "comms.h"
#include "esp_log.h"
#include "hx711.h"
#include "pump.h"
#include "scanneri2c.h"
#include "sensors.h"
#include "system.h"

#define CALIBRATION (1U)

void app_main() {

  /*
  while (1) {
    scanner_scan();
    vTaskDelay(pdMS_TO_TICKS(1000));
  }*/

  vTaskDelay(pdMS_TO_TICKS(3000));

  SystemDevs* sysDevs = system_init();

  SensorConfig sensorsconfig;
  sensorsconfig.bmeDev    = sysDevs->bme;
  sensorsconfig.adsDev    = sysDevs->ads;
  sensorsconfig.hx711Data = sysDevs->hx711Data;
  sensorsconfig.hx711Sck  = sysDevs->hx711Sck;
  sensors_init(&sensorsconfig);
  pump_init(sysDevs->pumpGpio);

#if CALIBRATION
  sensors_calibrate();
#endif

  SensorData data;
  for (;;) {
    sensors_update(&data);

    printf("LUZ: %2.4f | HUM: %2.4f | P: %2.2f | H: %2.2f | T: %2.2f | grams: %4.4f\n", data.adcLDR,
           data.adcHumidity, data.bme.pressure, data.bme.humidity, data.bme.airTemp, data.grams);
    //..

    comms_send();

    // Actuate pump if humidity is low
    /*if (data.adcHumidity < 20.0f) {
      ESP_LOGI("PUMP", "Actuating pump...");
      pump_actuate();
      ESP_LOGI("PUMP", "Pump actuation complete.");
    }*/

    system_sleep();

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}