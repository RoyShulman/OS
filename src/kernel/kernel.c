#include <drivers/screen.h>
#include <drivers/timer.h>
#include <drivers/keyboard.h>

#include "mem.h"
#include "isr.h"
#include "types.h"
#include "string.h"

void kmain() {

	clear_screen();
	isr_install();
	__asm__ __volatile__ ("sti");

	init_timer(100);
	init_keyboard();
	uint32_t tmp =  kmalloc_a(0x1000);
	char a[0x1000];
	itoa(tmp, a);
	print(a);
}