#include "hc164.h"
#include "drv_gpio.h"

/* Initializes HC164 gpio pins */
HC164_STATUS hc164_init(const Hc164* hc164){
	// Safety check: validate hc164 is not NULL
	if (hc164 == NULL){
		return HC164_ERROR;
	}

	// Init hc164 pins as outputs, LOW (CLK idles LOW so the first HIGH write is a rising edge)
	DRV_GPIO_STATUS ds_gpio_status = drv_gpio_init_output(hc164 -> ds_port, hc164 -> ds_pin, DRV_GPIO_LOW);
	DRV_GPIO_STATUS clk_gpio_status = drv_gpio_init_output(hc164 -> clk_port, hc164 -> clk_pin, DRV_GPIO_LOW);

	if (ds_gpio_status == DRV_GPIO_ERROR || clk_gpio_status == DRV_GPIO_ERROR){
		return HC164_ERROR;
	}

	return HC164_OK;
}

/* Shifts one bit into the shift register: the outputs (QA-QH) change immediately */
HC164_STATUS hc164_shift_bit(const Hc164* hc164, DRV_GPIO_LEVEL bit){
	// Safety check: validate hc164 is not NULL
	if (hc164 == NULL){
		return HC164_ERROR;
	}

	// Set DS before the CLK rising edge, since the 164 reads DS at that instant
	DRV_GPIO_STATUS ds_status = drv_gpio_write(hc164 -> ds_port, hc164 -> ds_pin, bit);

	// Pulse CLK: on the rising edge, the bit enters QA and every other bit moves one stage toward QH
	DRV_GPIO_STATUS clk_high_status = drv_gpio_write(hc164 -> clk_port, hc164 -> clk_pin, DRV_GPIO_HIGH);
	// Back to idle, ready for the next pulse
	DRV_GPIO_STATUS clk_low_status = drv_gpio_write(hc164 -> clk_port, hc164 -> clk_pin, DRV_GPIO_LOW);

	if (ds_status == DRV_GPIO_ERROR || clk_high_status == DRV_GPIO_ERROR || clk_low_status == DRV_GPIO_ERROR){
		return HC164_ERROR;
	}

	return HC164_OK;
}
