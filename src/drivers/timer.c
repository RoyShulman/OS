#include "IO.h"
#include "screen.h"
#include "../kernel/isr.h"
#include "../kernel/utils.h"

#include "timer.h"

// 32 bit tick counter
unsigned int tick = 0;

/**
 * @brief      Interrupt handler for timer interrupt
 *
 * @param[in]  r     How the stack looks when the timer interrupt is called
 */
static void timer_callback(registers_t r) {
	UNUSED(r);
	print("Got tick: ");
	char tick_ascii[256];
	atoi(tick, tick_ascii);
	print(tick_ascii);
	print("\n");
}

void set_frequency(int frequency) {
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

void destroy_timer() {
	unregister_interrupt_handler(IRQ0);
}