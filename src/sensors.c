#include "sensors.h"
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include "IBme280.h"
#include "ads1115.h"
#include "bme280.h"
#include "hal/ads1115_hals.h"

struct {
  struct bme280_dev bmedev;
  Ads1115 ads;
} sensors;

static char* TAG = "SENSORS";

void sensors_init(SensorConfig* config) {

  sensors.bmedev.read     = ibme280_i2c_read;
  sensors.bmedev.write    = ibme280_i2c_write;
  sensors.bmedev.delay_us = ibme280_delay;
  sensors.bmedev.intf_ptr = &(config->bmeDev);
  sensors.bmedev.intf     = BME280_I2C_INTF;
  if (bme280_init(&sensors.bmedev) == 0) {
    ESP_LOGE(TAG, "BME280 Initialized");
  } else {
    while (1) {
      ESP_LOGE(TAG, "BME280 ERROR!");
      vTaskDelay(pdMS_TO_TICKS(1000));
    }
  }

  struct bme280_settings settings;
  bme280_get_sensor_settings(&settings, &sensors.bmedev);
  settings.osr_h = BME280_OVERSAMPLING_16X;
  settings.osr_p = BME280_OVERSAMPLING_16X;
  settings.osr_t = BME280_OVERSAMPLING_16X;

  bme280_set_sensor_settings(BME280_SEL_ALL_SETTINGS, &settings, &sensors.bmedev);

  bme280_set_sensor_mode(BME280_POWERMODE_NORMAL, &sensors.bmedev);

  ads1115_esp32Create(&sensors.ads, &config->adsDev);

  Ads1115Config adsConf = {
      .mode = ADS1115_MODE_CONTINUOUS,
      .fsr  = ADS1115_FSR_4_096V,
      .dr   = ADS1115_DR_128SPS,
      .mux  = ADS1115_MUX_AIN0_GND,
  };

  ads1115_config(&sensors.ads, &adsConf);
}

void sensors_update(SensorData* data) {
  struct bme280_data bmedata;
  bme280_get_sensor_data(BME280_ALL, &bmedata, &sensors.bmedev);

  data->bme.pressure = bmedata.pressure;
  data->bme.humidity = bmedata.humidity;
  data->bme.airTemp  = bmedata.temperature;

  data->adcValueV = ads1115_readVolts(&sensors.ads);
}
