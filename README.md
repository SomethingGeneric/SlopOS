# SlopOS
A minimal operating system built with GRUB bootloader

SlopOS is a simple operating system that demonstrates multiboot kernel development using the GRUB bootloader. It resolves the previous boot loop issues by using a proven, reliable bootloader instead of a custom implementation.

## Architecture

SlopOS uses the multiboot specification and boots via GRUB, providing a stable and reliable boot process.

### Boot Process
1. **GRUB Bootloader**: Handles hardware initialization and multiboot protocol
2. **Multiboot Kernel**: 32-bit protected mode kernel with multiboot headers
3. **VGA Text Output**: Simple text-based output for demonstration

## Building and Running

### Prerequisites
- NASM assembler
- GCC compiler 
- GRUB tools (grub-mkrescue, grub-pc-bin)
- QEMU x86-64 emulator
- Make
- xorriso (for ISO creation)
- mtools

### Quick Start
```bash
# Build and run the OS (builds ISO and boots with GRUB)
./run.sh

# Or manually:
make all        # Build ISO image
make run        # Run in QEMU (nographic)
make run-gui    # Run in QEMU with graphics
make clean      # Clean build files
```

### Project Structure  
- `src/` - Kernel source code
  - `multiboot_kernel.cpp` - Main kernel with multiboot headers and basic shell commands
  - `multiboot_entry.asm` - Assembly entry point for multiboot kernel
  - `multiboot.ld` - Linker script for multiboot ELF kernel
  - `terminal.cpp` - VGA text mode terminal driver (available for expansion)
  - `string.cpp` - String manipulation functions (available for expansion)
  - `timer.cpp` - Timer functionality (available for expansion)
- `build/` - Generated build artifacts and ISO image
- `Makefile` - Build configuration for GRUB-based system
- `run.sh` - Build and run script
## Features
- [x] GRUB-based bootloader (reliable, no boot loops)
- [x] Multiboot-compliant 32-bit protected mode kernel  
- [x] VGA text mode output with proper newline handling
- [x] Clean build system with ISO generation
- [x] QEMU testing support
- [x] Resolves previous boot loop issues
- [x] Interactive shell with command input/output
- [x] Shell commands (version, hello, uptime, help)
- [x] Keyboard input support with character echo
- [ ] Memory management (future enhancement)
- [ ] Process management (future enhancement)

## Boot Loop Fix

This version of SlopOS resolves the previous boot loop issue by:
- Replacing the problematic custom bootloader with GRUB
- Using the multiboot specification for reliable kernel loading
- Implementing proper multiboot headers and entry points
- Providing a stable, tested boot process

The GRUB bootloader successfully loads the kernel and displays a working system with basic shell functionality.

## Shell Commands

SlopOS now includes an interactive shell that allows users to type commands and see their output:

- `version` - displays "slopOS 1.0"
- `hello` - displays "world"  
- `uptime` - displays system uptime in CPU ticks
- `help` - displays list of available commands

The shell features:
- Interactive command input with keyboard support
- Proper newline handling for readable output  
- Command parsing and execution
- Error handling for unknown commands
- Real-time user interaction
