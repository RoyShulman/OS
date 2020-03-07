; Defined in isr.c
[extern isr_handler]
[extern irq_handler]

%macro  create_isr_handler  1
   
isr%1:     
        cli
        push byte %1
        jmp isr_common_stub

%endmacro

%macro  create_isr_handler_dummy_error_code 1

isr%1:
        cli
        push byte 0
        push byte %1
        jmp isr_common_stub

%endmacro

%macro  create_irq_handler  1

irq%1:
    cli
    push byte %1
    push byte (32 + %1)
    jmp irq_common_stub

%endmacro

; Maybe I went a little too far with the macros...
%macro loop_create_irq_handlers 1

%assign i %1
%rep    i+1

    create_irq_handler i

%assign i i-1
%endrep


%endmacro


; Common ISR code
isr_common_stub:
    ; 1. Save CPU state
	pusha ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax
	mov ax, ds ; Lower 16-bits of eax = ds.
	push eax ; save the data segment descriptor
	mov ax, 0x10  ; kernel data segment descriptor
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
    ; 2. Call C handler
	call isr_handler
	
    ; 3. Restore state
	pop eax 
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	popa
	add esp, 8 ; Cleans up the pushed error code and pushed ISR number
	sti
	iret ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP
	
; Common IRQ code. Identical to ISR code except for the 'call' 
; and the 'pop ebx'
irq_common_stub:
    pusha 
    mov ax, ds
    push eax
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    call irq_handler ; Different than the ISR code
    pop ebx  ; Different than the ISR code
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
    popa
    add esp, 8
    sti
    iret 

; We don't get information about which interrupt was caller
; when the handler is run, so we will need to have a different handler
; for every interrupt.
; Furthermore, some interrupts push an error code onto the stack but others
; don't, so we will push a dummy error code for those which don't, so that
; we have a consistent stack for all of them.

; First make the ISRs global
global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31

;IRQS
global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15

; 0: Divide By Zero Exception
create_isr_handler_dummy_error_code 0

; 1: Debug Exception
create_isr_handler_dummy_error_code 1

; 2: Non Maskable Interrupt Exception
create_isr_handler_dummy_error_code 2

; 3: Int 3 Exception
create_isr_handler_dummy_error_code 3

; 4: INTO Exception
create_isr_handler_dummy_error_code 4

; 5: Out of Bounds Exception
create_isr_handler_dummy_error_code 5

; 6: Invalid Opcode Exception
create_isr_handler_dummy_error_code 6

; 7: Coprocessor Not Available Exception
create_isr_handler_dummy_error_code 7

; 8: Double Fault Exception (With Error Code!)
create_isr_handler 8

; 9: Coprocessor Segment Overrun Exception
create_isr_handler_dummy_error_code 9

; 10: Bad TSS Exception (With Error Code!)
create_isr_handler 10

; 11: Segment Not Present Exception (With Error Code!)
create_isr_handler 11

; 12: Stack Fault Exception (With Error Code!)
create_isr_handler 12

; 13: General Protection Fault Exception (With Error Code!)
create_isr_handler 13

; 14: Page Fault Exception (With Error Code!)
create_isr_handler 14

; 15: Reserved Exception
create_isr_handler_dummy_error_code 15

; 16: Floating Point Exception
create_isr_handler_dummy_error_code 16

; 17: Alignment Check Exception
create_isr_handler_dummy_error_code 17

; 18: Machine Check Exception
create_isr_handler_dummy_error_code 18

; 19: Reserved
create_isr_handler_dummy_error_code 19

; 20: Reserved
create_isr_handler_dummy_error_code 20

; 21: Reserved
create_isr_handler_dummy_error_code 21

; 22: Reserved
create_isr_handler_dummy_error_code 22

; 23: Reserved
create_isr_handler_dummy_error_code 23

; 24: Reserved
create_isr_handler_dummy_error_code 24

; 25: Reserved
create_isr_handler_dummy_error_code 25

; 26: Reserved
create_isr_handler_dummy_error_code 26

; 27: Reserved
create_isr_handler_dummy_error_code 27

; 28: Reserved
create_isr_handler_dummy_error_code 28

; 29: Reserved
create_isr_handler_dummy_error_code 29

; 30: Reserved
create_isr_handler_dummy_error_code 30

; 31: Reserved
create_isr_handler_dummy_error_code 31

; IRQ handlers
; create_irq_handler 0

; create_irq_handler 1

; create_irq_handler 2

; create_irq_handler 3

; create_irq_handler 4

; create_irq_handler 5

; create_irq_handler 6

; create_irq_handler 7

; create_irq_handler 8

; create_irq_handler 9

; create_irq_handler 10

; create_irq_handler 11

; create_irq_handler 12

; create_irq_handler 13

; create_irq_handler 14

; create_irq_handler 15

loop_create_irq_handlers 15