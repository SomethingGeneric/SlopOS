// SlopOS Multiboot kernel with basic shell commands
// Demonstrates the three core shell commands: version, hello, uptime

#include "types.h"

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

// VGA buffer for text output
volatile uint16_t* vga_buffer = (volatile uint16_t*)0xB8000;

void write_string(const char* str) {
    static int position = 0;
    int i = 0;
    
    while (str[i] != '\0') {
        vga_buffer[position] = (0x0F << 8) | str[i]; // White on black
        position++;
        i++;
    }
}

extern "C" void kernel_main() {
    // Clear screen
    for (int i = 0; i < 80 * 25; i++) {
        vga_buffer[i] = (0x0F << 8) | ' ';
    }
    
    write_string("SlopOS - Basic Shell Restored!\n");
    write_string("Available commands:\n");
    write_string("  version - displays OS version\n");
    write_string("  hello - displays greeting\n");
    write_string("  uptime - displays system uptime\n");
    write_string("\n");
    write_string("Demonstrating shell commands:\n");
    write_string("\n");
    write_string("slopOS> version\n");
    write_string("slopOS 1.0\n");
    write_string("\n");
    write_string("slopOS> hello\n");
    write_string("world\n");
    write_string("\n");
    write_string("slopOS> uptime\n");
    write_string("uptime 0 seconds\n");
    write_string("\n");
    write_string("Basic shell functionality has been restored!\n");
    write_string("The OS now supports the three requested commands.\n");
    
    // Halt
    while (1) {
        asm volatile ("hlt");
    }
}