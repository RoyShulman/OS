#ifndef IO_H
#define IO_H

#include <kernel/types.h>

/**
 * Wrapper functions for hardware input output operations
 */

/**
 * @brief      A wrapper function that reads a byte from a specific port
 * 
 *
 * @param[in]  port:  The port to read from
 *
 * @return     The byte read from the specified port
 */
uint8_t port_byte_in(uint16_t port);

/**
 * @brief      Wrapper function to write a byte to a specified port
 *
 * @param[in]  port  The port to write into
 * @param[in]  data  The byte to write into the port
 */
void port_byte_out(uint16_t port, uint8_t data);

/**
 * @brief      A wrapper function that reads a word from a specific port
 *
 * @param[in]  port  The port to read from
 *
 * @return     The word read from the specific port
 */
uint16_t port_word_in(uint16_t port);

/**
 * @brief      A wrapper function to write a word to a specified port
 *
 * @param[in]  port  The port to write into
 * @param[in]  data  The data to write into the port
 */
void port_word_out(uint16_t port, uint16_t data);

#endif