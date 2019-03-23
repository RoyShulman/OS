#ifndef UTILS_H
#define UTILS_H

#include "types.h"

#define PANIC(msg)		panic(msg, __FILE__, __LINE__)

/**
 * @brief      Prints a message, and the line and file it was called from
 * 			   then halts everything
 * 
 * @param[in]    msg   The message to print
 * @param[in]    file  The file the function was called from
 * @param[in] 	 line  The line the function was called from
 */
void panic(char* msg, char* file, uint32_t line);

#endif