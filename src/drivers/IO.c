#include "IO.h"

uint8_t port_byte_in(uint16_t port) {
	/*
	 * "=a" (result) means: Put AL register in veriable result when finished
	 * "d" (port) means: load EDX with port
	 */
	uint8_t result;
	__asm__ __volatile__("in %%dx, %%al" : "=a" (result) : "d" (port));
	return result;
}

void port_byte_out(uint16_t port, uint8_t data) {
	// "a" (data) means: load EAX with data
	// "d" (port) mean: load EDX with port
	__asm__ __volatile__("out %%al, %%dx" : :"a" (data), "d" (port));
}

uint16_t port_word_in(uint16_t port) {
	/*
	 * "=a" (result) means: Put AL register in veriable result when finished
	 * "d" (port) means: load EDX with port
	 */
	uint16_t result;
	__asm__ __volatile__("in %%dx, %%al" : "=a" (result) : "d" (port));
	return result;
}

void port_word_out(uint16_t port, uint16_t data) {
	// "a" (data) means: load EAX with data
	// "d" (port) mean: load EDX with port
	__asm__ __volatile__("out %%al, %%dx" : :"a" (data), "d" (port));
}