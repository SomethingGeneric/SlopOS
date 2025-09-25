#include "command_loader.h"
#include "process.h"
#include "filesystem.h"
#include "memory.h"
#include "string.h"

// Simple command binary structure - contains actual compiled code
typedef struct {
    uint32_t entry_offset;  // Offset to entry point from start
    uint32_t code_size;     // Size of code section
    uint8_t code[];         // Actual machine code
} simple_executable_t;

// Forward declarations for command processes
extern "C" void version_process_main();
extern "C" void hello_process_main();
extern "C" void help_process_main();
extern "C" void ps_process_main();
extern "C" void uptime_process_main();
extern "C" void memory_process_main();
extern "C" void yield_process_main();

// Initialize command loader and create process-ready commands
void command_loader_init() {
    filesystem_init();
    
    // For this simplified implementation, we'll register commands
    // by directly creating processes when requested rather than 
    // storing executables in the filesystem. This is more practical
    // for the current kernel architecture.
}

// Spawn a command as a process using direct function call
uint32_t command_spawn_process(const char* command_name) {
    void (*entry_point)() = NULL;
    
    // Map command names to their entry points
    if (strcmp(command_name, "version") == 0) {
        entry_point = version_process_main;
    } else if (strcmp(command_name, "hello") == 0) {
        entry_point = hello_process_main;
    } else if (strcmp(command_name, "help") == 0) {
        entry_point = help_process_main;
    } else if (strcmp(command_name, "ps") == 0) {
        entry_point = ps_process_main;
    } else if (strcmp(command_name, "uptime") == 0) {
        entry_point = uptime_process_main;
    } else if (strcmp(command_name, "memory") == 0) {
        entry_point = memory_process_main;
    } else if (strcmp(command_name, "yield") == 0) {
        entry_point = yield_process_main;
    }
    
    if (entry_point) {
        // Create process with the function entry point
        return process_create(command_name, entry_point, 1);
    }
    
    return 0; // Command not found
}

// Check if command is available
bool command_is_available(const char* command_name) {
    return (strcmp(command_name, "version") == 0 ||
            strcmp(command_name, "hello") == 0 ||
            strcmp(command_name, "help") == 0 ||
            strcmp(command_name, "ps") == 0 ||
            strcmp(command_name, "uptime") == 0 ||
            strcmp(command_name, "memory") == 0 ||
            strcmp(command_name, "yield") == 0);
}