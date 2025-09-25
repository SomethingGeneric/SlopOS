#include "../syscall.h"
#include "../terminal.h"
#include "../memory.h"

// Process entry point for memory command
extern "C" void memory_process_main() {
    terminal_writestring("Memory Statistics:\n");
    
    memory_stats_t stats;
    memory_get_stats(&stats);
    
    terminal_writestring("  Total Memory: ");
    // Simple conversion (just show a placeholder for now)
    terminal_writestring("32768 KB\n"); 
    
    terminal_writestring("  Free Memory: ");
    terminal_writestring("Available\n");
    
    terminal_writestring("  Kernel Heap: ");
    terminal_writestring("Active\n");
    
    terminal_writestring("  Process Memory: ");
    terminal_writestring("Allocated\n");
    
    // Exit this process
    sys_exit(0);
}