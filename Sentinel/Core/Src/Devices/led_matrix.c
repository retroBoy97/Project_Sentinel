#include "led_matrix.h"
#include "hc595.h"
#include "hc164.h"
#include "drv_gpio.h"

// Row the next refresh will display (0-7)
static uint8_t current_row = 0U;
// Pattern being displayed: 8 bytes, one per row
static const uint8_t* current_pattern = NULL;

/* Blanks the columns: all cathodes HIGH, so no LED can light whichever rows are on */
static LED_MATRIX_STATUS led_matrix_blank_columns(const Led_Matrix* led_matrix){
	// 0xFF = all 8 columns HIGH
	HC595_STATUS shift_status = hc595_shift_byte(led_matrix -> columns, 0xFFU);
	// Copy the shifted byte to the outputs
	HC595_STATUS latch_status = hc595_latch(led_matrix -> columns);

	if (shift_status == HC595_ERROR || latch_status == HC595_ERROR){
		return LED_MATRIX_ERROR;
	}

	return LED_MATRIX_OK;
}

/* Initializes both shift registers, turns every LED off and selects the default pattern */
LED_MATRIX_STATUS led_matrix_init(const Led_Matrix* led_matrix){
	// Safety check: validate led_matrix is not NULL
	if (led_matrix == NULL){
		return LED_MATRIX_ERROR;
	}

	// Init both shift registers' gpio pins
	HC595_STATUS columns_status = hc595_init(led_matrix -> columns);
	HC164_STATUS rows_status = hc164_init(led_matrix -> rows);

	if (columns_status == HC595_ERROR || rows_status == HC164_ERROR){
		return LED_MATRIX_ERROR;
	}

	// Blank the columns first, so nothing lights while the rows are cleared
	if (led_matrix_blank_columns(led_matrix) == LED_MATRIX_ERROR){
		return LED_MATRIX_ERROR;
	}

	// Clear the rows: CLR is tied to 3.3 V, so the only way to empty the 164 is to shift 8 zeros through it
	for (uint8_t i = 0U; i < 8U; i++){
		if (hc164_shift_bit(led_matrix -> rows, DRV_GPIO_LOW) == HC164_ERROR){
			return LED_MATRIX_ERROR;
		}
	}

	// Start the scan from row 0 with the default pattern
	current_row = 0U;
	current_pattern = OK_PATTERN;

	return LED_MATRIX_OK;
}

/* Selects the pattern to display (8 bytes, one per row): the next refresh calls show it */
LED_MATRIX_STATUS led_matrix_set_pattern(const uint8_t* pattern){
	// Safety check: validate pattern is not NULL, so refresh never reads through NULL
	if (pattern == NULL){
		return LED_MATRIX_ERROR;
	}

	// Change the pattern
	current_pattern = pattern;

	return LED_MATRIX_OK;
}

/* Displays one row of the current pattern, then moves to the next row: must be called periodically (~2 ms) */
LED_MATRIX_STATUS led_matrix_refresh(const Led_Matrix* led_matrix){
	// Safety check: validate led_matrix is not NULL and a pattern is selected (init was called)
	if (led_matrix == NULL || current_pattern == NULL){
		return LED_MATRIX_ERROR;
	}

	// Blank the columns, so the next row doesn't flash the previous row's columns
	if (led_matrix_blank_columns(led_matrix) == LED_MATRIX_ERROR){
		return LED_MATRIX_ERROR;
	}

	// Move to the next row: a 1 enters at row 0 (start of a frame), then 0s push it down one row per call
	DRV_GPIO_LEVEL row_bit;
	if (current_row == 0U){
		row_bit = DRV_GPIO_HIGH;
	}
	else{
		row_bit = DRV_GPIO_LOW;
	}

	if (hc164_shift_bit(led_matrix -> rows, row_bit) == HC164_ERROR){
		return LED_MATRIX_ERROR;
	}

	// Show this row's columns: inverted because a LED lights when its column (cathode) is LOW
	// The cast keeps the low 8 bits, since ~ promotes the uint8_t to a 32-bit int
	uint8_t columns = (uint8_t)(~current_pattern[current_row]);
	HC595_STATUS shift_status = hc595_shift_byte(led_matrix -> columns, columns);
	HC595_STATUS latch_status = hc595_latch(led_matrix -> columns);

	if (shift_status == HC595_ERROR || latch_status == HC595_ERROR){
		return LED_MATRIX_ERROR;
	}

	// Next call shows the next row, back to row 0 after row 7
	current_row++;
	if (current_row == 8U){
		current_row = 0U;
	}

	return LED_MATRIX_OK;
}
