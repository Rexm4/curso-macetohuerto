#ifndef SENSORS_C__
#define SENSORS_C__

#include <driver/i2c_master.h>

typedef struct {
  i2c_master_dev_handle_t bmeDev;
  i2c_master_dev_handle_t adsDev;
  gpio_num_t hx711Sck;
  gpio_num_t hx711Data;
} SensorConfig;

typedef struct {
  struct {
    float pressure;
    float humidity;
    float airTemp;
  } bme;
  float adcLDR;
  float adcHumidity;
  float grams;
} SensorData;

void sensors_init(SensorConfig* config);

void sensors_update(SensorData* data);

void sensors_calibrate(void);

#endif  //SENSORS_C__