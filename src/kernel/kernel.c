#include <drivers/screen.h>
#include <drivers/timer.h>
#include <drivers/keyboard.h>

#include "mem.h"
#include "isr.h"
#include "paging.h"

void kmain() {

	clear_screen();
	isr_install();
	__asm__ __volatile__ ("sti");

	init_timer(100);
	init_keyboard();
	initialise_paging();
}