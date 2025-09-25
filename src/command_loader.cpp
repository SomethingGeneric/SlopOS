#include "command_loader.h"
#include "process.h"
#include "filesystem.h"
#include "memory.h"
#include "string.h"

// Command registry structure
typedef struct {
    const char* name;
    void (*entry_point)();
} command_entry_t;

// Registry of available commands
static command_entry_t command_registry[] = {
    {"version", version_process_main},
    {"hello", hello_process_main},
    {"help", help_process_main},
    {"ps", ps_process_main},
    {NULL, NULL} // Sentinel
};

// Initialize command loader and register commands in filesystem
void command_loader_init() {
    filesystem_init();
    
    // Register each command as a "file" in our in-memory filesystem
    for (int i = 0; command_registry[i].name != NULL; i++) {
        command_entry_t* cmd = &command_registry[i];
        
        // Create a simple "executable" - just store the function pointer
        uint8_t* executable = (uint8_t*)kmalloc(sizeof(void*));
        if (executable) {
            *((void**)executable) = (void*)cmd->entry_point;
            filesystem_add_file(cmd->name, executable, sizeof(void*));
            kfree(executable); // Filesystem makes its own copy
        }
    }
}

// Spawn a command as a process
uint32_t command_spawn_process(const char* command_name) {
    uint32_t size;
    uint8_t* executable = filesystem_read_file(command_name, &size);
    
    if (!executable) {
        return 0; // Command not found
    }
    
    // Create process from the executable
    return process_create_from_buffer(command_name, executable, size, 1);
}

// Check if command is available
bool command_is_available(const char* command_name) {
    return filesystem_find_file(command_name) != NULL;
}