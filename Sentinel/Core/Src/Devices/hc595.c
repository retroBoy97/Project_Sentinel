#include "hc595.h"
#include "drv_gpio.h"

/* Creates a pulse (LOW -> HIGH -> LOW) on the given pin: the rising edge triggers the 595 */
static HC595_STATUS hc595_pulse(GPIO_TypeDef* gpio_port, uint8_t pin){
	// Rising edge (the pin idles LOW): the 595 acts here
	DRV_GPIO_STATUS high_status = drv_gpio_write(gpio_port, pin, DRV_GPIO_HIGH);
	// Back to idle, ready for the next pulse
	DRV_GPIO_STATUS low_status = drv_gpio_write(gpio_port, pin, DRV_GPIO_LOW);

	if (high_status == DRV_GPIO_ERROR || low_status == DRV_GPIO_ERROR){
		return HC595_ERROR;
	}

	return HC595_OK;
}

/* Initializes HC595 gpio pins */
HC595_STATUS hc595_init(const Hc595* hc595){
	// Safety check: validate hc595 is not NULL
	if (hc595 == NULL){
		return HC595_ERROR;
	}

	DRV_GPIO_STATUS ds_gpio_status, shcp_gpio_status, stcp_gpio_status;

	// Init hc595 pins
	ds_gpio_status = drv_gpio_init_output(hc595 -> ds_port, hc595 -> ds_pin, DRV_GPIO_LOW);
	shcp_gpio_status = drv_gpio_init_output(hc595 -> shcp_port, hc595 -> shcp_pin, DRV_GPIO_LOW);
	stcp_gpio_status = drv_gpio_init_output(hc595 -> stcp_port, hc595 -> stcp_pin, DRV_GPIO_LOW);

	if (ds_gpio_status == DRV_GPIO_ERROR || shcp_gpio_status == DRV_GPIO_ERROR || stcp_gpio_status == DRV_GPIO_ERROR){
		return HC595_ERROR;
	}

	return HC595_OK;
}

/* Updates the content of the shift register by shifting bit by bit */
HC595_STATUS hc595_shift_byte(const Hc595* hc595, uint8_t byte){
	// Safety check: validate hc595 is not NULL
	if (hc595 == NULL){
		return HC595_ERROR;
	}

	// Loop over the byte's bits, LSB first
	for (uint8_t i = 0U; i < 8U; i++){
		// Extract bit i: shift it to position 0, then mask out the others
		uint8_t bit = (byte >> i) & 1U;

		// Convert the bit to the level DS must take
		DRV_GPIO_LEVEL level;
		if (bit == 1U){
			level = DRV_GPIO_HIGH;
		}
		else{
			level = DRV_GPIO_LOW;
		}

		// Set DS before the SH_CP rising edge
		DRV_GPIO_STATUS ds_status = drv_gpio_write(hc595 -> ds_port, hc595 -> ds_pin, level);

		// Pulse SH_CP: the bit on DS enters stage 0 and every other bit moves one stage toward Q7
		HC595_STATUS shcp_status = hc595_pulse(hc595 -> shcp_port, hc595 -> shcp_pin);

		if (ds_status == DRV_GPIO_ERROR || shcp_status == HC595_ERROR){
			return HC595_ERROR;
		}
	}

	return HC595_OK;
}

/* Creates a rising edge ST_CP to copy the content of Shift Register into Storage Register */
HC595_STATUS hc595_latch(const Hc595* hc595){
	// Safety check: validate hc595 is not NULL
	if (hc595 == NULL){
		return HC595_ERROR;
	}

	// Pulse ST_CP: on its rising edge, the shift register's 8 bits are copied to the storage register (Q0-Q7)
	return hc595_pulse(hc595 -> stcp_port, hc595 -> stcp_pin);
}

