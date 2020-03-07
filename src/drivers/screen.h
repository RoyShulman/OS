#ifndef SCREEN_H
#define SCREEN_H

#define VIDEO_ADDRESS 	(0xb8000)
#define MAX_ROWS 		(25)
#define MAX_COLS		(80)
#define WHITE_ON_BLACK	(0x0f)

// Screen device I/O ports
#define REG_SCREEN_CTRL	(0x3d4)
#define REG_SCREEN_DATA	(0x3d5)

/**
 * @brief      Prints a char on the screen at a specific col, row or cursor position
 *
 * @param[in]  character       The character to write
 * @param[in]  col             The col to writ ein
 * @param[in]  row             The row to write in
 * @param[in]  attribute_byte  Style to write in
 */
void print_char(char character, int col, int row, char attribute_byte);

/**
 * @brief      A static wrapper function to calculate the current position of the cursor.
 * 			   Queries the screen device I/O ports
 *
 * @return     The cursor offset in the VGA
 */
int get_cursor();

/**
 * @brief      Sets the cursor position in the VGA
 *
 * @param[in]  offset  The offset to set.
 */
void set_cursor(const int cell_offset);

/**
 * @brief      Print a string at a given col and row. If col and row both equal -1,
 * 			   print from current cursor position
 *
 * @param[in]  message  The string to print
 * @param[in]  col      The col to print at
 * @param[in]  row      The row to print in
 */
void print_at(const char* message, const int col, const int row);

/**
 * @brief      Simple function to print a string at current cursor position
 *
 * @param[in]  message  The string to print
 */
void print(const char* message);

/**
 * @brief      Helper function to clear the screen
 */
void clear_screen();

/**
 * @brief      Convert row and col to an offset in the VGA
 *
 * @param[in]  col   The col
 * @param[in]  row   The row
 *
 * @return     The screen offset in the VGA
 */
int get_screen_offset(const int col, const int row);

/**
 * @brief      Move the screen down when we reach the bottom of the screen.
 * 			   This is done by copying all rows up and having the bottom empty 
 *
 * @param[in]  offset  The offset in which we are currently at
 *
 * @return     Updated cursor position
 */
int handle_scrolling(const int curser_offset);

/**
 * @brief      Wrapper function to print an integer
 *
 * @param[in]  num   The integer to print
 */
void print_itoa(int num);

#endif