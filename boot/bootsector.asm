; Bootsector that prints a message to the screen using BIOS routine

; Because the BIOS maps our bootsector to 0x7c00 we tell the assembler we will be mapped there so he
; corrects the offsets
[org 0x7c00]

mov ah, 0x0e ; int 0x10 with ah = 0x0e -> means scrolling teletype

mov bx, my_string
loop:
	cmp [bx], dword 0
	je endloop

	mov al, [bx]
	int 0x10
	add bx, word 1

	jne loop

endloop:

jmp $

my_string:
	db 'Hello there!',0

;$$ is the start of the current position
;$ is the address of current position
;$-$$ is how far from the start we are. We need to fill bytes until byte 511 and 512
times 510-($-$$) db 0
; Little endian
dw 0xaa55