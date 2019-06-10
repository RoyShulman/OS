ENTRY(_boot)

SECTIONS {
	. = 0x500;			/* Setup the stack */
	_stack_start = .;
	. = 0x7c00;
	_stack_end = .;
	/* The bios maps the first 512 bytes to 0x7c00 */
	.first-boot-sector : ALIGN(512){
		*(.first-boot-sector)
	}
	ASSERT ( (SIZEOF(.first-boot-sector) < 513), "first boot sector overflow!")
	__page_table_start = 0x100000;

	.page_tables : ALIGN(0x1000) {
			/* Page Tables */
		_pdpt = .; /* Page directory pointer table */
		. += 0x1000;
		_pdt = .;  /* Page directory table */
		. += 0x1000;
		_pt = .; /* Page table */
		. += 0x1000;
		__page_table_end = .;

	}
	. += 0x1000;
	_kernel_start = .;


}