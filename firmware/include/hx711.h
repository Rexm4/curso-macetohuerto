#ifndef HX_711_H__
#define HX_711_H__

#include <driver/gpio.h>

typedef enum {
  HX711_CHA_128 = 0,
  HX711_CHB_32,
  HX711_CHB_64,
} Hx711Channel;

typedef struct {
  gpio_num_t gpioData;
  gpio_num_t gpioSck;
  Hx711Channel ch;
  int32_t tare;
  float offset;
  float scale;
} Hx711;

uint8_t hx711_init(Hx711* dev);

uint8_t hx711_readValueRaw(Hx711* dev, int32_t* value);
uint8_t hx711_readValueScaled(Hx711* dev, float* grams);

uint8_t hx711_setTare(Hx711* dev, int32_t tare);
uint8_t hx711_setScaleOffset(Hx711* dev, float scale, float offset);


#endif  //HX_711_H__