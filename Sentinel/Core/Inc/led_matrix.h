#ifndef MATRIX_H
#define MATRIX_H

#include "hc595.h"
#include "hc164.h"

typedef enum{
	LED_MATRIX_OK,
	LED_MATRIX_ERROR
}LED_MATRIX_STATUS;

typedef struct{
	const Hc164* rows;
	const Hc595* columns;
}Led_Matrix;


static const Led_Matrix LED_MATRIX = {&HC164, &HC595};

// Default pattern (check mark): one byte per row, bit 7 = C1 (left), 1 = LED on
static const uint8_t OK_PATTERN[8] = {
	0x00,	// 00000000
	0x01,	// 00000001
	0x02,	// 00000010
	0x04,	// 00000100
	0x88,	// 10001000
	0x50,	// 01010000
	0x20,	// 00100000
	0x00	// 00000000
};

// Alarm pattern (cross X): one byte per row, bit 7 = C1 (left), 1 = LED on
static const uint8_t ALARM_PATTERN[8] = {
	0x81,	// 10000001
	0x42,	// 01000010
	0x24,	// 00100100
	0x18,	// 00011000
	0x18,	// 00011000
	0x24,	// 00100100
	0x42,	// 01000010
	0x81	// 10000001
};

// Blank pattern: every LED off, used to turn the matrix dark
static const uint8_t BLANK_PATTERN[8] = {
	0x00,	// 00000000
	0x00,	// 00000000
	0x00,	// 00000000
	0x00,	// 00000000
	0x00,	// 00000000
	0x00,	// 00000000
	0x00,	// 00000000
	0x00	// 00000000
};

/* Initializes both shift registers, turns every LED off and selects the default pattern */
LED_MATRIX_STATUS led_matrix_init(const Led_Matrix* led_matrix);
/* Selects the pattern to display (8 bytes, one per row): the next refresh calls show it */
LED_MATRIX_STATUS led_matrix_set_pattern(const uint8_t* pattern);
/* Displays one row of the current pattern, then moves to the next row: must be called periodically (~2 ms) */
LED_MATRIX_STATUS led_matrix_refresh(const Led_Matrix* led_matrix);














#endif /* MATRIX_H */

