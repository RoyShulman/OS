#include <drivers/screen.h>
#include <drivers/timer.h>
#include <drivers/keyboard.h>

#include "utils.h"
#include "isr.h"

void main() {

	clear_screen();
	isr_install();
	__asm__ __volatile__ ("sti");


	init_timer(100);
	init_keyboard();
}