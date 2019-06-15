[bits 16]
; load DH num sectors to ES:BX from drive DL
disk_load:
	mov [SECTORS], dh ; So we remeber how many we need to read
	mov ah, 0x02 ; BIOS read sector function

	mov ch, 0x00 ; Select cylinder 0
	mov dh, 0x00 ; Select head 0(i.e. the first part of the floppy disk)
	mov cl, 0x02 ; Start reading from the second sector(After the boot sector - each sector is 512 bytes) - 
			  ; Note second since this is base 1

next_group:
	mov di, 5 ; When reading from disk, failing is common and we are encouraged to try again
again:
	mov ah, 0x02 ; 	BIOS read sector function
	mov al, [SECTORS]
	int 0x13 ; Do the actual read
	jc maybe_retry ; If an error occurs the bios will set the carry flag

	sub [SECTORS], al ; Remaining to read because the bios read the number of read sectors into al
	jz done
	mov cl, 0x01 ; Start reading from sector 1 from now on
	xor dh, 0x01 ; Select the second head, and exit after it
	jnz next_group
	inc ch ; Next cylinder
	jmp next_group
maybe_retry:
	mov ah, 0x00 ; Reset diskdrive
	int 0x13
	dec di
	jnz again
	jmp disk_error
done:
	ret

; Print the return code of interrupt
disk_error:
	mov bx, DISK_ERROR_MESSAGE
	call print_string_16
	mov dh, ah
	call print_hex
	jmp $

DISK_ERROR_MESSAGE:
	db "Disk read message! Return Code: ", 0

SECTORS			db 0