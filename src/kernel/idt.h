#ifndef IDT_H
#define IDT_H

#define GDT_KERNEL_CODE_SEG (0x8)

typedef struct {
    unsigned short low_offset;  // offset bits 0-15 of entry point of ISR
    unsigned short selector;    // code segment descriptor in gdt
    unsigned char zero;         // unused
    unsigned char attributes;    
    /*
     * Bit 7:       Interrupt present
     * Bits 6-5:    Privilege level of caller
     * bit 4:       Set to 0 for interrupt gate
     * Bit 3-0:     1110 - 32 bit interrupt gate
     */
    unsigned short high_offset;  // offset bits 16-31 of entry point of ISR
} __attribute__((packed)) idt_gate_t;

// Pointer to the array of interrupt descriptors
// Load with lidt
typedef struct {
    unsigned short limit;
    unsigned int base;
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
void set_idt_gate(int num, unsigned int offset);

#endif