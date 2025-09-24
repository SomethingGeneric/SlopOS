[BITS 32]

extern kernel_main

global _start
_start:
    ; We're already in protected mode with stack set up
    ; Call the C++ kernel
    call kernel_main
    
    ; If kernel returns, halt
halt_loop:
    hlt
    jmp halt_loop