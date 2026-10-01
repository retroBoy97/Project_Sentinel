#include "keypad.h"
#include "drv_gpio.h"
#include "usart.h"
#include <string.h>

char PASSWORD[KEYPAD_PASSWORD_MAX_LENGTH + 1U] = "8686";
uint8_t password_length = 4U;

static const char KEYPAD_MAP[4][4] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'}
};

/* Initializing keypad pins */
KEYPAD_STATUS keypad_init(const Keypad* keypad, char* password){
	DRV_GPIO_STATUS row_status;
	DRV_GPIO_STATUS column_status;

	for (int i = 0; i < 4; i++){
		// Initialize row pins as output
		row_status = drv_gpio_init_output(keypad -> rows_gpio_port,
									  keypad -> row_pins[i],
									  DRV_GPIO_HIGH);

		// Initialize column pins as input pull-up
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

/* Scans the entered numbers by the user through the keypad */
KEYPAD_STATUS keypad_scan(const Keypad* keypad){
	static char entered_password[KEYPAD_PASSWORD_MAX_LENGTH] = "";
	static char entered_character_index;
	DRV_GPIO_STATUS status;

	// Loop over the rows
	for (uint8_t row = 0; row < 4; row++){
		// Drive the rows LOW one at a time
		status = drv_gpio_write(keypad -> rows_gpio_port, keypad -> row_pins[row], DRV_GPIO_LOW);

		// Return KEYPAD_ERROR in case the write fails
		if(status == DRV_GPIO_ERROR){
			return KEYPAD_ERROR;
		}

		// Loop over the columns
		for (uint8_t column = 0; column < 4; column++){
			DRV_GPIO_LEVEL column_level;
			status = drv_gpio_read(keypad -> columns_gpio_port, keypad -> column_pins[column], &column_level);

			// Return KEYPAD_ERROR if the read fails
			if (status == DRV_GPIO_ERROR){
				return KEYPAD_ERROR;
			}

			// Check if the current column reads LOW
			if (column_level == DRV_GPIO_LOW){
				entered_password[entered_character_index] = KEYPAD_MAP[row][column];
				entered_character_index++;
				HAL_UART_Transmit(&huart2, (uint8_t*) &KEYPAD_MAP[row][column], 1, 10000);

				if (entered_character_index == password_length){
					// Validate the entered passowrd
					//validate_password(entered_password, PASSWORD);
					entered_character_index = 0;
				}
			}
		}

		// Drive the row back to HIGH
		status = drv_gpio_write(keypad -> rows_gpio_port, keypad -> row_pins[row], DRV_GPIO_HIGH);
	}

	return KEYPAD_OK;

}

// To be implemented: Logic with screen, buzzer and Leds
void validate_password(char entered_password[], char valid_password[]){
	int cmp = strcmp(entered_password, valid_password);
	const char success_msg[] = "correct password";
	const char error_msg[] = "wrong password, try again";

	if (cmp == 0){
		HAL_UART_Transmit(&huart2, (uint8_t*)success_msg, strlen(success_msg), 10000);
	}
	else{
		HAL_UART_Transmit(&huart2, (uint8_t*)error_msg, strlen(error_msg), 10000);

	}
}

























