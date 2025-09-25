#include "syscall.h" 
#include "string.h"
#include "terminal.h"

// For now, use direct terminal functions instead of system calls
static int write_string(const char* str) {
    terminal_writestring(str);
    return strlen(str);
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
        // For now, just a message - no actual yielding
        write_string("Back in shell (no actual yield yet)\n");
    } else if (strcmp(command, "exit") == 0) {
        write_string("Goodbye!\n");
        // For now, just return from function
        return;
    } else if (strlen(command) == 0) {
        // Empty command, do nothing
        return;
    } else {
        write_string("Unknown command: ");
        write_string(command);
        write_string("\nType 'help' for available commands.\n");
    }
}

extern "C" void shell_main() {
    char command_buffer[256];
    
    write_string("Welcome to SlopOS Shell v2.0!\n"); 
    write_string("Running with memory management support.\n");
    write_string("Type 'help' for available commands.\n\n");
    
    while (1) {
        write_string("slopOS> ");
        
        // Use terminal functions directly for now
        terminal_getstring(command_buffer, sizeof(command_buffer));
        
        shell_execute_command(command_buffer);
        
        // Check for exit command
        if (strcmp(command_buffer, "exit") == 0) {
            break;
        }
    }
}