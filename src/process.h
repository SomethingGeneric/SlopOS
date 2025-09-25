#ifndef PROCESS_H
#define PROCESS_H

#include "types.h"
#include "memory.h"

#define MAX_PROCESSES 32
#define STACK_SIZE 4096

// Process states
typedef enum {
    PROCESS_READY,
    PROCESS_RUNNING,
    PROCESS_BLOCKED,
    PROCESS_TERMINATED
} process_state_t;

// CPU context for process switching
typedef struct {
    uint32_t eax, ebx, ecx, edx;
    uint32_t esi, edi, esp, ebp;
    uint32_t eip, eflags;
    uint32_t cs, ds, es, fs, gs, ss;
} cpu_context_t;

// Process Control Block
typedef struct {
    uint32_t pid;                    // Process ID
    process_state_t state;           // Process state
    cpu_context_t context;           // CPU context
    page_directory_t* page_dir;      // Virtual memory space
    uint32_t stack_base;             // Stack base address
    uint32_t stack_top;              // Current stack top
    uint32_t priority;               // Process priority
    char name[32];                   // Process name
} process_t;

// Process management functions
void process_init();
uint32_t process_create(const char* name, void (*entry_point)(), uint32_t priority);
void process_terminate(uint32_t pid);
void process_yield();
void process_schedule();
process_t* process_get_current();
process_t* process_get_by_pid(uint32_t pid);
process_t* process_get_by_index(int index); // Get process by table index

// Context switching (implemented in assembly)
extern "C" void context_switch(cpu_context_t* old_context, cpu_context_t* new_context);

#endif