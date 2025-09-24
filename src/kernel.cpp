// SlopOS Kernel
// Minimal C++ kernel for bare hardware

// For now, this is just a placeholder as the bootloader handles everything
// In the future, this will contain the main kernel logic

extern "C" void kernel_main() {
    // This function would be called by the bootloader in a more complex setup
    // For now, the bootloader directly prints the message
    
    // Halt the CPU
    while (true) {
        asm volatile ("hlt");
    }
}