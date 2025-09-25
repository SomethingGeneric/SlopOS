#include "../syscall.h"
#include "../terminal.h"

// Process entry point for help command
extern "C" void help_process_main() {
    terminal_writestring("Available commands:\n");
    terminal_writestring("  version - Show OS version\n");
    terminal_writestring("  hello   - Hello world message\n");
    terminal_writestring("  ps      - Show running processes\n");
    terminal_writestring("  uptime  - Show system uptime\n");
    terminal_writestring("  memory  - Show memory statistics\n");
    terminal_writestring("  yield   - Yield CPU to other processes\n");
    terminal_writestring("  help    - Show this help message\n");
    terminal_writestring("  exit    - Exit shell\n");
    
    // Exit this process
    sys_exit(0);
}