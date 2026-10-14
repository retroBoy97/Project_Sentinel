#include "drv_uart.h"
#include "stm32f4xx_hal.h"

#define DRV_UART_PRIORITY_MAX 15U

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

	// Set the USART Enable bit in Control Register 1 (CR1)
	usart_peripheral -> CR1 |= USART_CR1_UE;

	return DRV_UART_OK;
}

/* Sends data_size bytes from data over usart_peripheral, blocking until transmission completes */
DRV_UART_STATUS drv_uart_transmit(USART_TypeDef* usart_peripheral, uint8_t* data, uint16_t data_size){
	// Safety check: validate usart_peripheral and data are not NULL, and there is something to send
	if (usart_peripheral == NULL || data == NULL || data_size == 0U){
		return DRV_UART_ERROR;
	}

	/* Add a timeout that returns DRV_UART_ERROR instead of hanging the calling task. */

	// Loop over data
	for (uint16_t i = 0U; i < data_size; i++){
		// Wait until Transmit Data Register (TDR) is empty
		while (!(usart_peripheral -> SR & USART_SR_TXE)){}

		// Write current byte into Data Register (DR)
		usart_peripheral -> DR = data[i];
	}

	// Wait until Transmission is complete
	while (!(usart_peripheral -> SR & USART_SR_TC)){}

	return DRV_UART_OK;
}

/* Reads data_size bytes into data from usart_peripheral, blocking until reception completes */
DRV_UART_STATUS drv_uart_receive(USART_TypeDef* usart_peripheral, uint8_t* data, uint16_t data_size){
	// Safety check: validate usart_peripheral and data are not NULL, and there is something to receive
	if (usart_peripheral == NULL || data == NULL || data_size == 0U){
		return DRV_UART_ERROR;
	}

	// Add a timeout that returns DRV_UART_ERROR instead of hanging the calling task.

	// Loop over data
	for (uint16_t i = 0U; i < data_size; i++){
		// Wait until Receive Data Register (RDR) is not empty
		while (!(usart_peripheral -> SR & USART_SR_RXNE)){}

		// Read SR once, then DR: reading DR returns RDR's byte and clears RXNE (and ORE, since SR was read just before)
		uint32_t status = usart_peripheral -> SR;
		data[i] = (uint8_t)(usart_peripheral -> DR);

		// Overrun: a byte arrived before the previous one was read and was lost, so the received data is incomplete
		if (status & USART_SR_ORE){
			return DRV_UART_ERROR;
		}
	}

	return DRV_UART_OK;
}

/* Configures NVIC for the USART RX-not-empty interrupt */
DRV_UART_STATUS drv_uart_enable_rx_interrupt(USART_TypeDef* usart_peripheral, IRQn_Type irqn, uint8_t priority){
	if (usart_peripheral == NULL || irqn < 0 || priority > DRV_UART_PRIORITY_MAX){
		return DRV_UART_ERROR;
	}

	// Peripheral side: set the RXNE Interrupt Enable bit in Control Register 1 (CR1)
	// so the USART raises its interrupt line when RXNE (or ORE) is set
	usart_peripheral -> CR1 |= USART_CR1_RXNEIE;

	// Set the Interrupt priority in NVIC Interrupt Priority Register (NVIC_IPRx)
	volatile uint8_t* priority_register = &NVIC -> IP[(uint8_t)irqn];
	*priority_register = (uint8_t)(priority << (8U - __NVIC_PRIO_BITS));

	// Set the Interrupt Set-Enable Register (NVIC_ISERx) to let the interrupt reach the CPU
	volatile uint32_t* interrupt_set_enable_register = &NVIC -> ISER[(uint8_t)irqn / 32];
	uint8_t offset = (uint8_t)irqn % 32;
	*interrupt_set_enable_register |= (1U << offset);

	return DRV_UART_OK;
}

/* Reads a single byte already received into usart_peripheral's data register (for use inside the RX ISR) */
DRV_UART_STATUS drv_uart_read_byte(USART_TypeDef* usart_peripheral, uint8_t* byte){
	// Safety check: validate usart_peripheral and byte are not NULL
	if (usart_peripheral == NULL || byte == NULL){
		return DRV_UART_ERROR;
	}

	// Read SR once: the copy is used for both checks below, since reading DR clears RXNE and ORE in the real register
	uint32_t status = usart_peripheral -> SR;

	// The USART interrupt line is shared by several sources: if RXNE is not set, no new byte is waiting in RDR
	if (!(status & USART_SR_RXNE)){
		return DRV_UART_ERROR;
	}

	// Read DR: returns RDR's byte and clears RXNE (and ORE, since SR was read just before)
	*byte = (uint8_t)(usart_peripheral -> DR);

	// Overrun: a byte arrived before the previous one was read and was lost
	if (status & USART_SR_ORE){
		return DRV_UART_ERROR;
	}

	return DRV_UART_OK;
}

/* Computes the BRR register value (16x oversampling) for the given peripheral clock speed and baud rate */
DRV_UART_STATUS compute_BRR_value(uint32_t clock_speed, uint32_t baud_rate, uint16_t* BRR_value){
	if (clock_speed == 0U || baud_rate == 0U || BRR_value == NULL){
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
