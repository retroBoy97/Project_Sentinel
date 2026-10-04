#include "drv_exti.h"
#include "drv_gpio.h"

#define DRV_EXTI_LINE_MAX 15U
#define DRV_EXTI_PRIORITY_MAX 15U

static DRV_EXTI_STATUS drv_set_exti_line(GPIO_TypeDef* gpio_port, uint32_t* control_register, uint8_t offset){
	if (gpio_port == GPIOA){
		*control_register &= ~(1U << offset);
		*control_register &= ~(1U << (offset + 1));
		*control_register &= ~(1U << (offset + 2));
		*control_register &= ~(1U << (offset + 3));
	}
	else if (gpio_port == GPIOB){
		*control_register |= (1U << offset);
		*control_register &= ~(1U << (offset + 1));
		*control_register &= ~(1U << (offset + 2));
		*control_register &= ~(1U << (offset + 3));
	}
	else if (gpio_port == GPIOC){
		*control_register &= ~(1U << offset);
		*control_register |= (1U << (offset + 1));
		*control_register &= ~(1U << (offset + 2));
		*control_register &= ~(1U << (offset + 3));
	}
	else if (gpio_port == GPIOD){
		*control_register |= (1U << offset);
		*control_register |= (1U << (offset + 1));
		*control_register &= ~(1U << (offset + 2));
		*control_register &= ~(1U << (offset + 3));
	}
	else if (gpio_port == GPIOE){
		*control_register &= ~(1U << offset);
		*control_register &= ~(1U << (offset + 1));
		*control_register |= (1U << (offset + 2));
		*control_register &= ~(1U << (offset + 3));
	}
	else if (gpio_port == GPIOH){
		*control_register |= (1U << offset);
		*control_register |= (1U << (offset + 1));
		*control_register |= (1U << (offset + 2));
		*control_register &= ~(1U << (offset + 3));
	}
	else{
		return DRV_EXTI_ERROR;
	}

	return DRV_EXTI_OK;
}

/* Configures a GPIO pin as an EXTI interrupt source for the given edge */
DRV_EXTI_STATUS drv_exti_init(GPIO_TypeDef* gpio_port, uint8_t pin, DRV_EXTI_EDGE edge){
	// Safety check: validate pin is within range (0-15)
	if (pin > DRV_EXTI_LINE_MAX){
		return DRV_EXTI_ERROR;
	}

	// Enable SYSCFG clock
	RCC -> APB2ENR |= (1U << RCC_APB2ENR_SYSCFGEN_Pos);

	// Associate the corresponding EXTI line for the corresponding gpio_port
	uint32_t* control_register = &SYSCFG -> EXTICR[pin / 4]; // The used control register from SYSCFG
	uint8_t offset = (pin % 4) * 4U; // EXTI line offset in that register
	if (drv_set_exti_line(gpio_port, control_register, offset) != DRV_EXTI_OK){
		return DRV_EXTI_ERROR;
	}

	// Set the Rising Trigger Selection Register (EXTI_RTSR)
	// or Falling Trigger Selection Register (EXTI_FTSR) bit
	// to get the desired edge
	if (edge == DRV_EXTI_RISING_EDGE|| edge == DRV_EXTI_BOTH_EDGES){
		EXTI -> RTSR |= (1U << pin);
	}
	if (edge == DRV_EXTI_FALLING_EDGE|| edge == DRV_EXTI_BOTH_EDGES){
		EXTI -> FTSR |= (1U << pin);
	}

	// Clear EXTI Pending Register (EXTI_PR) bit
	EXTI -> PR = (1U << pin);

	// Set the EXTI Interrupt Mask Register (EXTI_IMR) bit to unmask the line
	EXTI -> IMR |= 1U << pin;

	return DRV_EXTI_OK;
}

/* Configures NVIC for the EXTI Interrupt */
DRV_EXTI_STATUS drv_exti_enable(IRQn_Type irqn, uint8_t priority){
	// Safety check: validate irqn is a non-negative external interrupt number (not a core exception)
	if (irqn < 0){
		return DRV_EXTI_ERROR;
	}

	// Safety check: validate priority is within the implemented range (0-15)
	if (priority > DRV_EXTI_PRIORITY_MAX){
		return DRV_EXTI_ERROR;
	}

	// Set the Interrupt priority in Interrupt Priority Register (NVIC_IPRx)

	// Set the Interrupt Set-Enable Register (NVIC_ISERx) to enable the Interrupt to happen
}

/* Clears the pending bit for the EXTI line */
DRV_EXTI_STATUS drv_exti_clear_pending(uint8_t pin){
	// Safety check: validate pin is within range (0-15)
	if (pin > DRV_EXTI_LINE_MAX){
		return DRV_EXTI_ERROR;
	}

	// Clear EXTI Pending Register (EXTI_PR) bit
	EXTI -> PR = (1U << pin);

	return DRV_EXTI_OK;
}

/* Checks whether a given EXTI line's pending flag is currently set */
DRV_EXTI_STATUS drv_exti_is_pending(uint8_t pin, uint8_t* is_pending){
	// Safety check: validate pin is within range (0-15)
	if (pin > DRV_EXTI_LINE_MAX){
		return DRV_EXTI_ERROR;
	}

	// Safety check: validate is_pending is not NULL
	if (is_pending == NULL){
		return DRV_EXTI_ERROR;
	}

	// Check if the corresponding EXTI Pending Register (EXTI_PR) is set to 1
	if (EXTI -> PR & (1U << pin)){
		*is_pending = 1U;
	}
	else{
		*is_pending = 0U;
	}

	return DRV_EXTI_OK;
}
