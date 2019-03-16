#ifndef MEM_H
#define MEM_H

// To surpress unused warning
#define UNUSED(x) (void)(x)

#define NULL 	(0)

/**
 * @brief      Helper functiont to copy num_bytes from one buffer to the other
 *
 * @param[in]  source     The source buffer to copy from
 * @param[out] dest       The destination buffer to copy to
 * @param[in]  num_bytes  The number of bytes to copy
 *
 */
void memcpy(char* source, char* dest, int num_bytes);

/**
 * @brief      Set value num times to ptr address
 *
 * @param      ptr    The pointer to the address
 * @param[in]  value  The value to write to the address
 * @param[in]  num    The number of times to write the value
 */
void memset(void* ptr, int value, const unsigned int num);

#endif