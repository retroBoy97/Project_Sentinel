#include "drv_led.h"
#include "drv_gpio.h"

DRV_LED_STATUS turn_on_led(const Led* led){
	DRV_GPIO_STATUS status = drv_gpio_write(led -> gpio_port, led -> pin, DRV_GPIO_HIGH);

	if (status == DRV_GPIO_ERROR){
		return DRV_LED_ERROR;
	}
	return DRV_LED_OK;
}

DRV_LED_STATUS turn_off_led(const Led* led){
	DRV_GPIO_STATUS status = drv_gpio_write(led -> gpio_port, led -> pin, DRV_GPIO_LOW);

	if (status == DRV_GPIO_ERROR){
		return DRV_LED_ERROR;
	}
	return DRV_LED_OK;
}

DRV_LED_STATUS toggle_led(const Led* led){
	DRV_GPIO_STATUS status = drv_gpio_toggle(led -> gpio_port, led -> pin);

	if (status == DRV_GPIO_ERROR){
		return DRV_LED_ERROR;
	}
	return DRV_LED_OK;
}



