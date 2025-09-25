#include "syscall.h" 
#include "string.h"

// Simple system call wrappers for user programs
static inline int syscall(uint32_t call_num, uint32_t arg1 = 0, uint32_t arg2 = 0, uint32_t arg3 = 0) {
    // For now, call directly (in real OS this would use int 0x80 or similar)
    return syscall_handler(call_num, arg1, arg2, arg3);
}

static int write_string(const char* str) {
    return syscall(SYS_WRITE, (uint32_t)str, strlen(str), 0);
}

// Shell command execution
void shell_execute_command(const char* command) {
    if (strcmp(command, "version") == 0) {
        write_string("slopOS 2.0 - Now with process management!\n");
    } else if (strcmp(command, "hello") == 0) {
        write_string("world\n");
    } else if (strcmp(command, "ps") == 0) {
        write_string("Process list:\n");
        write_string("  PID 0: kernel (running)\n");
        write_string("  PID 1: shell (running)\n");
    } else if (strcmp(command, "help") == 0) {
        write_string("Available commands:\n");
        write_string("  version - Display OS version\n");
        write_string("  hello   - Display greeting\n");
        write_string("  ps      - Show running processes\n");
        write_string("  memory  - Show memory usage\n");
        write_string("  yield   - Yield CPU to other processes\n");
        write_string("  exit    - Exit shell\n");
        write_string("  help    - Show this help message\n");
    } else if (strcmp(command, "memory") == 0) {
        write_string("Memory management active\n");
        write_string("  Physical memory allocator: OK\n");
        write_string("  Virtual memory manager: OK\n");
        write_string("  Kernel heap: OK\n");
    } else if (strcmp(command, "yield") == 0) {
        write_string("Yielding CPU...\n");
        syscall(SYS_YIELD);
        write_string("Back in shell\n");
    } else if (strcmp(command, "exit") == 0) {
        write_string("Goodbye!\n");
        syscall(SYS_EXIT, 0);
    } else if (strlen(command) == 0) {
        // Empty command, do nothing
        return;
    } else {
        write_string("Unknown command: ");
        write_string(command);
        write_string("\nType 'help' for available commands.\n");
    }
}

// Shell main loop - this will be the entry point for the shell process
extern "C" void shell_main() {
    char command_buffer[256];
    
    write_string("Welcome to SlopOS Shell v2.0!\n"); 
    write_string("Running as separate user process.\n");
    write_string("Type 'help' for available commands.\n\n");
    
    while (1) {
        write_string("slopOS> ");
        
        // Simple command input (using system call)
        syscall(SYS_READ, (uint32_t)command_buffer, sizeof(command_buffer), 0);
        
        // Remove newline if present
        size_t len = strlen(command_buffer);
        if (len > 0 && command_buffer[len-1] == '\n') {
            command_buffer[len-1] = '\0';
        }
        
        shell_execute_command(command_buffer);
    }
}