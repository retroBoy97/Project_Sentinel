#ifndef DRV_GPIO_H
#define DRV_GPIO_H

#include "stm32f4xx.h"

typedef enum{
	DRV_GPIO_HIGH = 1U,
	DRV_GPIO_LOW = 0U
}DRV_GPIO_LEVEL;

typedef enum{
	DRV_GPIO_PULL_NONE = 0U,
	DRV_GPIO_PULL_UP   = 1U,
	DRV_GPIO_PULL_DOWN = 2U
}DRV_GPIO_PULL;

typedef enum{
	DRV_GPIO_OK,
	DRV_GPIO_ERROR
}DRV_GPIO_STATUS;

DRV_GPIO_STATUS drv_gpio_init_output(GPIO_TypeDef* gpio_port, uint8_t pin, DRV_GPIO_LEVEL level);
DRV_GPIO_STATUS drv_gpio_init_input(GPIO_TypeDef* gpio_port, uint8_t pin, DRV_GPIO_PULL pull);
DRV_GPIO_STATUS drv_gpio_write(GPIO_TypeDef* gpio_port, uint8_t pin, DRV_GPIO_LEVEL level);
DRV_GPIO_STATUS drv_gpio_toggle(GPIO_TypeDef* gpio_port, uint8_t pin);

#endif /* DRV_GPIO_H */
