#ifndef IDT_H
#define IDT_H

#include <stdint.h>

#define GDT_KERNEL_CODE_SEG (0x8)

typedef struct {
    uint16_t low_offset;  // offset bits 0-15 of entry point of ISR
    uint16_t selector;    // code segment descriptor in gdt
    uint8_t zero;         // unused
    uint8_t attributes;    
    /*
     * Bit 7:       Interrupt present
     * Bits 6-5:    Privilege level of caller
     * bit 4:       Set to 0 for interrupt gate
     * Bit 3-0:     1110 - 32 bit interrupt gate
     */
    uint16_t high_offset;  // offset bits 16-31 of entry point of ISR
} __attribute__((packed)) idt_gate_t;

// Pointer to the array of interrupt descriptors
// Load with lidt
typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_register_t;

#define IDT_ENTRIES (256)
idt_register_t idt_reg;
idt_gate_t idt[IDT_ENTRIES];

/**
 * @brief      Sets the idt
 */
void set_idt();

/**
 * @brief      Sets the idt descriptor.
 *
 * @param[in]  index     The index of interrupt
 * @param[in]  offset    The offset of the isr
 */
void set_idt_gate(int num, uint32_t offset);

#endif