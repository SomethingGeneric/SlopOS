// SlopOS Kernel - 32-bit C++ implementation
// Main kernel with shell functionality moved from assembly to C++

#include "terminal.h"
#include "string.h"
#include "timer.h"

extern "C" void kernel_main();

static char input_buffer[128];
static uint32_t boot_time = 0;

void read_input(char* buffer, int max_length);
void process_command(const char* command);
void show_uptime();

void kernel_main() {
    // Initialize terminal
    terminal_initialize();
    
    // Store boot time (simplified)
    boot_time = timer_get_ticks();
    
    // Print welcome message
    terminal_writestring("Welcome to SlopOS!\n");
    terminal_writestring("C++ kernel loaded successfully!\n");
    terminal_writestring("Architecture upgraded from assembly to C++\n");
    terminal_writestring("Available commands: uptime, version\n\n");
    
    // Main shell loop
    while (true) {
        terminal_writestring("> ");
        read_input(input_buffer, sizeof(input_buffer));
        process_command(input_buffer);
    }
    terminal_writestring("C++ kernel loaded successfully!\n");
    terminal_writestring("Architecture upgraded from assembly to C++\n");
    terminal_writestring("Available commands: uptime, version\n\n");
    
    // Main shell loop
    while (true) {
        terminal_writestring("> ");
        read_input(input_buffer, sizeof(input_buffer));
        process_command(input_buffer);
    }
}

void read_input(char* buffer, int max_length) {
    int index = 0;
    char c;
    
    while (index < max_length - 1) {
        c = terminal_getchar();
        
        if (c == '\n' || c == '\r') {
            buffer[index] = '\0';
            terminal_putchar('\n');
            return;
        } else if (c == '\b' && index > 0) {
            // Backspace
            index--;
            terminal_putchar('\b');
            terminal_putchar(' ');
            terminal_putchar('\b');
        } else if (c >= 32 && c <= 126) {
            // Printable character
            buffer[index] = c;
            index++;
            terminal_putchar(c);
        }
    }
    
    buffer[max_length - 1] = '\0';
    terminal_putchar('\n');
}

void process_command(const char* command) {
    if (strlen(command) == 0) {
        return;
    }
    
    if (strcmp(command, "uptime") == 0) {
        show_uptime();
    } else if (strcmp(command, "version") == 0) {
        terminal_writestring("SlopOS 1.0 - C++ kernel edition\n");
    } else if (strcmp(command, "help") == 0) {
        terminal_writestring("Available commands:\n");
        terminal_writestring("  uptime  - Show system uptime\n");
        terminal_writestring("  version - Show OS version\n");
        terminal_writestring("  help    - Show this help\n");
    } else {
        terminal_writestring("Unknown command: ");
        terminal_writestring(command);
        terminal_writestring("\nType 'help' for available commands.\n");
    }
}

void show_uptime() {
    uint32_t current_time = timer_get_ticks();
    uint32_t uptime_ticks = current_time - boot_time;
    
    // Convert ticks to seconds (rough approximation)
    // In a real OS you'd calibrate this properly
    uint32_t uptime_seconds = uptime_ticks / 1000000; // Rough estimate
    
    terminal_writestring("Uptime: ");
    
    char buffer[32];
    itoa32(uptime_seconds, buffer, 10);
    terminal_writestring(buffer);
    terminal_writestring(" seconds\n");
}