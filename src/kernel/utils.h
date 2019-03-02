#ifndef UTILS_H
#define UTILS_H

/**
 * Utils definitions and functions
 */

// To surpress unused veriable error
#define UNUSED(x) (void)(x)

/**
 * @brief      Helper functiont to copy num_bytes from one buffer to the other
 *
 * @param      source     The source buffer to copy from
 * @param      dest       The destination buffer to copy to
 * @param[in]  num_bytes  The number of bytes to copy
 */
void copy_memory(char* source, char* dest, int num_bytes);

#endif
