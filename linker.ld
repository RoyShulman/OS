ENTRY(_boot)

SECTIONS {
	. = 0x500;			/* Setup the stack */
	_stack_start = .;
	. = 0x7c00;
	_stack_end = .;
	.bootloader : 		/* The bios maps the first 512 bytes to 0x7c00 */
	{
		*(.first-boot-sector)
		_second_bootsector_start_addr = .;
		*(.boot)
 	    *(.text .text.*)
        *(.rodata .rodata.*)
        *(.data .data.*)
        *(.bss .bss.*)
        *(.got)
        . = ALIGN(512);
        _second_bootsector_end_addr = .;
	}
	/* Page Tables */
	__page_table_start = 0x100000;
	. = ALIGN(0x1000);
	_pdpt = .; /* Page directory pointer table */
	. += 0x1000;
	_pdt = .;  /* Page directory table */
	. += 0x1000;
	_pt = .; /* Page table */
	. += 0x1000;
	__page_table_end = .;
	. += 0x1000;
	_kernel_start = .;


}