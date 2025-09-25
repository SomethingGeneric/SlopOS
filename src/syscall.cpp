#include "syscall.h"
#include "process.h"
#include "terminal.h"

// Initialize system call interface
void syscall_init() {
    // For now, just a placeholder
    // In a full implementation, this would set up interrupt handlers
}

// System call handler
int syscall_handler(uint32_t call_num, uint32_t arg1, uint32_t arg2, uint32_t arg3) {
    switch (call_num) {
        case SYS_EXIT:
            return sys_exit((int)arg1);
        case SYS_WRITE:
            return sys_write((const char*)arg1, (size_t)arg2);
        case SYS_READ:
            return sys_read((char*)arg1, (size_t)arg2);
        case SYS_YIELD:
            return sys_yield();
        default:
            return -1; // Invalid system call
    }
}

// Exit current process
int sys_exit(int status) {
    process_t* current = process_get_current();
    if (current && current->pid != 0) { // Don't exit kernel process
        process_terminate(current->pid);
    }
    return status;
}

// Write to terminal
int sys_write(const char* buffer, size_t count) {
    for (size_t i = 0; i < count; i++) {
        terminal_putchar(buffer[i]);
    }
    return (int)count;
}

// Read from terminal
int sys_read(char* buffer, size_t count) {
    for (size_t i = 0; i < count - 1; i++) {
        char c = terminal_getchar();
        if (c == '\n') {
            buffer[i] = c;
            buffer[i + 1] = '\0';
            return (int)(i + 1);
        }
        buffer[i] = c;
    }
    buffer[count - 1] = '\0';
    return (int)count;
}

// Yield CPU
int sys_yield() {
    process_yield();
    return 0;
}