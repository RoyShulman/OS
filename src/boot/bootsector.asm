; Bootsector that prints a message to the screen using BIOS routine

[bits 16]
; Because the BIOS maps our bootsector to 0x7c00 we tell the assembler we will be mapped there so he
; corrects the offsets
[org 0x7c00]
KERNEL_OFFSET equ 0x1000	; The memory offset to which we load our kernel

	mov [BOOT_DRIVE], dl 	; BIOS stores the boot drive in dl

	mov bp, 0x9000			; Setup the stack
	mov sp, bp

	mov bx, MSG_REAL_MODE
	call print_string

	call load_kernel
 
	call switch_to_pm ; Never return


	jmp $

load_kernel:
	mov bx, MSG_LOAD_KERNEL
	call print_string

	mov bx, KERNEL_OFFSET 	; Load the first 15(just to be safe) sectors excluding the boot sector
	mov dh, 15				; from our boot drive into the address 0x1000 which is the KERNEL_OFFSET
	mov dl, [BOOT_DRIVE]
	call disk_load

	ret

%include "print_string.asm"
%include "print_hex.asm"
%include "read_disk.asm"
%include "gdt.asm"
%include "switch_to_pm.asm"



[bits 32]:
; Arrive here after switching and initializing pm
BEGIN_PM:
	mov ebx, MSG_PROT_MODE
	call print_string_pm

	call KERNEL_OFFSET 		; Jump to the kernel code!

	jmp $

BOOT_DRIVE		db 0
MSG_REAL_MODE	db "Started in 16 bit real mode!", 0
MSG_PROT_MODE	db "Successfully landed in 32 bit protected code!", 0
MSG_LOAD_KERNEL	db "Loading kernel into memory!", 0

;$$ is the start of the current position
;$ is the address of current position
;$-$$ is how far from the start we are. We need to fill bytes until byte 511 and 512
times 510-($-$$) db 0
; Little endian
dw 0xaa55
