[BITS 16]
[ORG 0x7C00]

; Boot sector for SlopOS
; This is a minimal bootloader that prints "hello from SlopOS" directly

start:
    ; Set up segments
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    sti

    ; Print the message
    mov si, hello_msg
    call print_string

    ; Halt the system
    cli
    hlt

print_string:
    ; Print null-terminated string at SI
    mov ah, 0x0E  ; BIOS teletype function
.loop:
    lodsb         ; Load byte from SI into AL and increment SI
    cmp al, 0     ; Check for null terminator
    je .done
    int 0x10      ; Call BIOS video interrupt
    jmp .loop
.done:
    ret

hello_msg db 'hello from SlopOS', 0x0D, 0x0A, 0

; Fill the rest of the boot sector with zeros and add boot signature
times 510-($-$$) db 0
dw 0xAA55