; Use dx as the parameter value to print
print_hex:
	pusha
	mov bx, 5 ; start after 0x
print_hex_loop:
	mov al, dl
	and al, 0xf 
	add al, '0'
	cmp al, '9'
	jbe less_then_a
	add al, 7
less_then_a:
	mov [HEX_OUT + bx], al
	dec bx
	cmp bx, 1 ; check if we filled all 4 bytes
	je end_print_hex
	ror dx, 4
	jmp print_hex_loop

end_print_hex:
	mov bx, HEX_OUT
	call print_string
	popa
	ret

HEX_OUT:
	db '0x0000', 0