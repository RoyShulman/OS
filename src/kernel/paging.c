#include <drivers/screen.h>

#include "mem.h"
#include "isr.h"

#include "paging.h"

#define BITS_IN_BYTE		(8)
#define PTR_32_BIT_SIZE		((BITS_IN_BYTE * sizeof(uint32_t*)))
#define PTR_32_MAX_SIZE		(0xfffffff) // 2**32 - 1
#define MAX_PAGE_ADDR		(0x1000000)
#define PAGE_ALIGN_SIZE		(0x1000)
#define PAGE_FAULT_INT_NUM	(14)

// uint32_t nframes; // The number of frames

/*
 * This is a bitset where each bit defines if the frame address is free or used.
 * we use 2 functions to access members of the bitset.
 */

bool* frames;

// // static void set_frame(uint32_t frame_addr) {
// // 	uint32_t frame = frame_addr / PAGE_ALIGN_SIZE; // Frame are alligned to PAGE_ALIGN_SIZE
// // 	frames[index] |= true; // Set used bit in offset in index
// // }

// // static void clear_frame(uint32_t frame_addr) {
// // 	uint32_t frame = frame_addr / PAGE_ALIGN_SIZE; // Frame are alligned to PAGE_ALIGN_SIZE
// // 	frames[index] = false; // clear used bit in offset in index
// // }

// // static uint32_t is_frame_used(uint32_t frame_addr) { 
// // 	uint32_t frame = frame_addr / PAGE_ALIGN_SIZE; // Frame are alligned to PAGE_ALIGN_SIZE
// // 	return (frames[index] == true); 
// // }

// // static uint32_t first_free_frame() { 
// // 	for (unsigned int i = 0; i < nframes; i++) {
// 		// if (!frames[i]) { 
// 		// 	// Found a free frame!
// 		// 	return i; // returns an address of a frame / PAGE_ALIGN_SIZE
// 		// }
// // 	}
// // 	return NULL; // No free frames left!
// // }

// *
//  * @brief      Allocates a frame and connects it to a page
//  *
//  * @param      page       The page to allocate a frame to
//  * @param[in]  privilege  The privilege level of the page
//  * @param[in]  writeable  Is the frame writeable
//  *
//  * @return     { description_of_the_return_value }
 
// // static int alloc_frame(page_t* page, uint32_t privilege, uint32_t writeable) {
// // 	if (page == NULL) {
// // 		return -1;
// // 	}
// // 	if (page->frame) {
// // 		// Page is already allocated, return
// // 		return 1;
// // 	}
// // 	else {
// // 		uint32_t free_frame = first_free_frame();
// // 		if (free_frame == NULL) {
// // 			// No free frames!
// // 			return -1;
// // 		}
// // 		set_frame(free_frame * PAGE_ALIGN_SIZE);// Set a used page in the bitset
// // 		page->frame = free_frame; 				// Addr of frame
// // 		page->present = 1;						// Page is preset(it's ours!)
// // 		page->user = (privilege)?0:1;			// 0 for kernel mode, 1 for user mode
// // 		page->rw = (writeable)?1:0;				// 1 for read-write, 0 for read only
// // 	}
// // }

// /**
//  * @brief      Frees a frame
//  *
//  * @param      page  The page that holds information about that frame
//  */
// // static void free_frame(page_t* page) {
// // 	if (page == NULL) {
// // 		return;
// // 	}
// // 	uint32_t frame = page->frame;
// // 	if (page->frame == NULL) {
// // 		// Frame is already free!
// // 		return;
// // 	}
// // 	clear_frame(frame * PAGE_ALIGN_SIZE);	// Clear the page from the bitset
// // 	page->frame = NULL;
// // }

int initialise_paging() {
	uint32_t nframes = MAX_PAGE_ADDR / PAGE_ALIGN_SIZE; // We have 4096 frames
	frames = (bool*) kmalloc_a(nframes); // Allocate memory for the frame bitset
	if (frames == NULL) {
		return -1;
	}
	memset(frames, 0, nframes);

	page_directory_t* kernel_directory = (page_directory_t*) kmalloc_a(sizeof(page_directory_t)); 
	memset(kernel_directory, 0, sizeof(page_directory_t));

	// for (int j = 0; j < 1024; j++) {
	// 	page_table_t* page_table = (page_table_t*)kmalloc_a(sizeof(page_table_t));
	// 	memset(page_table, 0, sizeof(page_table_t));

	// 	for (int i = 0; i < 1024; i++) {
	// 		page_table->pages[i].present = 1;
	// 		page_table->pages[i].rw = 1;
	// 		page_table->pages[i].frame = i + j*1024;
	// 	}
	// 	kernel_directory->tables[j] = page_table;
	// 	kernel_directory->tablesPhysical[j] = (page_table->pages[j].frame * PAGE_ALIGN_SIZE) | 3;
	// }
	kernel_directory->physicalAddr = (uint32_t)&(kernel_directory->tablesPhysical);

	register_page_fault_handler();

	switch_page_directory(kernel_directory);

	return 0;
}

void switch_page_directory(page_directory_t* page_directory) {
	// Set cr3 to the address that points to the physical addresses of pages
	__asm__ __volatile__("mov %0, %%cr3":: "r"(&(page_directory->tablesPhysical[0])));

	uint32_t cr0;
	__asm__ __volatile__("mov %%cr0, %0": "=r"(cr0));
   	cr0 |= 0x80000000; // Enable paging!
   	__asm__ __volatile__("mov %0, %%cr0":: "r"(cr0));
}

void register_page_fault_handler() {
	register_interrupt_handler(PAGE_FAULT_INT_NUM, page_fault_handler);
}

void destroy_page_fault_handler() {
	unregister_interrupt_handler(PAGE_FAULT_INT_NUM);
}

void page_fault_handler(registers_t regs) {
	print("Page fault hit!: ");
	uint32_t fault_address;
	__asm__ __volatile__("mov %%cr2, %0": "=r"(fault_address));

   int present   = !(regs.err_code & 0x1); // Page not present
   int rw = regs.err_code & 0x2;           // Write operation?
   int us = regs.err_code & 0x4;           // Processor was in user-mode?
   int reserved = regs.err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
   int id = regs.err_code & 0x10;          // Caused by an instruction fetch?

   if(present) { 
   	print("present ");
   }
   if(rw) {
   	print("rw ");
   }
   if(reserved) {
   	print("reserved ");
   }
   if (id) {
   	print("id ");
   }
   if (us) {
   	print("user ");
   }
   print("\n");

}