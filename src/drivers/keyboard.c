#include <kernel/types.h>
#include <kernel/isr.h>
#include <kernel/mem.h>

#include "IO.h"
#include "screen.h"

#include "keyboard.h"

#define KEYBOARD_PORT	(0x60)

/**
 * @brief      Translate a scan code to a it's meaning
 *
 * @param[in]  scancode     The scancode
 * @param[out] return_char  The return character
 *
 * @return     Returns 0 on sucess, -1 on error
 */
static int scancode_to_char(uint8_t scancode, char** return_char) {
	if(return_char == NULL) {
		return -1;
	}
	uint8_t press_release_difference = 0x80;
	uint8_t known_scancodes = 0x44; 	// This is also the number of cases + 1
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
		case 0x2a:
			*return_char = "LSHIFT";
			break;
		case 0x2b:
			*return_char = "\\";
			break;
		case 0x2c:
			*return_char = "z";
			break;
		case 0x2d:
			*return_char = "x";
			break;
		case 0x2e:
			*return_char = "c";
			break;
		case 0x2f:
			*return_char = "v";
			break;
		case 0x30:
			*return_char = "b";
			break;
		case 0x31:
			*return_char = "n";
			break;
		case 0x32:
			*return_char = "m";
			break;
		case 0x33:
			*return_char = ",";
			break;
		case 0x34:
			*return_char = ".";
			break;
		case 0x35:
			*return_char = "/";
			break;
		case 0x36:
			*return_char = "RSHIFT";
			break;
		case 0x37:
			*return_char = "*";
			break;
		case 0x38:
			*return_char = "LALT";
			break;
		case 0x39:
			*return_char = "SPACE";
			break;
		case 0x3a:
			*return_char = "CAPSLOCK";
			break;
		case 0x3b:
			*return_char = "F1";
			break;
		case 0x3c:
			*return_char = "F2";
			break;
		case 0x3d:
			*return_char = "F3";
			break;
		case 0x3e:
			*return_char = "F4";
			break;
		case 0x3f:
			*return_char = "F5";
			break;
		case 0x40:
			*return_char = "F6";
			break;
		case 0x41:
			*return_char = "F7";
			break;
		case 0x42:
			*return_char = "F8";
			break;
		case 0x43:
			*return_char = "F9";
			break;
		case 0x44:
			*return_char = "F10";
			break;
		default:
			if (scancode <= known_scancodes + press_release_difference) {
				scancode_to_char(scancode - press_release_difference, return_char);
				break;
			}
			*return_char = "?";
			break;
	}
	return 0;
}

/**
 * @brief      Interrupt handler for keyboard(IRQ1)
 *
 * @param[in]  r     How the stack looks after IRQ1
 */
static void keyboard_callback(registers_t r) {
	UNUSED(r);

	uint8_t scancode;
	scancode = port_byte_in(KEYBOARD_PORT);

	char* input_char = NULL;
	if (scancode_to_char(scancode, &input_char) != 0) {
		print("Could not translate input scan code to char!\n");
	}
	else {
		print("Input char was: ");
		print(input_char);
		print("\n");		
	}
}


void init_keyboard() {
	register_interrupt_handler(IRQ1, keyboard_callback);
}

void destroy_keyboard() {
	unregister_interrupt_handler(IRQ1);
}