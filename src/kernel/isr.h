#ifndef ISR_H
#define ISR_H

#include "types.h"

/* ISRs reserved for CPU exceptions */
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

/* IRQ definitions */
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

/* Struct which aggregates many registers */
typedef struct {
   uint32_t ds; /* Data segment selector */
   uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; /* Pushed by pusha. */
   uint32_t int_no, err_code; /* Interrupt number and error code (if applicable) */
   uint32_t eip, cs, eflags, useresp, ss; /* Pushed by the processor automatically */
} registers_t;

/**
 * @brief      map all the isrs and irq to their index in the idt and load the dt
 */
void isr_install();

/**
 * @brief      Interrupt service routine handler - handles all the interrupts
 *
 * @param[in]  r     The registers pushed with the interrupt
 */
void isr_handler(registers_t r);

// Pointer to a functiont for a modular way to handler interrupt IRQs
typedef void (*isr_handler_ptr_t)(registers_t); 	

/**
 * @brief      Register a function to handler a specific interrupt IRQ
 *
 * @param[in]  int_num  The interrupt number
 * @param[in]  handler  Pointer to a function to call to handle the interrupt
 */
void register_interrupt_handler(uint8_t int_num, isr_handler_ptr_t handler);

/**
 * @brief      Unregisters the interrupt handler
 *
 * @param[in]  int_num  The interrupt number
 */
void unregister_interrupt_handler(uint8_t int_num);

#define MASTER_PIC_COMMAND		(0x20)
#define MASTER_PIC_DATA			(0x21)
#define SLAVE_PIC_COMMAND		(0xa0)
#define SLAVE_PIC_DATA			(0xa1)
#define PIC_EOI					(0x20)

/**
 * @brief      Remaps the IRQs to ISRs 32-47
 */
void remap_irq();

/**
 * @brief      Basic handler for irq interrupts
 *
 * @param[in]  r     Registers pushed
 */
void irq_handler(registers_t r);
#endif