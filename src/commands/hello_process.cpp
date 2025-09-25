#include "../syscall.h"
#include "../terminal.h"

// Process entry point for hello command
extern "C" void hello_process_main() {
    terminal_writestring("Hello from SlopOS!\n");
    terminal_writestring("This command is now running as a separate process!\n");
    
    // Exit this process
    sys_exit(0);
}