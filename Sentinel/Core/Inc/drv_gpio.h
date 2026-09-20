#include "stm32f4xx.h"

typedef enum {
	DRV_GPIO_INPUT,
	DRV_GPIO_OUTPUT
}DRV_GPIO_MODE;

typedef enum{
	DRV_GPIO_HIGH,
	DRV_GPIO_LOW
}DRV_GPIO_LEVEL;

typedef enum{
	DRV_GPIO_OK,
	DRV_GPIO_ERROR
}DRV_GPIO_STATUS;
