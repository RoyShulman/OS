#include <drivers/screen.h>
#include <drivers/IO.h>

#include "idt.h"
#include "utils.h"

#include "isr.h"

#define create_idt_gate(x)     set_idt_gate(x, (uint32_t)isr(x))

isr_handler_ptr_t interrupt_handlers[256];


/* Can't do this with a loop because we need the address
 * of the function names */
void isr_install() {

    set_idt_gate(0, (uint32_t)isr0);
    set_idt_gate(1, (uint32_t)isr1);
    set_idt_gate(2, (uint32_t)isr2);
    set_idt_gate(3, (uint32_t)isr3);
    set_idt_gate(4, (uint32_t)isr4);
    set_idt_gate(5, (uint32_t)isr5);
    set_idt_gate(6, (uint32_t)isr6);
    set_idt_gate(7, (uint32_t)isr7);
    set_idt_gate(8, (uint32_t)isr8);
    set_idt_gate(9, (uint32_t)isr9);
    set_idt_gate(10, (uint32_t)isr10);
    set_idt_gate(11, (uint32_t)isr11);
    set_idt_gate(12, (uint32_t)isr12);
    set_idt_gate(13, (uint32_t)isr13);
    set_idt_gate(14, (uint32_t)isr14);
    set_idt_gate(15, (uint32_t)isr15);
    set_idt_gate(16, (uint32_t)isr16);
    set_idt_gate(17, (uint32_t)isr17);
    set_idt_gate(18, (uint32_t)isr18);
    set_idt_gate(19, (uint32_t)isr19);
    set_idt_gate(20, (uint32_t)isr20);
    set_idt_gate(21, (uint32_t)isr21);
    set_idt_gate(22, (uint32_t)isr22);
    set_idt_gate(23, (uint32_t)isr23);
    set_idt_gate(24, (uint32_t)isr24);
    set_idt_gate(25, (uint32_t)isr25);
    set_idt_gate(26, (uint32_t)isr26);
    set_idt_gate(27, (uint32_t)isr27);
    set_idt_gate(28, (uint32_t)isr28);
    set_idt_gate(29, (uint32_t)isr29);
    set_idt_gate(30, (uint32_t)isr30);
    set_idt_gate(31, (uint32_t)isr31);

    remap_irq();

     // Install the IRQs
    set_idt_gate(32, (uint32_t)irq0);
    set_idt_gate(33, (uint32_t)irq1);
    set_idt_gate(34, (uint32_t)irq2);
    set_idt_gate(35, (uint32_t)irq3);
    set_idt_gate(36, (uint32_t)irq4);
    set_idt_gate(37, (uint32_t)irq5);
    set_idt_gate(38, (uint32_t)irq6);
    set_idt_gate(39, (uint32_t)irq7);
    set_idt_gate(40, (uint32_t)irq8);
    set_idt_gate(41, (uint32_t)irq9);
    set_idt_gate(42, (uint32_t)irq10);
    set_idt_gate(43, (uint32_t)irq11);
    set_idt_gate(44, (uint32_t)irq12);
    set_idt_gate(45, (uint32_t)irq13);
    set_idt_gate(46, (uint32_t)irq14);
    set_idt_gate(47, (uint32_t)irq15);

    set_idt(); // Load with ASM
}

/* To print the message which defines every exception */
// TODO: why is this empty during run time 


void isr_handler(registers_t r) {
    const char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
    };


    print("received interrupt: ");
    char s[3];
    itoa(r.int_no, s);
    print(s);
    print("\n");
    print(exception_messages[r.int_no]);
    print("\n");
}
    
void remap_irq() {
    port_byte_out(MASTER_PIC_COMMAND, 0x11);    // Write Init to master
    port_byte_out(SLAVE_PIC_COMMAND, 0x11);     // Write init to slave

    port_byte_out(MASTER_PIC_DATA, 0x20);       // remap master pic to 0x20
    port_byte_out(SLAVE_PIC_DATA, 0x28);        // remap slave pic to 0x28 

    port_byte_out(MASTER_PIC_DATA, 0x04);       // IRQ2 -> connection to slave
    port_byte_out(SLAVE_PIC_DATA, 0x02);

    port_byte_out(MASTER_PIC_DATA, 0x01);       // Write init4 to the master pic
    port_byte_out(SLAVE_PIC_DATA, 0x01);        // write init4 to the slave pic

    port_byte_out(MASTER_PIC_DATA, 0x0);        // enable all IRQs on master PIC
    port_byte_out(SLAVE_PIC_DATA, 0x0);         // enable all IRQs on slave PIC
}

void register_interrupt_handler(uint8_t int_num, isr_handler_ptr_t handler) {
    interrupt_handlers[int_num] = handler;
}

void irq_handler(registers_t r) {
    /* We need to send PIC end of interrupt after we finish dealing with
     * the current one or else we won't get anymore
    */
    if (r.int_no >= 40) {
        // If the interrupt came from slave, we must write to it as well
        port_byte_out(SLAVE_PIC_COMMAND, PIC_EOI);
    } 
    port_byte_out(MASTER_PIC_COMMAND, PIC_EOI);
    // handle the interrupt in a moduler way
    if (interrupt_handlers[r.int_no] != 0) {
        isr_handler_ptr_t handler = interrupt_handlers[r.int_no];
        handler(r);
    }
}

void unregister_interrupt_handler(uint8_t int_num) {
    interrupt_handlers[int_num] = NULL;
}