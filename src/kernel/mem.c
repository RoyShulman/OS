#include "types.h"

#include "mem.h"

#define HEAP_START		(0x1000)
#define PAGE_SIZE		(0x1000)
#define MAX_MEM_INDEX	(0x10000)

typedef struct {
	uint32_t start_ptr;
	uint32_t end_ptr;
	uint8_t used;
} __attribute__((packed)) heap_header_t;

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
