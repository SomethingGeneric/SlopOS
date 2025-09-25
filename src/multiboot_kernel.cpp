// Test version to debug terminal output
#include "types.h"
#include "terminal.h"
#include "string.h"

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

extern "C" void kernel_main() {
    // Initialize terminal
    terminal_initialize();
    
    // Simple test output
    terminal_writestring("Hello from SlopOS!\n");
    terminal_writestring("Terminal test successful.\n");
    terminal_writestring("String input/output test ready.\n\n");
    
    // Buffer for string input
    char input_buffer[256];
    
    // Main input/output loop
    while (1) {
        terminal_writestring("Enter a string: ");
        terminal_getstring(input_buffer, sizeof(input_buffer));
        terminal_writestring("You entered: ");
        terminal_writestring(input_buffer);
        terminal_writestring("\n\n");
    }
}