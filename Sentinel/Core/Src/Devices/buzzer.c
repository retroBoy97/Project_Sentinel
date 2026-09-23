#include "buzzer.h"
#include "drv_gpio.h"

BUZZER_STATUS buzzer_turn_on(const Buzzer* buzzer){
	BUZZER_STATUS status = drv_gpio_write(buzzer -> gpio_port, buzzer -> pin, DRV_GPIO_HIGH);

	if(status == DRV_GPIO_ERROR){
		return BUZZER_ERROR;
	}
	return BUZZER_OK;
}

BUZZER_STATUS buzzer_turn_off(const Buzzer* buzzer){
	BUZZER_STATUS status = drv_gpio_write(buzzer -> gpio_port, buzzer -> pin, DRV_GPIO_LOW);

	if(status == DRV_GPIO_ERROR){
		return BUZZER_ERROR;
	}
	return BUZZER_OK;
}

BUZZER_STATUS buzzer_toggle(const Buzzer* buzzer){
	BUZZER_STATUS status = drv_gpio_toggle(buzzer -> gpio_port, buzzer -> pin);

	if(status == DRV_GPIO_ERROR){
		return BUZZER_ERROR;
	}
	return BUZZER_OK;
}
