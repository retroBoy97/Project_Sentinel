#ifndef DRV_UART_H
#define DRV_UART_H

#include "stm32f4xx.h"

typedef enum{
	DRV_UART_MODE_TX_ONLY,
	DRV_UART_MODE_RX_ONLY,
	DRV_UART_MODE_FULL_DUPLEX,
	DRV_UART_MODE_HALF_DUPLEX
}DRV_UART_MODE;

typedef enum{
	DRV_UART_OK,
	DRV_UART_ERROR
}DRV_UART_STATUS;

/* Configures the USART peripheral for the given mode and baud rate (8N1 frame format, hardcoded) */
DRV_UART_STATUS drv_uart_init(USART_TypeDef* usart_peripheral, DRV_UART_MODE mode, uint32_t baud_rate);
/* Sends data_size bytes from data over usart_peripheral, blocking until transmission completes */
DRV_UART_STATUS drv_uart_transmit(USART_TypeDef* usart_peripheral, uint8_t* data, uint16_t data_size);
/* Reads data_size bytes into data from usart_peripheral, blocking until reception completes */
DRV_UART_STATUS drv_uart_receive(USART_TypeDef* usart_peripheral, uint8_t* data, uint16_t data_size);
/* Configures NVIC for the USART RX-not-empty interrupt */
DRV_UART_STATUS drv_uart_enable_rx_interrupt(USART_TypeDef* usart_peripheral, IRQn_Type irqn, uint8_t priority);
/* Reads a single byte already received into usart_peripheral's data register (for use inside the RX ISR) */
DRV_UART_STATUS drv_uart_read_byte(USART_TypeDef* usart_peripheral, uint8_t* byte);
/* Computes the BRR register value (16x oversampling) for the given peripheral clock speed and baud rate */
DRV_UART_STATUS compute_BRR_value(uint32_t clock_speed, uint32_t baud_rate, uint16_t* BRR_value);

#endif /* DRV_UART_H */
