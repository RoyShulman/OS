#ifndef PAGING_H
#define PAGING_H

#include "types.h"

typedef struct page
{
   uint32_t present    : 1;   // Page present in memory
   uint32_t rw         : 1;   // Read-only if clear, readwrite if set
   uint32_t user       : 1;   // Supervisor level only if clear
   uint32_t accessed   : 1;   // Has the page been accessed since last refresh?
   uint32_t dirty      : 1;   // Has the page been written to since last refresh?
   uint32_t unused     : 7;   // Amalgamation of unused and reserved bits
   uint32_t frame      : 20;  // Frame address (shifted right 12 bits)
} __attribute__((packed)) page_t;

typedef struct page_table
{
   page_t pages[1024] __attribute__((aligned(0x1000)));
} page_table_t;

typedef struct page_directory
{
   /**
      Array of pointers to pagetables.
   **/
   page_table_t *tables[1024] __attribute__((aligned(0x1000)));
   /**
      Array of pointers to the pagetables above, but gives their *physical*
      location, for loading into the CR3 register.
   **/
   uint32_t tablesPhysical[1024] __attribute__((aligned(0x1000)));
   /**
      The physical address of tablesPhysical. This comes into play
      when we get our kernel heap allocated and the directory
      may be in a different location in virtual memory.
   **/
   uint32_t physicalAddr;
} page_directory_t;

/**
 * @brief      Sets up the environment, page directories and enables paging
 * 
 * @return     Returns 0 on success, -1 on error
 */
int initialise_paging();

/**
 * @brief      Causes the specified page directory to be loaded into the
 * 			   CR3 register.
 *
 * @param      new_dir  The page directory to be loaded
 */
void switch_page_directory(page_directory_t* new_dir);

/**
 * @brief      Handler for page fauls interrupt
 *
 * @param[in]  regs  registers describing how the stack looks like
 */
void page_fault_handler(registers_t regs);

/**
 * @brief      Registers the page fault handler
 */
void register_page_fault_handler();

/**
 * @brief      Unregisters the page fault handler  
 */
void destroy_page_fault_handler();

/**
 * @brief      Retrieves a pointer to a page for a virtual memory address
 *
 * @param[in]  address         The physical address the page has
 * @param[in]  should_alloc    If to allocate the page if it doesn't exist
 * @param      page_directory  The page directory the page resides in
 *
 * @return     The page.
 */
page_t* get_page(uint32_t address, bool should_alloc, page_directory_t* page_directory);

#endif