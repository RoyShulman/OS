; Bootsector that prints a message to the screen using BIOS routine

[bits 16]
; Because the BIOS maps our bootsector to 0x7c00 we tell the assembler we will be mapped there so he
; corrects the offsets
[org 0x7c00]

mov bp, 0x9000	; move the stack
mov sp, bp

mov bx, MSG_REAL_MODE
call print_string

call switch_to_pm ; Never return

jmp $

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

	jmp $

MSG_REAL_MODE	db "Started in 16 bit real mode", 0
MSG_PROT_MODE	db "Successfully landed in 32 bit protected code", 0

;$$ is the start of the current position
;$ is the address of current position
;$-$$ is how far from the start we are. We need to fill bytes until byte 511 and 512
times 510-($-$$) db 0
; Little endian
dw 0xaa55
