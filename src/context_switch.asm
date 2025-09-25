; Context switching assembly code
[BITS 32]

section .text
global context_switch

; void context_switch(cpu_context_t* old_context, cpu_context_t* new_context)
context_switch:
    ; Get parameters - old_context in eax, new_context in edx
    mov eax, [esp+4]    ; old_context
    mov edx, [esp+8]    ; new_context
    
    ; Save current context to old_context if it's not NULL
    test eax, eax
    jz restore_context
    
    ; Save general purpose registers
    mov [eax+4],  ebx   ; ebx at offset 4
    mov [eax+8],  ecx   ; ecx at offset 8
    mov [eax+12], edx   ; edx at offset 12 (save original edx)
    mov [eax+16], esi   ; esi at offset 16
    mov [eax+20], edi   ; edi at offset 20
    mov [eax+24], esp   ; esp at offset 24
    mov [eax+28], ebp   ; ebp at offset 28
    
    ; Save eip (return address from the stack)
    mov ebx, [esp]
    mov [eax+32], ebx   ; eip at offset 32
    
    ; Save eflags
    pushfd
    pop ebx
    mov [eax+36], ebx   ; eflags at offset 36
    
    ; Save segment registers
    mov bx, cs
    mov [eax+40], ebx   ; cs at offset 40
    mov bx, ds
    mov [eax+44], ebx   ; ds at offset 44
    mov bx, es
    mov [eax+48], ebx   ; es at offset 48
    mov bx, fs
    mov [eax+52], ebx   ; fs at offset 52
    mov bx, gs
    mov [eax+56], ebx   ; gs at offset 56
    mov bx, ss
    mov [eax+60], ebx   ; ss at offset 60
    
    ; Save eax last (we've been using it)
    mov ebx, [esp+4]    ; get old_context parameter again
    mov [eax+0], ebx    ; save as eax value (just save something reasonable)

restore_context:
    ; Load new context from new_context (edx points to it)
    mov eax, edx        ; eax = new_context
    
    ; Restore segment registers first (except CS which is handled by far jump)
    mov bx, [eax+44]    ; ds
    mov ds, bx
    mov bx, [eax+48]    ; es
    mov es, bx
    mov bx, [eax+52]    ; fs
    mov fs, bx
    mov bx, [eax+56]    ; gs
    mov gs, bx
    mov bx, [eax+60]    ; ss
    mov ss, bx
    
    ; Restore general purpose registers
    mov ebx, [eax+4]    ; ebx
    mov ecx, [eax+8]    ; ecx
    mov esi, [eax+16]   ; esi
    mov edi, [eax+20]   ; edi
    mov esp, [eax+24]   ; esp
    mov ebp, [eax+28]   ; ebp
    
    ; Restore eflags
    push dword [eax+36] ; eflags
    popfd
    
    ; Push new eip onto stack for return
    push dword [eax+32] ; eip
    
    ; Restore edx and eax last
    mov edx, [eax+12]   ; edx
    mov eax, [eax+0]    ; eax
    
    ; Return to new eip
    ret