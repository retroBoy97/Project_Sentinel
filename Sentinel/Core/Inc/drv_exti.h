#ifndef DRV_EXTI_H
#define DRV_EXTI_H

#include "stm32f4xx.h"

typedef enum{
	DRV_EXTI_RISING_EDGE,
	DRV_EXTI_FALLING_EDGE,
	DRV_EXTI_BOTH_EDGES
}DRV_EXTI_EDGE;

typedef enum{
	DRV_EXTI_OK,
	DRV_EXTI_ERROR
}DRV_EXTI_STATUS;

/* Configures a GPIO pin as an EXTI interrupt source for the given edge */
DRV_EXTI_STATUS drv_exti_init(GPIO_TypeDef* gpio_port, uint8_t pin, DRV_EXTI_EDGE edge);
/* Configures NVIC for the EXTI Interrupt*/
DRV_EXTI_STATUS drv_exti_enable(IRQn_Type irqn, uint8_t priority);
/* Clears the pending bit for the EXTI line */
DRV_EXTI_STATUS drv_exti_clear_pending(uint8_t pin);
/* Checks whether a given EXTI line's pending flag is currently set */
DRV_EXTI_STATUS drv_exti_is_pending(uint8_t pin, uint8_t* is_pending);


#endif /* DRV_EXTI_H */
