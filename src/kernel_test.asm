[BITS 32]

global kernel_main

kernel_main:
    ; Simple test: write directly to VGA memory
    mov edi, 0xB8000
    mov eax, 0x0F480F48  ; "HH" in white on black
    mov [edi], eax
    mov eax, 0x0F650F6C  ; "le" in white on black  
    mov [edi + 4], eax
    mov eax, 0x0F6C0F6C  ; "ll" in white on black
    mov [edi + 8], eax
    mov eax, 0x0F210F6F  ; "o!" in white on black
    mov [edi + 12], eax
    
    ; Infinite loop
spin:
    hlt
    jmp spin