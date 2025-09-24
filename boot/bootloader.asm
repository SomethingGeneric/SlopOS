[BITS 16]
[ORG 0x7C00]

; Boot sector for SlopOS
; Minimal bootloader with embedded kernel functionality

start:
    ; Set up segments
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    sti

    ; Print welcome message
    mov si, welcome_msg
    call print_string
    
    ; Initialize boot time
    mov ah, 0x00
    int 0x1A
    mov [boot_time_high], dx
    mov [boot_time_low], cx
    
shell_loop:
    ; Print prompt
    mov si, prompt_msg
    call print_string
    
    ; Read command
    call read_line
    
    ; Process command
    call process_command
    
    ; Loop back
    jmp shell_loop

read_line:
    mov di, input_buffer
    mov cx, 0
    
.read_char:
    mov ah, 0x00
    int 0x16
    
    cmp al, 0x0D
    je .done
    
    cmp al, 0x08
    je .backspace
    
    cmp cx, 30
    jae .read_char
    
    mov ah, 0x0E
    int 0x10
    
    stosb
    inc cx
    jmp .read_char
    
.backspace:
    cmp cx, 0
    je .read_char
    
    mov ah, 0x0E
    mov al, 0x08
    int 0x10
    mov al, ' '
    int 0x10
    mov al, 0x08
    int 0x10
    
    dec di
    dec cx
    jmp .read_char
    
.done:
    mov al, 0
    stosb
    mov si, newline
    call print_string
    ret

process_command:
    mov si, input_buffer
    
    ; Check for "uptime"
    mov di, cmd_uptime
    call compare_strings
    cmp ax, 1
    je .handle_uptime
    
    ; Check for "version"
    mov si, input_buffer
    mov di, cmd_version
    call compare_strings
    cmp ax, 1
    je .handle_version
    
    ; Check for empty
    mov si, input_buffer
    cmp byte [si], 0
    je .done
    
    mov si, unknown_cmd_msg
    call print_string
    jmp .done
    
.handle_uptime:
    call show_uptime
    jmp .done
    
.handle_version:
    mov si, version_msg
    call print_string
    
.done:
    ret

show_uptime:
    mov ah, 0x00
    int 0x1A
    
    sub cx, [boot_time_low]
    
    mov ax, cx
    mov bx, 18
    xor dx, dx
    div bx
    
    mov si, uptime_msg
    call print_string
    
    call print_number
    
    mov si, seconds_msg
    call print_string
    ret

print_number:
    mov bx, 10
    mov cx, 0
    
    cmp ax, 0
    jne .convert
    mov ah, 0x0E
    mov al, '0'
    int 0x10
    ret
    
.convert:
    cmp ax, 0
    je .print_digits
    
    xor dx, dx
    div bx
    push dx
    inc cx
    jmp .convert
    
.print_digits:
    cmp cx, 0
    je .done
    
    pop dx
    add dl, '0'
    mov ah, 0x0E
    mov al, dl
    int 0x10
    dec cx
    jmp .print_digits
    
.done:
    ret

compare_strings:
    push si
    push di
    
.loop:
    mov al, [si]
    mov bl, [di]
    cmp al, bl
    jne .not_equal
    
    cmp al, 0
    je .equal
    
    inc si
    inc di
    jmp .loop
    
.equal:
    mov ax, 1
    jmp .done
    
.not_equal:
    mov ax, 0
    
.done:
    pop di
    pop si
    ret

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

; Data
welcome_msg db 'Welcome to SlopOS!', 0x0D, 0x0A, 'Type: uptime, version', 0x0D, 0x0A, 0
prompt_msg db '> ', 0
newline db 0x0D, 0x0A, 0
unknown_cmd_msg db 'Unknown command', 0x0D, 0x0A, 0
uptime_msg db 'Uptime: ', 0
seconds_msg db ' seconds', 0x0D, 0x0A, 0
version_msg db 'SlopOS 1.0', 0x0D, 0x0A, 0
cmd_uptime db 'uptime', 0
cmd_version db 'version', 0

boot_time_high dw 0
boot_time_low dw 0
input_buffer times 32 db 0

; Fill the rest of the boot sector with zeros and add boot signature
times 510-($-$$) db 0
dw 0xAA55