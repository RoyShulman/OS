loop:
	jmp loop

;$$ is the start of the current position
;$ is the address of current position
;$-$$ is how far from the start we are. We need to fill bytes until byte 511 and 512
times 510-($-$$) db 0
; Little endian
dw 0xaa55