#include "IO.h"
#include "screen.h"
#include "../kernel/isr.h"
#include "../kernel/utils.h"

#include "keyboard.h"

/**
 * @brief      Interrupt handler for keyboard(IRQ1)
 *
 * @param[in]  r     How the stack looks after IRQ1
 */
static void keyboard_callback(registers_t r) {
	UNUSED(r);

	unsigned char input_byte;
	input_byte = port_byte_in(KEYBOARD_PORT);

	char input_char[256];
	itoa(input_byte, input_char);
	print("Input char was: ");
	print(input_char);
	print("\n");
}

void init_keyboard() {
	register_interrupt_handler(IRQ1, keyboard_callback);
}

void destroy_keyboard() {
	unregister_interrupt_handler(IRQ1);
}