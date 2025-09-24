; Multiboot entry point
[BITS 32]

section .text
global _start
extern kernel_main

_start:
    ; Set up stack
    mov esp, stack_top
    
    ; Call kernel main
    call kernel_main
    
    ; Halt if kernel returns
    cli
.hang:
    hlt
    jmp .hang

section .bss
align 16
stack_bottom:
    resb 16384  ; 16KB stack
stack_top: