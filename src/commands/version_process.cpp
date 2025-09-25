#include "../syscall.h"
#include "../terminal.h"

// Process entry point for version command
extern "C" void version_process_main() {
    // Use terminal functions directly for now (will use syscalls later)
    terminal_writestring("slopOS 2.0 - Now with process management!\n");
    
    // Exit this process
    sys_exit(0);
}