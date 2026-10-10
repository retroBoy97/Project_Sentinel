#include "drv_uart.h"
#include "stm32f4xx_hal.h"

static DRV_UART_STATUS drv_uart_enable_clock(USART_TypeDef* usart_peripheral){
	if (usart_peripheral == USART1){
		// Set USART1 enable bit in RCC_APB2ENR
		RCC -> APB2ENR |= RCC_APB2ENR_USART1EN;

	}
	else if (usart_peripheral == USART2){
		// Set USART2 enable bit in RCC_APB1ENR
		RCC -> APB1ENR |= RCC_APB1ENR_USART2EN;
	}
	else if (usart_peripheral == USART6){
		// Set USART6 enable bit in RCC_APB2ENR
		RCC -> APB2ENR |= RCC_APB2ENR_USART6EN;

	}
	else {
		return DRV_UART_ERROR;
	}
	return DRV_UART_OK;
}

/* Configures the USART peripheral for the given mode and baud rate (8N1 frame format, hardcoded) */
DRV_UART_STATUS drv_uart_init(USART_TypeDef* usart_peripheral, DRV_UART_MODE mode, uint32_t baud_rate){
	// Enable usart_peripheral's clock
	if (drv_uart_enable_clock(usart_peripheral) == DRV_UART_ERROR){
		return DRV_UART_ERROR;
	}

	// Set the usart_peripheral's mode
	if (mode == DRV_UART_MODE_TX_ONLY || mode == DRV_UART_MODE_FULL_DUPLEX || mode == DRV_UART_MODE_HALF_DUPLEX){
		// Set the Transmitter Enable bit in Control Register 1 (CR1)
		usart_peripheral -> CR1 |= USART_CR1_TE;
	}
	if (mode == DRV_UART_MODE_RX_ONLY || mode == DRV_UART_MODE_FULL_DUPLEX || mode == DRV_UART_MODE_HALF_DUPLEX){
		// Set the Receiver Enable bit in Control Register 1 (CR1)
		usart_peripheral -> CR1 |= USART_CR1_RE;
	}

	/* Compute usart_peripheral Baud Rate register's value */
	// Get usart_peripheral's clock speed
	uint32_t clock_speed = 0U;
	if (usart_peripheral == USART2){
		clock_speed = HAL_RCC_GetPCLK1Freq(); // USART2 is clocked by APB1
	}
	else{
		clock_speed = HAL_RCC_GetPCLK2Freq(); // USART1 and USART6 are clocked by APB2
	}

	// Calculate the value that should be held by BRR
	uint16_t brr_value = 0U;
	if (compute_BRR_value(clock_speed, baud_rate, &brr_value) == DRV_UART_ERROR){
		return DRV_UART_ERROR;
	}

	// Set the baud rate register
	usart_peripheral -> BRR = brr_value;

	return DRV_UART_OK;
}

/* Computes the BRR register value (16x oversampling) for the given peripheral clock speed and baud rate */
DRV_UART_STATUS compute_BRR_value(uint32_t clock_speed, uint32_t baud_rate, uint16_t* BRR_value){
	if (clock_speed == 0U || baud_rate == 0U){
		return DRV_UART_ERROR;
	}

	// Safety check: validate BRR_value is not NULL
	if (BRR_value == NULL){
		return DRV_UART_ERROR;
	}

	// (Mantissa << 4) + Fraction
	uint32_t brr = (clock_speed + (baud_rate / 2U)) / baud_rate;

	// Safety check: validate the result fits in the 16-bit BRR field (fails for baud rates too low for clock_speed)
	if (brr > 0xFFFFU){
		return DRV_UART_ERROR;
	}

	*BRR_value = (uint16_t)brr;

	return DRV_UART_OK;
}
