#include "paging.h"

#define PTR_32_BIT_SIZE		(32)
#define PTR_32_MAX_SIZE		(0xfffffff) // 2**32 - 1

uint32_t nframes; // The number of frames

/*
 * This is a bitset where each bit defines if the frame address if's free or used.
 * Because it's 32 bit and holds 32 bit pointers, it can hold 1024 pages
 */

uint32_t* frames;

/*
 *	This macro is to find the index of the frame address in 
 *	the bitset of free/used address
 */
#define ADDR_TO_INDEX_BIT(x)		(x/32) 

/*
 * This macro is to find the offset in the index, for the 
 * given address. Use this after we found the index
 * to find the offset in the index
 */
#define ADDR_TO_OFFSET_BIT(x)		(x%32) // This 

static void set_frame(uint32_t frame_addr) {
	uint32_t frame = frame_addr / 0x1000; // Frame are alligned to 0x1000
	uint32_t index = ADDR_TO_INDEX_BIT(frame);
	uint32_t offset = ADDR_TO_OFFSET_BIT(frame);
	frames[index] |= (0x1 << offset); // Set used bit in offset in index
}

static void clear_frame(uint32_t frame_addr) {
	uint32_t frame = frame_addr / 0x1000; // Frame are alligned to 0x1000
	uint32_t index = ADDR_TO_INDEX_BIT(frame);
	uint32_t offset = ADDR_TO_OFFSET_BIT(frame);
	frames[index] &= ~(0x1 << offset); // clear used bit in offset in index
}

static void is_frame_used(uint32_t frame_addr) { 
	uint32_t frame = frame_addr / 0x1000; // Frame are alligned to 0x1000
	uint32_t index = ADDR_TO_INDEX_BIT(frame);
	uint32_t offset = ADDR_TO_OFFSET_BIT(frame);
	return (frames[index] &= ~(0x1 << offset)); 
}

static uint32_t first_free_frame() { 
	for (int i = 0; i < ADDR_TO_INDEX_BIT(nframes); i++) {
		// This iterates on all the indexes since nframes holds the
		// highest frame address / 0x1000 
		
		if (frames[i] == PTR_32_MAX_SIZE) { // 2**32-1
			// This means in this index all frames are used
			continue;
		}
		for (int j = 0; j < PTR_32_BIT_SIZE; j++) {
			uint32_t offset_to_test = frames[i] & (1 << j);
			if (!offset_to_test) { 
				// Found a free frame!
				return ((i * PTR_32_BIT_SIZE) + j) // returns an address of a frame / 0x1000
			}
		}				   
	}
	return NULL; // No free frames left!
}