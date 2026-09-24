#ifndef RELAY_H
#define RELAY_H

#include "stm32f4xx.h"

typedef struct{
	GPIO_TypeDef* gpio_port;
	uint8_t in_pin;
}Relay;

typedef enum{
	RELAY_OK,
	RELAY_ERROR
}RELAY_STATUS;

static const Relay RELAY = {GPIOC, 11U};

RELAY_STATUS relay_init(const Relay* relay);
RELAY_STATUS relay_cut_power(const Relay* relay);
RELAY_STATUS relay_restore_power(const Relay* relay);

#endif /* RELAY_H */
