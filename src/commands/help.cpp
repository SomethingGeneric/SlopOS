#include "help.h"

// Function to execute help command from shell
void execute_help_command(void (*write_string)(const char*)) {
    write_string("Available commands:\n");
    write_string("  version - Display OS version\n");
    write_string("  hello   - Display greeting\n");
    write_string("  ps      - Show running processes\n");
    write_string("  uptime  - Show system uptime\n");
    write_string("  memory  - Show memory usage\n");
    write_string("  yield   - Yield CPU to other processes\n");
    write_string("  exit    - Exit shell\n");
    write_string("  help    - Show this help message\n");
}