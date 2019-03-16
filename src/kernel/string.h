#ifndef STRING_H
#define STRING_H


/**
 * @brief      Convert an int to ascii
 *
 * @param[in]  num   The int to convert
 * @param[out] str   The buffer to write the ascii of the int to
 *
 * @return     Returns 0 on success, -1 on error
 */
int itoa(int num, char* str);

/**
 * @brief        Reverse a string
 *
 * @param[inout] string  The string to reverse
 *
 * @return       Returns 0 on success, -1 on error
 */
int reverse(char* string);

/**
 * @brief        Calculate the length of a null terminated string
 *
 * @param[inout] string  The string
 *
 * @return       len of the string
 */
int strlen(char* string);

#endif