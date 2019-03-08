#ifndef UTILS_H
#define UTILS_H

/**
 * Utils definitions and functions
 */

// To surpress unused veriable error
#define UNUSED(x) (void)(x)

#define NULL 	(0)

/**
 * @brief      Helper functiont to copy num_bytes from one buffer to the other
 *
 * @param[in]      	source     The source buffer to copy from
 * @param[out]      dest       The destination buffer to copy to
 * @param[in]  		num_bytes  The number of bytes to copy
 */
void copy_memory(char* source, char* dest, int num_bytes);

/**
 * @brief      Convert an int to ascii
 *
 * @param[in]  		num   The int to convert
 * @param[out]      str   The buffer to write the ascii of the int to
 */
void itoa(int num, char* str);

/**
 * @brief     Reverse a string
 *
 * @param[inout]      string  The string to reverse
 */
void reverse(char* string);

/**
 * @brief      Calculate the length of a null terminated string
 *
 * @param[inout]      string  The string
 *
 * @return     len of the string
 */
int strlen(char* string);

#endif
