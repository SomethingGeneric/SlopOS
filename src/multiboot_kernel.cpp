// SlopOS Kernel with process and memory management
#include "types.h"
#include "terminal.h"
#include "string.h"
#include "timer.h"
#include "memory.h"
#include "process.h"
#include "syscall.h"

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
    // Initialize terminal first
    terminal_initialize();
    
    // Simple test message
    terminal_writestring("SlopOS v2.0 - Memory and Process Management\n");
    terminal_writestring("Kernel started successfully!\n");
    
    // Test memory allocation
    terminal_writestring("Initializing memory management...\n");
    memory_init(32 * 1024 * 1024);
    terminal_writestring("Memory manager initialized.\n");
    
    // Test basic allocation
    void* test_ptr = kmalloc(1024);
    if (test_ptr) {
        terminal_writestring("Memory allocation test: SUCCESS\n");
    } else {
        terminal_writestring("Memory allocation test: FAILED\n");
    }
    
    // Initialize other systems
    timer_initialize();
    process_init();
    syscall_init();
    
    terminal_writestring("All systems initialized.\n");
    terminal_writestring("Starting shell as separate process...\n\n");
    
    // Create shell process instead of calling directly
    uint32_t shell_pid = process_create("shell", shell_main, 1);
    if (shell_pid == 0) {
        terminal_writestring("Failed to create shell process!\n");
        while (1) {
            asm volatile ("hlt");
        }
    }
    
    terminal_writestring("Shell process created successfully.\n");
    terminal_writestring("Entering scheduler loop...\n\n");
    
    // Kernel becomes idle process - keep scheduling
    while (1) {
        process_schedule();
        // Small delay to prevent busy waiting
        for (volatile int i = 0; i < 100000; i++);
    }
}