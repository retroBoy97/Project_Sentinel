#ifndef KEYPAD_H
#define KEYPAD_H

#include "stm32f4xx.h"

typedef struct{
	GPIO_TypeDef* rows_gpio_port;
	GPIO_TypeDef* columns_gpio_port;

	uint8_t row_pins[4];
	uint8_t column_pins[4];
}Keypad;

typedef enum{
	KEYPAD_OK,
	KEYPAD_ERROR
}KEYPAD_STATUS;

static const Keypad KEYPAD = {GPIOC,
							  GPIOA,
							  {0U, 1U, 2U, 3U},
							  {0U, 1U, 4U, 6U}
							 };

#define KEYPAD_PASSWORD_MAX_LENGTH (4U)

extern char PASSWORD[KEYPAD_PASSWORD_MAX_LENGTH + 1U];
extern uint8_t password_length;

KEYPAD_STATUS keypad_init(const Keypad* keypad, char* password);
KEYPAD_STATUS keypad_set_password(const char* new_password, uint8_t new_length);
KEYPAD_STATUS keypad_scan(const Keypad* keypad);

#endif /* KEYPAD_H */
