#include "process.h"
#include "memory.h"
#include "string.h"

// Process table and management
static process_t process_table[MAX_PROCESSES];
static uint32_t next_pid = 1;
static int current_process = 0;
static int process_count = 0;

// Initialize process management
void process_init() {
    // Clear process table
    for (int i = 0; i < MAX_PROCESSES; i++) {
        process_table[i].pid = 0;
        process_table[i].state = PROCESS_TERMINATED;
        process_table[i].page_dir = NULL;
    }
    
    // Create kernel process (PID 0)
    process_table[0].pid = 0;
    process_table[0].state = PROCESS_RUNNING;
    process_table[0].page_dir = NULL; // Uses kernel page directory
    process_table[0].priority = 0;
    strcpy(process_table[0].name, "kernel");
    current_process = 0;
    
    // Create shell process entry (PID 1) - it will run in kernel context
    process_table[1].pid = 1;
    process_table[1].state = PROCESS_RUNNING;
    process_table[1].page_dir = NULL;
    process_table[1].priority = 1;
    strcpy(process_table[1].name, "shell");
    process_table[1].stack_base = 0; // Uses kernel stack
    process_table[1].stack_top = 0;
    
    process_count = 2;
    next_pid = 2;
}

// Create a new process
uint32_t process_create(const char* name, void (*entry_point)(), uint32_t priority) {
    // Find free slot in process table
    int slot = -1;
    for (int i = 1; i < MAX_PROCESSES; i++) {
        if (process_table[i].state == PROCESS_TERMINATED) {
            slot = i;
            break;
        }
    }
    
    if (slot == -1) {
        return 0; // No free slots
    }
    
    process_t* proc = &process_table[slot];
    
    // Initialize process
    proc->pid = next_pid++;
    proc->state = PROCESS_READY;
    proc->priority = priority;
    strcpy(proc->name, name);
    
    // Allocate page directory for process (simplified - no paging)
    proc->page_dir = NULL;
    
    // Allocate stack in kernel heap for now
    proc->stack_base = (uint32_t)kmalloc(STACK_SIZE);
    if (!proc->stack_base) {
        return 0; // Failed to allocate stack
    }
    proc->stack_top = proc->stack_base + STACK_SIZE;
    
    // Initialize CPU context
    proc->context.eip = (uint32_t)entry_point;
    proc->context.esp = proc->stack_top - 16; // Leave some space
    proc->context.ebp = proc->context.esp;
    proc->context.eflags = 0x202; // Enable interrupts
    proc->context.cs = 0x08; // Kernel code segment for now
    proc->context.ds = proc->context.es = proc->context.fs = 
    proc->context.gs = proc->context.ss = 0x10; // Kernel data segment
    
    // Clear other registers
    proc->context.eax = proc->context.ebx = proc->context.ecx = 
    proc->context.edx = proc->context.esi = proc->context.edi = 0;
    
    process_count++;
    return proc->pid;
}

// Terminate a process
void process_terminate(uint32_t pid) {
    process_t* proc = process_get_by_pid(pid);
    if (!proc || proc->state == PROCESS_TERMINATED) {
        return;
    }
    
    // Free process resources
    if (proc->stack_base) {
        kfree((void*)proc->stack_base);
    }
    
    proc->state = PROCESS_TERMINATED;
    process_count--;
    
    // If current process is terminating, schedule next one
    if (proc->pid == process_table[current_process].pid) {
        process_schedule();
    }
}

// Yield CPU to next process
void process_yield() {
    if (process_count <= 1) {
        return; // Only kernel process running
    }
    
    process_schedule();
}

// Simple round-robin scheduler
void process_schedule() {
    if (process_count <= 1) {
        return; // Only kernel process
    }
    
    process_t* current = &process_table[current_process];
    
    // Save current process state if it's still running
    if (current->state == PROCESS_RUNNING) {
        current->state = PROCESS_READY;
    }
    
    // Find next ready process
    int next = current_process;
    int attempts = 0;
    do {
        next = (next + 1) % MAX_PROCESSES;
        attempts++;
        // If we've checked all slots and found no ready process, stay with current
        if (attempts >= MAX_PROCESSES) {
            return;
        }
    } while (process_table[next].state != PROCESS_READY);
    
    // If we found a different process, switch to it
    if (next != current_process) {
        process_t* next_proc = &process_table[next];
        next_proc->state = PROCESS_RUNNING;
        
        // Skip page directory switching for now (no paging)
        
        // Perform context switch
        context_switch(&current->context, &next_proc->context);
        current_process = next;
    }
}

// Get current running process
process_t* process_get_current() {
    return &process_table[current_process];
}

// Get process by PID
process_t* process_get_by_pid(uint32_t pid) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].pid == pid && process_table[i].state != PROCESS_TERMINATED) {
            return &process_table[i];
        }
    }
    return NULL;
}

// Get process by table index
process_t* process_get_by_index(int index) {
    if (index >= 0 && index < MAX_PROCESSES && process_table[index].state != PROCESS_TERMINATED) {
        return &process_table[index];
    }
    return NULL;
}