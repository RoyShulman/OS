#include <kernel/utils.h>

#include "IO.h"

#include "screen.h"

#define CELL_SIZE 	(2)

void print_char(char character, int col, int row, char attribute_byte) {
	uint8_t* vidmem = (uint8_t*) VIDEO_ADDRESS;

	if (vidmem == NULL) {
		return;
	}

	if (!attribute_byte) {
		attribute_byte = WHITE_ON_BLACK;
	}

	// Get the video memory offset for the screen location
	int offset; 
	// If col and row are nonegative use them for offset
	if (col >= 0 && row >= 0) {
		offset = get_screen_offset(col, row);
	} 
	// Otherwise use the current cursor position
	else {
		offset = get_cursor();
	}

	// If we see a new line set offset to the end of current row
	// so it will be advanced to the first col of the next row
	if (character == '\n') {
		int rows = offset / (2 * MAX_COLS);
		offset = get_screen_offset(MAX_COLS - 1, rows);
	}
	// Otherwise write the char and the style to the calculated offset
	else {
		vidmem[offset] = character;
		vidmem[offset + 1] = attribute_byte;
	}

	// Update the offset to the next character cell which is 2 bytes ahead of our current cell
	int next_offset = 2;
	offset += next_offset;

	// Adjust the scrolling when we reach the bottom of the screen
	offset = handle_scrolling(offset);

	// Update the cursor position
	set_cursor(offset);
}

int get_cursor() {
	// The screen device uses the control register to know what query we want to perform 
	// and sets the data in the data register. 14 - cursor high byte, 15 - cursor low byte
	
	uint8_t cursor_high_byte_query = 14;
	uint8_t cursor_low_byte_query = 15;
	
	port_byte_out(REG_SCREEN_CTRL, cursor_high_byte_query);
	uint8_t cursor_high_byte = port_byte_in(REG_SCREEN_DATA) ;

	port_byte_out(REG_SCREEN_CTRL, cursor_low_byte_query);
	uint8_t cursor_low_byte = port_byte_in(REG_SCREEN_DATA);

	int offset = (cursor_high_byte << 8) + cursor_low_byte;

	// The cursor offset we got from the VGA hardware is the number of characters we
	// need to multiply it by 2 to convert it to a cell offset(each cell has the char and the style)
	offset *= CELL_SIZE;

	return offset;
}

void set_cursor(const int cell_offset) {
	// Because each cell in the VGA is of size 2 we need to devide our offset by 2
	// to convert to char offset from cell offset
	
	const int char_offset = cell_offset / CELL_SIZE;

	uint8_t cursor_high_byte = (char_offset >> 8) & 0xff;
	uint8_t cursor_low_byte = char_offset & 0xff;

	uint8_t cursor_high_byte_query = 14;
	uint8_t cursor_low_byte_query = 15;

	port_byte_out(REG_SCREEN_CTRL, cursor_high_byte_query);
	port_byte_out(REG_SCREEN_DATA, cursor_high_byte);

	port_byte_out(REG_SCREEN_CTRL, cursor_low_byte_query);
	port_byte_out(REG_SCREEN_DATA, cursor_low_byte);
}

void print_at(const char* message, const int col, const int row) {
	// Update cursor location if col and row aren't negative
	if (col >= 0 && row >= 0) {
		set_cursor(get_screen_offset(col, row));
	}

	// Loop through the string and print it
	uint32_t i = 0;
	while (message[i] != 0) {
		print_char(message[i], col, row, WHITE_ON_BLACK);
		++i;
	}
}

void print(const char* message) {
	print_at(message, -1, -1);
}

void clear_screen() {
	// Loop through the screen and set it's byte to ' '
	for (uint32_t row = 0; row < MAX_ROWS; row++) {
		for (uint32_t col = 0; col < MAX_COLS; col++) {
			print_char(' ', col, row, WHITE_ON_BLACK);
		}
	}
	// Set the cursor back at the top left
	set_cursor(get_screen_offset(0, 0));
}

int get_screen_offset(const int col, const int row) {
	// To calculate we use ((row * MAX_COL) + col) * CELL_SIZE
	return (((row * MAX_COLS) + col) * CELL_SIZE);
}

int handle_scrolling(const int curser_offset) {
	int vga_buffer_size = MAX_ROWS * MAX_COLS * CELL_SIZE;
	// If offset is inside the vga, return the offset unmodified
	if (curser_offset < vga_buffer_size) {
		return curser_offset;
	}

	// Shift rows back one, starting from index 1
	for (int i = 1; i < MAX_ROWS; i++) {
		memcpy((char*) (get_screen_offset(0, i) + VIDEO_ADDRESS),
					(char*) (get_screen_offset(0, i -1) + VIDEO_ADDRESS),
					MAX_COLS * CELL_SIZE);
		// Blank the last line
	}
	char* last_line = (char*) (get_screen_offset(0, MAX_ROWS - 1) + VIDEO_ADDRESS);
	for (int i = 0; i < MAX_COLS * CELL_SIZE; i++) {
		last_line[i] = 0;
	}
	// Set the curser to be on the last row, rather than the edge of the screen
	int new_curser_offset = curser_offset - 2 * MAX_COLS;

	return new_curser_offset;
}

void print_itoa(int num) {
	char num_ascii[256];
	itoa(num, num_ascii);
	print(num_ascii);
}