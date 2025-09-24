[BITS 16]
[ORG 0x7C00]

; Boot sector for SlopOS
; Bootloader that loads and jumps to C++ kernel

start:
    ; Set up segments
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    sti

    ; Print loading message
    mov si, loading_msg
    call print_string

    ; Load kernel from sectors 2+ to 0x1000
    mov ah, 0x02        ; BIOS read sectors
    mov al, 8           ; Number of sectors to read
    mov ch, 0           ; Cylinder 0
    mov cl, 2           ; Start from sector 2
    mov dh, 0           ; Head 0
    mov dl, 0x80        ; Drive 0
    mov bx, 0x1000      ; Load kernel at 0x1000
    int 0x13
    jc disk_error

    ; Enable A20 line (simple method)
    in al, 0x92
    or al, 2
    out 0x92, al

    ; Load GDT
    lgdt [gdt_descriptor]

    ; Enter protected mode
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; Far jump to protected mode code
    jmp CODE_SEG:protected_mode

disk_error:
    mov si, disk_error_msg
    call print_string
    cli
    hlt

print_string:
    mov ah, 0x0E
.loop:
    lodsb
    cmp al, 0
    je .done
    int 0x10
    jmp .loop
.done:
    ret

[BITS 32]
protected_mode:
    ; Set up data segments
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    
    ; Set up stack
    mov esp, 0x90000
    
    ; Jump to C++ kernel
    call 0x1000

    ; Should never reach here
halt_loop:
    hlt
    jmp halt_loop

; GDT
gdt_start:
    dq 0                        ; Null descriptor

gdt_code:
    dw 0xFFFF                   ; Limit
    dw 0                        ; Base low
    db 0                        ; Base middle
    db 10011010b                ; Access: present, ring 0, code, executable, readable
    db 11001111b                ; Flags: 4KB granularity, 32-bit
    db 0                        ; Base high

gdt_data:
    dw 0xFFFF                   ; Limit
    dw 0                        ; Base low
    db 0                        ; Base middle
    db 10010010b                ; Access: present, ring 0, data, writable
    db 11001111b                ; Flags: 4KB granularity, 32-bit
    db 0                        ; Base high

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; Size
    dd gdt_start                ; Offset

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

; Messages
loading_msg db 'Loading C++ kernel...', 0x0D, 0x0A, 0
disk_error_msg db 'Disk error!', 0x0D, 0x0A, 0

; Fill the rest of the boot sector with zeros and add boot signature
times 510-($-$$) db 0
dw 0xAA55