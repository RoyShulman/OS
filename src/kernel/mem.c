#include "mem.h"

static uint32_t free_ptr = 0x10000; // Make this large so we don't write over kernel code

void memcpy(char* source, char* dest, int num_bytes) {
	for (int i = 0; i < num_bytes; i++) {
		*(dest + i) = *(source + i);
	}
}

void memset(void* ptr, int value, const unsigned int num) {
	for(unsigned int i = 0; i < num; i++) {
		*(((char*) ptr) + i) = value;
	}
}

uint32_t kmalloc_a(uint32_t sz) {
	if (free_ptr & 0xFFFFF000) {
		// The address is not alligned so allign it
		free_ptr &= 0xFFFFF000;
		free_ptr += 0x1000;
	}
	uint32_t tmp = free_ptr;
	free_ptr += sz;
	return tmp;
}
