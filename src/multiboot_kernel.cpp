// SlopOS Kernel with basic shell functionality
#include "types.h"
#include "terminal.h"
#include "string.h"
#include "timer.h"

// Multiboot header
struct multiboot_header {
    uint32_t magic;
    uint32_t flags;
    uint32_t checksum;
} __attribute__((packed));

// Place multiboot header in a special section
__attribute__((section(".multiboot")))
static struct multiboot_header multiboot_hdr = {
    .magic = 0x1BADB002,    // Multiboot magic number
    .flags = 0,             // No special flags needed
    .checksum = 0xE4524FFE  // -(0x1BADB002 + 0) calculated manually
};

// Shell command execution
void shell_execute_command(const char* command) {
    if (strcmp(command, "version") == 0) {
        terminal_writestring("slopOS 1.0\n");
    } else if (strcmp(command, "hello") == 0) {
        terminal_writestring("world\n");
    } else if (strcmp(command, "uptime") == 0) {
        char buffer[32];
        uint32_t ticks = timer_get_ticks();
        itoa32(ticks, buffer, 10);
        terminal_writestring("System uptime: ");
        terminal_writestring(buffer);
        terminal_writestring(" CPU ticks\n");
    } else if (strcmp(command, "help") == 0) {
        terminal_writestring("Available commands:\n");
        terminal_writestring("  version - Display OS version\n");
        terminal_writestring("  hello   - Display greeting\n");
        terminal_writestring("  uptime  - Display system uptime\n");
        terminal_writestring("  help    - Show this help message\n");
    } else if (strlen(command) == 0) {
        // Empty command, do nothing
        return;
    } else {
        terminal_writestring("Unknown command: ");
        terminal_writestring(command);
        terminal_writestring("\nType 'help' for available commands.\n");
    }
}

extern "C" void kernel_main() {
    // Initialize terminal
    terminal_initialize();
    
    // Initialize timer
    timer_initialize();
    
    // Welcome message
    terminal_writestring("Welcome to SlopOS!\n");
    terminal_writestring("Type 'help' for available commands.\n\n");
    
    // Buffer for command input
    char command_buffer[256];
    
    // Main shell loop
    while (1) {
        terminal_writestring("slopOS> ");
        terminal_getstring(command_buffer, sizeof(command_buffer));
        shell_execute_command(command_buffer);
    }
}