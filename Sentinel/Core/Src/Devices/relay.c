#include "relay.h"
#include "drv_gpio.h"

RELAY_STATUS relay_init(const Relay* relay){
	DRV_GPIO_STATUS status = drv_gpio_init_output(relay -> gpio_port, relay -> in_pin, DRV_GPIO_LOW);

	if (status == DRV_GPIO_ERROR){
		return RELAY_ERROR;
	}
	return RELAY_OK;
}

RELAY_STATUS relay_cut_power(const Relay* relay){
	DRV_GPIO_STATUS status = drv_gpio_write(relay -> gpio_port, relay -> in_pin, DRV_GPIO_HIGH);

	if (status == DRV_GPIO_ERROR){
		return RELAY_ERROR;
	}
	return RELAY_OK;
}

RELAY_STATUS relay_restore_power(const Relay* relay){
	DRV_GPIO_STATUS status = drv_gpio_write(relay -> gpio_port, relay -> in_pin, DRV_GPIO_LOW);

	if (status == DRV_GPIO_ERROR){
		return RELAY_ERROR;
	}
	return RELAY_OK;
}
