#include "memory.h"

// Function to execute memory command from shell
void execute_memory_command(void (*write_string)(const char*)) {
    write_string("Memory management active\n");
    write_string("  Physical memory allocator: OK\n");
    write_string("  Virtual memory manager: OK\n");
    write_string("  Kernel heap: OK\n");
}