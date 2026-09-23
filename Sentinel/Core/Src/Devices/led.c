#include "led.h"
#include "drv_gpio.h"

LED_STATUS led_turn_on(const Led* led){
	DRV_GPIO_STATUS status = drv_gpio_write(led -> gpio_port, led -> pin, DRV_GPIO_HIGH);

	if (status == DRV_GPIO_ERROR){
		return LED_ERROR;
	}
	return LED_OK;
}

LED_STATUS led_turn_off(const Led* led){
	DRV_GPIO_STATUS status = drv_gpio_write(led -> gpio_port, led -> pin, DRV_GPIO_LOW);

	if (status == DRV_GPIO_ERROR){
		return LED_ERROR;
	}
	return LED_OK;
}

LED_STATUS led_toggle(const Led* led){
	DRV_GPIO_STATUS status = drv_gpio_toggle(led -> gpio_port, led -> pin);

	if (status == DRV_GPIO_ERROR){
		return LED_ERROR;
	}
	return LED_OK;
}
