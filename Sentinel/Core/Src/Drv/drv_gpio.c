#include "drv_gpio.h"


uint8_t drv_gpio_init(GPIO_Typedef* gpio_port, uint8_t pin);
uint8_t drv_gpio_write(GPIO_Typedef* gpio_port, uint8_t pin, DRV_GPIO_LVL level);
uint8_t drv_gpio_toggle(GPIO_Typedef* gpio_port, uint8_t pin);
