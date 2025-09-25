// SlopOS Kernel with process and memory management
#include "types.h"
#include "terminal.h"
#include "string.h"
#include "timer.h"
#include "memory.h"
#include "process.h"
#include "syscall.h"
#include "command_loader.h"

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

// Forward declaration of shell main function
extern "C" void shell_main();

extern "C" void kernel_main() {
    // Simple serial output for debugging (COM1 port 0x3F8)
    auto serial_putchar = [](char c) {
        while ((*(volatile uint8_t*)0x3FD & 0x20) == 0); // Wait for transmit ready
        *(volatile uint8_t*)0x3F8 = c; // Send character
    };
    
    auto serial_writestring = [&](const char* str) {
        for (int i = 0; str[i] != '\0'; i++) {
            serial_putchar(str[i]);
        }
    };
    
    // Initialize serial port
    *(volatile uint8_t*)0x3F9 = 0x00; // Disable interrupts
    *(volatile uint8_t*)0x3FB = 0x80; // Enable DLAB
    *(volatile uint8_t*)0x3F8 = 0x03; // Set divisor low byte (38400 baud)
    *(volatile uint8_t*)0x3F9 = 0x00; // Set divisor high byte
    *(volatile uint8_t*)0x3FB = 0x03; // 8 bits, no parity, one stop bit
    *(volatile uint8_t*)0x3FC = 0x0B; // IRQs enabled, RTS/DSR set
    
    serial_writestring("KERNEL: Starting SlopOS kernel...\r\n");
    
    // Initialize terminal first
    terminal_initialize();
    
    // Simple test message
    terminal_writestring("SlopOS v3.0 - Process-Based Command System\n");
    terminal_writestring("Kernel started successfully!\n");
    
    serial_writestring("KERNEL: Terminal initialized\r\n");
    
    // Test memory allocation
    terminal_writestring("Initializing memory management...\n");
    memory_init(32 * 1024 * 1024);
    terminal_writestring("Memory manager initialized.\n");
    
    serial_writestring("KERNEL: Memory initialized\r\n");
    
    // Test basic allocation
    void* test_ptr = kmalloc(1024);
    if (test_ptr) {
        terminal_writestring("Memory allocation test: SUCCESS\n");
        serial_writestring("KERNEL: Memory allocation test passed\r\n");
    } else {
        terminal_writestring("Memory allocation test: FAILED\n");
        serial_writestring("KERNEL: Memory allocation test FAILED\r\n");
    }
    
    // Initialize other systems
    timer_initialize();
    process_init();
    syscall_init();
    command_loader_init();
    
    serial_writestring("KERNEL: All systems initialized\r\n");
    
    terminal_writestring("All systems initialized.\n");
    terminal_writestring("Commands are now loaded as processes.\n");
    terminal_writestring("Available commands: version, hello, help, ps, uptime, memory, yield\n");
    terminal_writestring("Starting shell (process-based command system active)...\n\n");
    
    serial_writestring("KERNEL: About to start shell\r\n");
    
    // For now, call shell function directly but make it process-aware
    shell_main();
    
    // Should not normally reach here
    terminal_writestring("Shell exited. System halting.\n");
    serial_writestring("KERNEL: Shell exited, halting\r\n");
    while (1) {
        asm volatile ("hlt");
    }
}