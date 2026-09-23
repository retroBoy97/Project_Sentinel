#ifndef DRV_LED_H
#define DRV_LED_H

#include "stm32f4xx.h"

typedef enum{
	DRV_LED_OK,
	DRV_LED_ERROR
}DRV_LED_STATUS;

typedef struct{
	GPIO_TypeDef* gpio_port;
	uint8_t pin;
}Led;

static const Led GREEN_LED = {GPIOC, 5U};
static const Led BLUE_LED = {GPIOC, 8U};
static const Led RED_LED = {GPIOC, 6U};

DRV_LED_STATUS turn_on_led(const Led* led);
DRV_LED_STATUS turn_off_led(const Led* led);
DRV_LED_STATUS toggle_led(const Led* led);

#endif /* DRV_LED_H */
