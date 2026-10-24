#ifndef HC164_H
#define HC164_H

#include "stm32f4xx.h"
#include "drv_gpio.h"

typedef enum{
	HC164_OK,
	HC164_ERROR
}HC164_STATUS;

typedef struct{
	GPIO_TypeDef* ds_port;  // Serial data input (A; B is tied to 3.3 V)
	uint8_t ds_pin;
	GPIO_TypeDef* clk_port; // Shift register clock
	uint8_t clk_pin;
}Hc164;

static const Hc164 HC164 = {GPIOB, 1U, GPIOD, 2U};

/* Initializes HC164 gpio pins */
HC164_STATUS hc164_init(const Hc164* hc164);
/* Shifts one bit into the shift register: the outputs (QA-QH) change immediately */
HC164_STATUS hc164_shift_bit(const Hc164* hc164, DRV_GPIO_LEVEL bit);

#endif /* HC164_H */
