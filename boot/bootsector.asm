; Bootsector that prints a message to the screen using BIOS routine

; Because the BIOS maps our bootsector to 0x7c00 we tell the assembler we will be mapped there so he
; corrects the offsets
[org 0x7c00]


mov bx, my_string
call print_string

mov bx, GOODBYE
call print_string

mov dx, 0x1337
call print_hex

jmp $

%include "print_string.asm"

%include "print_hex.asm"

my_string:
	db 'Hello there!',0

GOODBYE:
	db 'Goodbye',0

;$$ is the start of the current position
;$ is the address of current position
;$-$$ is how far from the start we are. We need to fill bytes until byte 511 and 512
times 510-($-$$) db 0
; Little endian
dw 0xaa55