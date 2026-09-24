#ifndef BUZZER_H
#define BUZZER_H

#include "stm32f4xx.h"

typedef struct{
	GPIO_TypeDef* gpio_port;
	uint8_t pin;
}Buzzer;

typedef enum{
	BUZZER_OK,
	BUZZER_ERROR
}BUZZER_STATUS;

static const Buzzer BUZZER = {GPIOC, 9U};

BUZZER_STATUS buzzer_init(const Buzzer* buzzer);
BUZZER_STATUS buzzer_turn_on(const Buzzer* buzzer);
BUZZER_STATUS buzzer_turn_off(const Buzzer* buzzer);
BUZZER_STATUS buzzer_toggle(const Buzzer* buzzer);

#endif /* BUZZER_H */
