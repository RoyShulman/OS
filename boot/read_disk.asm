; load DH num sectors to ES:BX from drive DL
disk_load:
	push dx ; So we can later see how many sectors were requested to read
	mov ah, 0x02 ; BIOS read sector function

	mov ch, 0x00 ; Select cylinder 0
	mov dh, 0x00 ; Select head 0(i.e. the first part of the floppy disk)
	mov cl, 0x02 ; Start reading from the second sector(After the boot sector - each sector is 512 bytes) - 
			  ; Note second since this is base 1
	mov al, dh ; read DH num sectors from the start point

	int 0x13 ; Do the actual read
	xor dx, dx
	mov dh, ah
	call print_hex
	jc disk_error ; If an error occurs the bios will set the carry flag

	pop dx
	cmp al, dh ; The bios writes to al the numbers of sectors read
	jne disk_error
	ret

disk_error:
	mov bx, DISK_ERROR_MESSAGE
	call print_string
	jmp $

DISK_ERROR_MESSAGE:
	db "Disk read message!", 0