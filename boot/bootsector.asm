; Bootsector that prints a message to the screen using BIOS routine

[bits 16]
; Because the BIOS maps our bootsector to 0x7c00 we tell the assembler we will be mapped there so he
; corrects the offsets
[org 0x7c00]

mov [BOOT_DRIVE], dl ; The boot drive index is stored in dl and it's usually 0x80
mov bp, 0x8000
mov sp, bp

; call get_drive_parameters

; Load 5 sectors into ES:BX (0x0000 + 0x9000) from the boot disk
mov bx, 0x9000
mov dh, 5
mov dl, [BOOT_DRIVE]
call disk_load

mov dx, [0x9000]
call print_hex

mov dx, [0x9000 + 512]
call print_hex

jmp $

%include "print_string.asm"
%include "print_hex.asm"
%include "read_disk.asm"

BOOT_DRIVE:
	db 0
;$$ is the start of the current position
;$ is the address of current position
;$-$$ is how far from the start we are. We need to fill bytes until byte 511 and 512
times 510-($-$$) db 0
; Little endian
dw 0xaa55

times 256 dw 0xdada
times 256 dw 0xface