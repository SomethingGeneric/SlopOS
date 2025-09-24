// Simple test kernel for Limine bootloader
#include "limine.h"

// Request some bootloader features using the Limine protocol
__attribute__((used, section(".requests")))
static volatile LIMINE_BASE_REVISION(2);

// Halt and catch fire
extern "C" void hcf() {
    for (;;) {
        asm ("hlt");
    }
}

// Simple function to write to VGA memory directly
void vga_write(const char* str) {
    volatile uint16_t* vga_buffer = (volatile uint16_t*)0xB8000;
    int i = 0;
    while (str[i] != '\0') {
        vga_buffer[i] = (0x0F << 8) | str[i]; // White on black
        i++;
    }
}

// Main kernel entry point
extern "C" void kernel_main() {
    // Write directly to VGA memory to test if we get here
    vga_write("Hello from Limine kernel!");
    
    // Infinite loop
    hcf();
}