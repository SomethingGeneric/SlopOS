#include "syscall.h" 
#include "string.h"
#include "terminal.h"
#include "process.h"
#include "timer.h"
#include "command_loader.h"
// Remove old command includes since we'll use the process loader
// #include "commands/version.h"
// #include "commands/hello.h"
// #include "commands/ps.h"
// #include "commands/uptime.h"
// #include "commands/memory.h"
// #include "commands/yield.h"
// #include "commands/help.h"

// For now, use direct terminal functions instead of system calls
static void write_string(const char* str) {
    terminal_writestring(str);
}

// Shell command execution
void shell_execute_command(const char* command) {
    if (strlen(command) == 0) {
        // Empty command, do nothing
        return;
    } else if (strcmp(command, "exit") == 0) {
        write_string("Goodbye!\n");
        // Terminate this process
        process_t* current = process_get_current();
        if (current) {
            process_terminate(current->pid);
        }
        return;
    } else {
        // Try to spawn the command as a process
        uint32_t pid = command_spawn_process(command);
        
        if (pid > 0) {
            // Command spawned successfully, yield to let it run
            process_yield();
        } else {
            write_string("Unknown command: ");
            write_string(command);
            write_string("\nType 'help' for available commands.\n");
        }
    }
}

extern "C" void shell_main() {
    char command_buffer[256];
    
    write_string("Welcome to SlopOS Shell v3.0!\n"); 
    write_string("Running with process-based command system.\n");
    write_string("All commands now run as separate processes.\n");
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