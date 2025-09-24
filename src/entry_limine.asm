; Entry point for Limine kernel

[BITS 64]

global _start

extern kernel_main

section .text
_start:
    ; Clear the direction flag
    cld
    
    ; We are already in 64-bit mode with stack set up by Limine
    ; The stack is guaranteed to be 16-byte aligned
    
    ; Call the C kernel
    call kernel_main
    
    ; If kernel returns, halt
    cli
    hlt

; Limine requests section markers
section .requests_start_marker
    dq 0x13d86c035a1cd3e1, 0x2b0caa89d8f3026a

section .requests_end_marker
    dq 0x13d86c035a1cd3e1, 0x2b0caa89d8f3026a