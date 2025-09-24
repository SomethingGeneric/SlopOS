// SlopOS Kernel
// The kernel functionality has been moved to the bootloader (boot/bootloader.asm)
// for simplicity in this minimal operating system.
//
// The bootloader now contains:
// - Welcome message printing
// - Basic shell with command loop
// - 'uptime' command showing seconds since boot
// - 'version' command showing "SlopOS 1.0"
//
// This file is kept for future expansion when we move to protected mode
// and need separate kernel functionality.

extern "C" void kernel_main() {
    // This function is currently not used as the kernel is embedded
    // in the bootloader for the minimal implementation
    
    // In the future, this will contain:
    // - Protected mode initialization
    // - Memory management
    // - Process management
    // - More complex shell functionality
    
    // Halt the CPU
    while (true) {
        asm volatile ("hlt");
    }
}