#include <kernel/types.h>
#include <kernel/isr.h>
#include <kernel/mem.h>

#include "IO.h"
#include "screen.h"

#include "timer.h"

#define CLOCK_FREQUENCY (1193180) 	//Hz units
#define TIMER_COMMAND_PORT (0x43)
#define TIMER_CHANNEL_PORT (0x40)

// 32 bit tick counter
uint32_t tick = 0;

/**
 * @brief      Interrupt handler for timer interrupt
 *
 * @param[in]  r     How the stack looks when the timer interrupt is called
 */
static void timer_callback(registers_t r) {
	UNUSED(r);

	tick++;
	if (tick % 100 == 0) {
		print("Got tick: ");
		print_itoa(tick);
		print("\n");
	}
}

void set_frequency(int frequency) {
	/* Timer command port is an 8 bits that we use as:
	 * Bits 6-7: 00 	- Channel 0
	 * Bits 4-5: 11 	- Access mode lobyte/hibyte
	 * Bits 1-3: 011 	- Square wave generator
	 * Bit 0   : 0		- 16 bit binary
	 */
	port_byte_out(TIMER_COMMAND_PORT, 0x36); 
	int divisor = CLOCK_FREQUENCY / frequency;
	port_byte_out(TIMER_CHANNEL_PORT, divisor & 0xff); 		// Low bits
	port_byte_out(TIMER_CHANNEL_PORT, divisor >> 8);		// High bits
}

void init_timer(const int frequency) {
	// Register the handler
	register_interrupt_handler(IRQ0, timer_callback);

	// Set the desired frequency
	set_frequency(frequency);
}

void destroy_timer() {
	unregister_interrupt_handler(IRQ0);
}