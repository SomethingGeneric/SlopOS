// SlopOS Kernel - Updated for Limine bootloader
// Main kernel with shell functionality

#include "limine.h"
#include "terminal.h"
#include "string.h"
#include "timer.h"

// Request some bootloader features using the Limine protocol
__attribute__((used, section(".requests")))
static volatile LIMINE_BASE_REVISION(2);

// Request a terminal
__attribute__((used, section(".requests")))
static volatile struct limine_terminal_request terminal_request = {
    .id = LIMINE_TERMINAL_REQUEST,
    .revision = 0
};

// Request memory map
__attribute__((used, section(".requests")))
static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

// Request bootloader info
__attribute__((used, section(".requests")))
static volatile struct limine_bootloader_info_request bootloader_info_request = {
    .id = LIMINE_BOOTLOADER_INFO_REQUEST,
    .revision = 0
};

// Function prototypes
extern "C" void kernel_main();
void read_input(char* buffer, int max_length);
void process_command(const char* command);
void show_uptime();

static char input_buffer[128];
static uint32_t boot_time = 0;

// Terminal write wrapper for Limine
static limine_terminal_write_func terminal_write_ptr = nullptr;

// Use Limine terminal if available, otherwise fallback to VGA
void terminal_writestring_limine(const char* str) {
    if (terminal_write_ptr != nullptr) {
        terminal_write_ptr(str, strlen(str));
    } else {
        // Fallback to VGA terminal
        terminal_writestring(str);
    }
}

// Halt and catch fire
extern "C" void hcf() {
    for (;;) {
        asm ("hlt");
    }
}

// Main kernel entry point
extern "C" void kernel_main() {
    // Initialize VGA terminal as fallback
    terminal_initialize();
    
    // Check if we have a Limine terminal
    if (terminal_request.response) {
        terminal_write_ptr = terminal_request.response->write;
    }
    
    // Store boot time (simplified)
    boot_time = timer_get_ticks();
    
    // Print welcome message
    terminal_writestring_limine("Welcome to SlopOS (Limine Edition)!\n");
    terminal_writestring_limine("Kernel loaded successfully with Limine bootloader!\n");
    terminal_writestring_limine("Architecture: x86-64 with Limine boot protocol\n");
    
    // Show bootloader info if available
    if (bootloader_info_request.response) {
        terminal_writestring_limine("Bootloader: ");
        terminal_writestring_limine(bootloader_info_request.response->name);
        terminal_writestring_limine(" ");
        terminal_writestring_limine(bootloader_info_request.response->version);
        terminal_writestring_limine("\n");
    }
    
    // Show memory info if available
    if (memmap_request.response) {
        terminal_writestring_limine("Memory map entries: ");
        char buffer[32];
        itoa32(memmap_request.response->entry_count, buffer, 10);
        terminal_writestring_limine(buffer);
        terminal_writestring_limine("\n");
    }
    
    terminal_writestring_limine("Available commands: uptime, version, help\n\n");
    
    // Main shell loop
    while (true) {
        terminal_writestring_limine("> ");
        read_input(input_buffer, sizeof(input_buffer));
        process_command(input_buffer);
    }
}

void read_input(char* buffer, int max_length) {
    int pos = 0;
    char c;
    
    while (pos < max_length - 1) {
        c = terminal_getchar();
        
        if (c == '\n') {
            buffer[pos] = '\0';
            terminal_writestring_limine("\n");
            break;
        } else if (c == '\b' && pos > 0) {
            pos--;
            terminal_writestring_limine("\b");
        } else if (c >= 32 && c <= 126) {
            buffer[pos++] = c;
            char temp[2] = {c, '\0'};
            terminal_writestring_limine(temp);
        }
    }
    
    if (pos >= max_length - 1) {
        buffer[max_length - 1] = '\0';
    }
}

void process_command(const char* command) {
    if (strlen(command) == 0) {
        return;
    }
    
    if (strcmp(command, "uptime") == 0) {
        show_uptime();
    } else if (strcmp(command, "version") == 0) {
        terminal_writestring_limine("SlopOS 2.0 - Limine bootloader edition\n");
    } else if (strcmp(command, "help") == 0) {
        terminal_writestring_limine("Available commands:\n");
        terminal_writestring_limine("  uptime  - Show system uptime\n");
        terminal_writestring_limine("  version - Show OS version\n");
        terminal_writestring_limine("  help    - Show this help\n");
    } else {
        terminal_writestring_limine("Unknown command: ");
        terminal_writestring_limine(command);
        terminal_writestring_limine("\nType 'help' for available commands.\n");
    }
}

void show_uptime() {
    uint32_t current_time = timer_get_ticks();
    uint32_t uptime_ticks = current_time - boot_time;
    
    // Convert ticks to seconds (rough approximation)
    uint32_t uptime_seconds = uptime_ticks / 1000000; // Rough estimate
    
    terminal_writestring_limine("Uptime: ");
    
    char buffer[32];
    itoa32(uptime_seconds, buffer, 10);
    terminal_writestring_limine(buffer);
    terminal_writestring_limine(" seconds\n");
}