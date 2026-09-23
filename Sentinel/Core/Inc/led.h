#ifndef LED_H
#define LED_H

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

DRV_LED_STATUS led_turn_on(const Led* led);
DRV_LED_STATUS led_turn_off(const Led* led);
DRV_LED_STATUS led_toggle(const Led* led);

#endif /* LED_H */
