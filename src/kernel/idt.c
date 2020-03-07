#include "idt.h"

#define GDT_KERNEL_CODE_SEG (0x8)
#define IDT_GATE_ATTRIBUTES (0x8e)

void set_idt_gate(int index, uint32_t offset) {
	idt[index].low_offset = offset & 0xffff;
	idt[index].selector = GDT_KERNEL_CODE_SEG;
	idt[index].zero = 0;
	idt[index].attributes = IDT_GATE_ATTRIBUTES;
	idt[index].high_offset = (offset >> 16) & 0xffff;
}

void set_idt() {
	idt_reg.base = (uint32_t) &idt;
	idt_reg.limit = (IDT_ENTRIES * sizeof(idt_register_t)) - 1;
	__asm__ __volatile__("lidtl (%0)" : : "r" (&idt_reg));
}