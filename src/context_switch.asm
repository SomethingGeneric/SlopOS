; Context switching assembly code
[BITS 32]

section .text
global context_switch

; void context_switch(cpu_context_t* old_context, cpu_context_t* new_context)
context_switch:
    ; Get parameters
    mov eax, [esp+4]    ; old_context
    mov edx, [esp+8]    ; new_context
    
    ; Save current context to old_context
    ; Save eax last since we're using it
    mov [eax+4],  ebx   ; ebx
    mov [eax+8],  ecx   ; ecx  
    mov [eax+16], esi   ; esi
    mov [eax+20], edi   ; edi
    mov [eax+24], esp   ; esp
    mov [eax+28], ebp   ; ebp
    
    ; Save eip (return address)
    mov ebx, [esp]
    mov [eax+32], ebx   ; eip
    
    ; Save eflags
    pushfd
    pop ebx
    mov [eax+36], ebx   ; eflags
    
    ; Save segment registers
    mov bx, ds  
    mov [eax+44], ebx   ; ds
    
    ; Save edx and eax
    mov ebx, [esp+8]    ; get new_context parameter
    mov [eax+12], ebx   ; save as edx in context
    mov ebx, [esp+4]    ; get old_context parameter  
    mov [eax+0], ebx    ; save as eax in context
    
    ; Load new context from new_context (edx points to it)
    mov eax, edx        ; eax = new_context
    
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