#include "idt.h"
#include "utils.h"
#include "../drivers/screen.h"
#include "../drivers/IO.h"

#include "isr.h"

isr_handler_ptr_t interrupt_handlers[256];


/* Can't do this with a loop because we need the address
 * of the function names */
void isr_install() {

    set_idt_gate(0, (unsigned int)isr0);
    set_idt_gate(1, (unsigned int)isr1);
    set_idt_gate(2, (unsigned int)isr2);
    set_idt_gate(3, (unsigned int)isr3);
    set_idt_gate(4, (unsigned int)isr4);
    set_idt_gate(5, (unsigned int)isr5);
    set_idt_gate(6, (unsigned int)isr6);
    set_idt_gate(7, (unsigned int)isr7);
    set_idt_gate(8, (unsigned int)isr8);
    set_idt_gate(9, (unsigned int)isr9);
    set_idt_gate(10, (unsigned int)isr10);
    set_idt_gate(11, (unsigned int)isr11);
    set_idt_gate(12, (unsigned int)isr12);
    set_idt_gate(13, (unsigned int)isr13);
    set_idt_gate(14, (unsigned int)isr14);
    set_idt_gate(15, (unsigned int)isr15);
    set_idt_gate(16, (unsigned int)isr16);
    set_idt_gate(17, (unsigned int)isr17);
    set_idt_gate(18, (unsigned int)isr18);
    set_idt_gate(19, (unsigned int)isr19);
    set_idt_gate(20, (unsigned int)isr20);
    set_idt_gate(21, (unsigned int)isr21);
    set_idt_gate(22, (unsigned int)isr22);
    set_idt_gate(23, (unsigned int)isr23);
    set_idt_gate(24, (unsigned int)isr24);
    set_idt_gate(25, (unsigned int)isr25);
    set_idt_gate(26, (unsigned int)isr26);
    set_idt_gate(27, (unsigned int)isr27);
    set_idt_gate(28, (unsigned int)isr28);
    set_idt_gate(29, (unsigned int)isr29);
    set_idt_gate(30, (unsigned int)isr30);
    set_idt_gate(31, (unsigned int)isr31);

    remap_irq();

     // Install the IRQs
    set_idt_gate(32, (unsigned int)irq0);
    set_idt_gate(33, (unsigned int)irq1);
    set_idt_gate(34, (unsigned int)irq2);
    set_idt_gate(35, (unsigned int)irq3);
    set_idt_gate(36, (unsigned int)irq4);
    set_idt_gate(37, (unsigned int)irq5);
    set_idt_gate(38, (unsigned int)irq6);
    set_idt_gate(39, (unsigned int)irq7);
    set_idt_gate(40, (unsigned int)irq8);
    set_idt_gate(41, (unsigned int)irq9);
    set_idt_gate(42, (unsigned int)irq10);
    set_idt_gate(43, (unsigned int)irq11);
    set_idt_gate(44, (unsigned int)irq12);
    set_idt_gate(45, (unsigned int)irq13);
    set_idt_gate(46, (unsigned int)irq14);
    set_idt_gate(47, (unsigned int)irq15);

    set_idt(); // Load with ASM
}

/* To print the message which defines every exception */
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



void isr_handler(registers_t r) {
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

void register_interrupt_handler(unsigned char int_num, isr_handler_ptr_t handler) {
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