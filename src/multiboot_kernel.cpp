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
    // Initialize terminal
    terminal_initialize();
    
    // Initialize timer
    timer_initialize();
    
    // Initialize memory management (assume 32MB of RAM for now)
    memory_init(32 * 1024 * 1024);
    
    // Initialize process management
    process_init();
    
    // Initialize system calls
    syscall_init();
    
    // Kernel startup message
    terminal_writestring("SlopOS v2.0 - Process and Memory Management\n");
    terminal_writestring("=========================================\n");
    terminal_writestring("Kernel initialization complete:\n");
    terminal_writestring("  - Terminal driver: OK\n");
    terminal_writestring("  - Timer system: OK\n");
    terminal_writestring("  - Memory manager: OK\n");
    terminal_writestring("  - Process manager: OK\n");
    terminal_writestring("  - System calls: OK\n");
    terminal_writestring("\nStarting shell as user process...\n\n");
    
    // Create shell process
    uint32_t shell_pid = process_create("shell", shell_main, 1);
    if (shell_pid == 0) {
        terminal_writestring("ERROR: Failed to create shell process!\n");
        terminal_writestring("Falling back to kernel mode...\n");
        
        // Fallback: simple kernel loop
        while (1) {
            terminal_writestring("kernel> System ready (no shell)\n");
            // Simple delay
            for (volatile int i = 0; i < 10000000; i++);
        }
    }
    
    terminal_writestring("Shell process created with PID: ");
    char pid_str[16];
    itoa32(shell_pid, pid_str, 10);
    terminal_writestring(pid_str);
    terminal_writestring("\n");
    
    // Kernel idle loop - yield to other processes
    while (1) {
        process_yield();
        // Simple delay to prevent busy waiting
        for (volatile int i = 0; i < 1000; i++);
    }
}