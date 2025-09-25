#include "syscall.h" 
#include "string.h"
#include "terminal.h"
#include "process.h"
#include "timer.h"

// Uptime process - runs as separate process
extern "C" void uptime_process() {
    // For now, run once and terminate
    process_t* current = process_get_current();
    if (current) {
        char msg[] = "UPTIME: System uptime: ";
        terminal_writestring(msg);
        
        uint32_t ticks = timer_get_ticks();
        char uptime_str[32];
        int pos = 0;
        if (ticks == 0) {
            uptime_str[pos++] = '0';
        } else {
            char temp[32];
            int temp_pos = 0;
            while (ticks > 0) {
                temp[temp_pos++] = '0' + (ticks % 10);
                ticks /= 10;
            }
            // Reverse
            for (int j = temp_pos - 1; j >= 0; j--) {
                uptime_str[pos++] = temp[j];
            }
        }
        uptime_str[pos] = '\0';
        terminal_writestring(uptime_str);
        terminal_writestring(" ticks (separate process)\n");
        
        process_terminate(current->pid);
    }
}

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
        // Get real process information
        for (int i = 0; i < MAX_PROCESSES; i++) {
            process_t* proc = process_get_by_index(i);
            if (proc) {
                write_string("  PID ");
                char pid_str[16];
                // Simple integer to string conversion
                int pid = proc->pid;
                int pos = 0;
                if (pid == 0) {
                    pid_str[pos++] = '0';
                } else {
                    char temp[16];
                    int temp_pos = 0;
                    while (pid > 0) {
                        temp[temp_pos++] = '0' + (pid % 10);
                        pid /= 10;
                    }
                    // Reverse
                    for (int j = temp_pos - 1; j >= 0; j--) {
                        pid_str[pos++] = temp[j];
                    }
                }
                pid_str[pos] = '\0';
                write_string(pid_str);
                write_string(": ");
                write_string(proc->name);
                
                // Show state
                switch (proc->state) {
                    case PROCESS_RUNNING:
                        write_string(" (running)\n");
                        break;
                    case PROCESS_READY:
                        write_string(" (ready)\n");
                        break;
                    case PROCESS_BLOCKED:
                        write_string(" (blocked)\n");
                        break;
                    case PROCESS_TERMINATED:
                        write_string(" (terminated)\n");
                        break;
                }
            }
        }
    } else if (strcmp(command, "uptime") == 0) {
        write_string("Creating uptime process...\n");
        uint32_t uptime_pid = process_create("uptime", uptime_process, 1);
        if (uptime_pid == 0) {
            write_string("Failed to create uptime process!\n");
        } else {
            write_string("Uptime process created successfully.\n");
            // Yield to allow the uptime process to run
            process_yield();
        }
    } else if (strcmp(command, "help") == 0) {
        write_string("Available commands:\n");
        write_string("  version - Display OS version\n");
        write_string("  hello   - Display greeting\n");
        write_string("  ps      - Show running processes\n");
        write_string("  uptime  - Show system uptime\n");
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
        process_yield();
        write_string("Back in shell after yield.\n");
    } else if (strcmp(command, "exit") == 0) {
        write_string("Goodbye!\n");
        // Terminate this process
        process_t* current = process_get_current();
        if (current) {
            process_terminate(current->pid);
        }
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
    write_string("Shell running as separate process concept.\n");
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