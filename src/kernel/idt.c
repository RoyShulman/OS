#include "idt.h"

void set_idt_gate(int n, unsigned int handler) {
	unsigned short low_handler_offset = handler & 0xfff;
	unsigned short high_handler_offset = (handler >> 16) & 0xfff;

    idt[n].low_offset = low_handler_offset;
    idt[n].sel = KERNEL_CS;
    idt[n].always0 = 0;
    idt[n].flags = 0x8E; 
    idt[n].high_offset = high_handler_offset;
}

void set_idt() {
    idt_reg.base = (unsigned int) &idt;
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;
    /* Don't make the mistake of loading &idt -- always load &idt_reg */
    __asm__ __volatile__("lidtl (%0)" : : "r" (&idt_reg));
}