[BITS 32]

extern kernel_main
extern __bss_start
extern __bss_end

global _start
_start:
    ; We're already in protected mode with stack set up
    
    ; Initialize BSS section (zero out uninitialized global variables)
    mov edi, __bss_start
    mov ecx, __bss_end
    sub ecx, edi
    xor eax, eax
    cld
    rep stosb
    
    ; Call the C kernel
    call kernel_main
    
    ; If kernel returns, halt
halt_loop:
    hlt
    jmp halt_loop