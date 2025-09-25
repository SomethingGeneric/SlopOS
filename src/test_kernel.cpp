// Simple test kernel to verify boot process
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

// Simple terminal output for testing
static void terminal_putchar(char c) {
    volatile uint16_t* terminal_buffer = (volatile uint16_t*)0xB8000;
    static size_t terminal_row = 0;
    static size_t terminal_column = 0;
    static const size_t VGA_WIDTH = 80;
    static const size_t VGA_HEIGHT = 25;
    
    if (c == '\n') {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT) {
            terminal_row = 0;
        }
    } else {
        const size_t index = terminal_row * VGA_WIDTH + terminal_column;
        terminal_buffer[index] = (uint16_t) c | (uint16_t) 0x0F00;
        if (++terminal_column == VGA_WIDTH) {
            terminal_column = 0;
            if (++terminal_row == VGA_HEIGHT) {
                terminal_row = 0;
            }
        }
    }
}

static void terminal_writestring(const char* data) {
    for (size_t i = 0; data[i] != '\0'; i++) {
        terminal_putchar(data[i]);
    }
}

extern "C" void kernel_main() {
    // Clear screen first
    volatile uint16_t* terminal_buffer = (volatile uint16_t*)0xB8000;
    for (int i = 0; i < 80 * 25; i++) {
        terminal_buffer[i] = 0x0F20; // Space with white on black
    }
    
    terminal_writestring("TEST KERNEL BOOTED SUCCESSFULLY!\n");
    terminal_writestring("If you can see this, the boot process works.\n");
    terminal_writestring("This confirms the multiboot setup is correct.\n");
    terminal_writestring("Now we can debug the main kernel...\n");
    
    // Infinite loop
    while (1) {
        asm volatile ("hlt");
    }
}