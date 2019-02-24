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

; Use dx as the parameter value to print
print_hex:
	pusha
	mov bx, 2
print_hex_loop:
	ror dx, 4 ; Get the 4 lowest bits in the number 
	mov al, dl
	and al, 0xf
	add al, '0'
	cmp al, '9'
	jbe lessA
	add al, 7
lessA:
	mov [HEX_OUT + bx], al
	inc bx
	cmp bx, 6
	je end_print_hex
	jmp print_hex_loop

end_print_hex:
	mov bx, HEX_OUT
	call print_string
	popa
	ret

%include "print_string.asm"

HEX_OUT:
	db '0x0000', 0

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