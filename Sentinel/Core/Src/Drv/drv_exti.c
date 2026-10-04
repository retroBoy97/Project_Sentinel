#include "drv_exti.h"

/* Configures a GPIO pin as an EXTI interrupt source for the given edge */
DRV_EXTI_STATUS drv_exti_init(GPIO_TypeDef* gpio_port, uint8_t pin, DRV_EXTI_EDGE edge){
	// Safety check: validate pin is within range (0-15)

	// Enable SYSCFG clock

	// Associate the corresponding EXTI line for the corresponding gpio_port

	// Set the RTSR/ FTSR bit for the desired edge

	// Clear EXTI Pending Register (EXTI_PR) bit

	// Set the EXTI Interrupt Mask Register (EXTI_IMR) bit to unmask the line
}

/* Configures NVIC for the EXTI Interrupt*/
DRV_EXTI_STATUS drv_exti_enable(IRQn_Type irqn, uint8_t priority){
	// Safety check: validate irqn is a non-negative external interrupt number (not a core exception)

	// Safety check: validate priority is within the implemented range (0-15)

	// Set the Interrupt priority in Interrupt Priority Register (NVIC_IPRx)

	// Set the Interrupt Set-Enable Register (NVIC_ISERx) to enable the Interrupt to happen
}

/* Clears the pending bit for the EXTI line */
DRV_EXTI_STATUS drv_exti_clear_pending(uint8_t pin){
	// Safety check: validate pin is within range (0-15)

	// Clear EXTI Pending Register (EXTI_PR) bit

}

/* Checks whether a given EXTI line's pending flag is currently set */
DRV_EXTI_STATUS drv_exti_is_pending(uint8_t pin, uint8_t* is_pending){
	// Safety check: validate pin is within range (0-15)

	// Safety check: validate is_pending is not NULL

	// Check if the corresponding EXTI Pending Register (EXTI_PR) is set to 1

}
