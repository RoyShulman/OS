#include "IO.h"
#include "screen.h"
#include "../kernel/isr.h"
#include "../kernel/utils.h"

#include "timer.h"

static void timer_callback(registers_t r) {
	UNUSED(r);
	print("Got tick!\n");

}

static void set_frequency(int frequency) {
	port_byte_out(TIMER_COMMAND_PORT, 0x36); // Tell the PIC we are setting the frequency
	int divisor = CLOCK_FREQUENCY / frequency;
	port_byte_out(TIMER_CHANNEL_PORT, divisor & 0xff); 	// Low bits
	port_byte_out(TIMER_CHANNEL_PORT, divisor >> 8);		// High bits
}

void init_timer(const int frequency) {
	// Register the handler
	register_interrupt_handler(IRQ0, timer_callback);

	// Set the desired frequency
	set_frequency(frequency);

}