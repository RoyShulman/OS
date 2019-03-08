#include "IO.h"
#include "screen.h"
#include "../kernel/isr.h"
#include "../kernel/utils.h"

#include "keyboard.h"

/**
 * @brief      Translate a scan code to a it's meaning
 *
 * @param[in]  		scancode     The scancode
 * @param[out]      return_char  The return character
 */
static void scancode_to_char(unsigned char scancode, char** return_char) {
	UNUSED(return_char);
	switch(scancode) {
		case 0x01:
			*return_char = "ESC";
			break;
		case 0x02:
			*return_char = "1";
			break;
		case 0x03:
			*return_char = "2";
			break;
		case 0x04:
			*return_char = "3";
			break;
		case 0x05:
			*return_char = "4";
			break;
		case 0x06:
			*return_char = "5";
			break;
		case 0x07:
			*return_char = "6";
			break;
		case 0x08:
			*return_char = "7";
			break;
		case 0x09:
			*return_char = "8";
			break;
		case 0x0a:
			*return_char = "9";
			break;
		case 0x0b:
			*return_char = "0";
			break;
		case 0x0c:
			*return_char = "-";
			break;
		case 0x0d:
			*return_char = "=";
			break;
		case 0x0e:
			*return_char = "BACKSPACE";
			break;
		case 0x0f:
			*return_char = "TAB";
			break;
		case 0x10:
			*return_char = "q";
			break;
		case 0x11:
			*return_char = "w";
			break;
		case 0x12:
			*return_char = "e";
			break;
		case 0x13:
			*return_char = "r";
			break;
		case 0x14:
			*return_char = "t";
			break;
		case 0x15:
			*return_char = "y";
			break;
		case 0x16:
			*return_char = "u";
			break;
		case 0x17:
			*return_char = "i";
			break;
		case 0x18:
			*return_char = "o";
			break;
		case 0x19:
			*return_char = "p";
			break;
		case 0x1a:
			*return_char = "[";
			break;
		case 0x1b:
			*return_char = "]";
			break;
		case 0x1c:
			*return_char = "ENTER";
			break;
		case 0x1d:
			*return_char = "LCTRL";
			break;
		case 0x1e:
			*return_char = "a";
			break;
		case 0x1f:
			*return_char = "s";
			break;
		case 0x20:
			*return_char = "d";
			break;
		case 0x21:
			*return_char = "f";
			break;
		case 0x22:
			*return_char = "g";
			break;
		case 0x23:
			*return_char = "h";
			break;
		case 0x24:
			*return_char = "j";
			break;
		case 0x25:
			*return_char = "k";
			break;
		case 0x26:
			*return_char = "l";
			break;
		case 0x27:
			*return_char = ";";
			break;
		case 0x28:
			*return_char = "'";
			break;
		case 0x29:
			*return_char = "`";
			break;
		default:
			*return_char = "?";
			break;
	}
}

/**
 * @brief      Interrupt handler for keyboard(IRQ1)
 *
 * @param[in]  r     How the stack looks after IRQ1
 */
static void keyboard_callback(registers_t r) {
	UNUSED(r);

	unsigned char scancode;
	scancode = port_byte_in(KEYBOARD_PORT);

	char* input_char = { 0 };
	scancode_to_char(scancode, &input_char);
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