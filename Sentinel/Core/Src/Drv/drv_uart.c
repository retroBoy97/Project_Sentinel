#include "drv_uart.h"

static DRV_UART_STATUS drv_uart_enable_clock(USART_TypeDef* uart_peripheral){
	if (uart_peripheral == USART1){
		// Set USART1 enable bit in RCC_APB2ENR
		RCC -> APB2ENR |= RCC_APB2ENR_USART1EN;

	}
	else if (uart_peripheral == USART2){
		// Set USART2 enable bit in RCC_APB1ENR
		RCC -> APB1ENR |= RCC_APB1ENR_USART2EN;
	}
	else if (uart_peripheral == USART6){
		// Set USART6 enable bit in RCC_APB1ENR
		RCC -> APB2ENR |= RCC_APB2ENR_USART6EN;

	}
	else {
		return DRV_UART_ERROR;
	}
	return DRV_UART_OK;
}
