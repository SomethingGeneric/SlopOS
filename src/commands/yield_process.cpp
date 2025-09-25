#include "../syscall.h"
#include "../terminal.h"
#include "../process.h"

// Process entry point for yield command
extern "C" void yield_process_main() {
    terminal_writestring("Yielding CPU to other processes...\n");
    
    // Perform a yield
    process_yield();
    
    terminal_writestring("CPU control returned.\n");
    
    // Exit this process
    sys_exit(0);
}