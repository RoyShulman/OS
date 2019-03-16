; Small file to make sure when jumping to kernel code we will jump to main
; This code will always be at the start address of where we load our kernel
[bits 32]
[extern kmain]

call kmain

jmp $
