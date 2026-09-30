#include "keypad.h"
#include "drv_gpio.h"
#include <string.h>

char PASSWORD[KEYPAD_PASSWORD_MAX_LENGTH + 1U] = "8686";
uint8_t password_length = 4U;

KEYPAD_STATUS keypad_init(const Keypad* keypad, char* password){
	DRV_GPIO_STATUS row_status;
	DRV_GPIO_STATUS column_status;

	for (int i = 0; i < 4; i++){
		row_status = drv_gpio_init_output(keypad -> rows_gpio_port,
									  keypad -> row_pins[i],
									  DRV_GPIO_HIGH);

		column_status = drv_gpio_init_input(keypad -> columns_gpio_port,
											keypad -> column_pins[i],
											DRV_GPIO_PULL_UP);

		if (row_status == DRV_GPIO_ERROR || column_status == DRV_GPIO_ERROR){
			return KEYPAD_ERROR;
		}
	}

	return KEYPAD_OK;
}

KEYPAD_STATUS keypad_set_password(const char* new_password, uint8_t new_length){
	if ((new_password == NULL) || (new_length == 0U) || (new_length > KEYPAD_PASSWORD_MAX_LENGTH)){
		return KEYPAD_ERROR;
	}

	memcpy(PASSWORD, new_password, new_length);
	PASSWORD[new_length] = '\0';
	password_length = new_length;

	return KEYPAD_OK;
}

KEYPAD_STATUS keypad_scan(const Keypad* keypad){

}
