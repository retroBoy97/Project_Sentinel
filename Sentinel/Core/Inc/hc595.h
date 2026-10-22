#ifndef HC595_H
#define HC595_H

#include "stm32f4xx.h"

typedef enum{
	HC595_OK,
	HC595_ERROR
}HC595_STATUS;

typedef struct{
	GPIO_TypeDef* ds_port;   // Serial data input
	uint8_t ds_pin;
	GPIO_TypeDef* shcp_port; // Shift register clock
	uint8_t shcp_pin;
	GPIO_TypeDef* stcp_port; // Storage register clock (latch)
	uint8_t stcp_pin;
}Hc595;

/* Initializes HC595 gpio pins */
HC595_STATUS hc595_init(const Hc595* hc595);
/* Updates the content of the shift register by shifting bit by bit */
HC595_STATUS hc595_shift_byte(const Hc595* hc595, uint8_t byte);
/* Creates a rising edge ST_CP to copy the content of Shift Register into Storage Register */
HC595_STATUS hc595_latch(const Hc595* hc595);

#endif /* HC595_H */
