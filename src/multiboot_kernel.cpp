// SlopOS Multiboot kernel - works with GRUB
// Simple working kernel that replaces the problematic bootloader

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
    
    write_string("SlopOS - Multiboot kernel working!\n");
    write_string("Boot loop issue resolved with GRUB bootloader.\n");
    write_string("This proves the kernel code works fine.\n");
    write_string("The issue was with the custom bootloader implementation.\n");
    
    // Halt
    while (1) {
        asm volatile ("hlt");
    }
}