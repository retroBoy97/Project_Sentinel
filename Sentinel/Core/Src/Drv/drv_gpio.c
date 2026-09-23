#include "drv_gpio.h"

#define DRV_GPIO_PIN_MAX (15U)

static DRV_GPIO_STATUS drv_gpio_enable_clock(GPIO_TypeDef* gpio_port){
	if (gpio_port == GPIOA){
		RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	}
	else if (gpio_port == GPIOB){
		RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	}
	else if (gpio_port == GPIOC){
		RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	}
	else if (gpio_port == GPIOD){
		RCC -> AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	}
	else if (gpio_port == GPIOE){
		RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	}
	else if (gpio_port == GPIOH){
		RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOHEN;
	}
	else{
		return DRV_GPIO_ERROR;
	}

	return DRV_GPIO_OK;
}

DRV_GPIO_STATUS drv_gpio_init_output(GPIO_TypeDef* gpio_port, uint8_t pin, DRV_GPIO_LEVEL level){
	if (pin > DRV_GPIO_PIN_MAX){
		return DRV_GPIO_ERROR;
	}

	if (drv_gpio_enable_clock(gpio_port) != DRV_GPIO_OK){
		return DRV_GPIO_ERROR;
	}

	// Set pin level
	if (level == DRV_GPIO_HIGH){
		gpio_port -> BSRR = (1U << pin);
	}
	else if (level == DRV_GPIO_LOW){
		gpio_port -> BSRR = (1U << (pin + 16));
	}

	// Configure OTYPER to push-pull mode
	gpio_port -> OTYPER &= ~(1U << pin);

	// Configure OSPEEDR pin to LOW speed: how fast the voltage ramps from 0V to 3.3V (or back)
	gpio_port -> OSPEEDR &= ~(1U << (2 * pin));
	gpio_port -> OSPEEDR &= ~(1U << (2 * pin + 1));

	// Configure PUPDR to no pull: this must happen before MODER, since PUPDR only has any
	// effect while the pin isn't yet being actively driven, i.e. the gap before this point
	gpio_port -> PUPDR &= ~(1U << (2 * pin));
	gpio_port -> PUPDR &= ~(1U << (2 * pin + 1));

	// Set Output Mode for pin, last: everything above must be in place before the pin goes live
	gpio_port -> MODER |= (1U << (2 * pin));
	gpio_port -> MODER &= ~(1U << (2 * pin + 1));

	return DRV_GPIO_OK;
}

DRV_GPIO_STATUS drv_gpio_init_input(GPIO_TypeDef* gpio_port, uint8_t pin, DRV_GPIO_PULL pull){
	if (pin > DRV_GPIO_PIN_MAX){
		return DRV_GPIO_ERROR;
	}

	if (drv_gpio_enable_clock(gpio_port) != DRV_GPIO_OK){
		return DRV_GPIO_ERROR;
	}

	// Set Input Mode for pin (00)
	gpio_port -> MODER &= ~(1U << 2 * pin);
	gpio_port -> MODER &= ~(1U << (2 * pin + 1));

	// Configure PUPDR to the requested pull (field is cleared first, then set to the chosen value,
	// since "pull" is a variable here rather than a single fixed target like output mode was)
	gpio_port -> PUPDR &= ~(3U << (2U * pin));
	gpio_port -> PUPDR |= ((uint32_t)pull << (2U * pin));

	return DRV_GPIO_OK;
}

DRV_GPIO_STATUS drv_gpio_write(GPIO_TypeDef* gpio_port, uint8_t pin, DRV_GPIO_LEVEL level){


}

DRV_GPIO_STATUS drv_gpio_toggle(GPIO_TypeDef* gpio_port, uint8_t pin){

}

















