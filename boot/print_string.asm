; bx is the register that holds the address of the string
print_string:
    pusha
    mov ah, 0x0e ; int 0x10 with ah = 0x0e -> means scrolling teletype

loop:
    cmp [bx], byte 0
    je endloop

    mov al, [bx]
    int 0x10
    add bx, byte 1
    jmp loop

endloop:
    popa
    ret
