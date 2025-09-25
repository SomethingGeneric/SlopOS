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
  - `multiboot_kernel.cpp` - Main kernel with process and memory management
  - `multiboot_entry.asm` - Assembly entry point for multiboot kernel
  - `multiboot.ld` - Linker script for multiboot ELF kernel
  - `terminal.cpp` - VGA text mode terminal driver
  - `string.cpp` - String manipulation functions
  - `timer.cpp` - Timer functionality
  - **`memory.cpp` - Physical memory allocator and heap management**
  - **`process.cpp` - Process control and scheduling**
  - **`context_switch.asm` - Assembly context switching code**
  - **`syscall.cpp` - System call interface implementation**
  - **`shell.cpp` - Shell as separate integrated process**
- `build/` - Generated build artifacts and ISO image
- `Makefile` - Build configuration for GRUB-based system with new components
- `run.sh` - Build and run script
## Features
- [x] GRUB-based bootloader (reliable, no boot loops)
- [x] Multiboot-compliant 32-bit protected mode kernel  
- [x] VGA text mode output with proper newline and cursor positioning
- [x] Clean build system with ISO generation
- [x] QEMU testing support
- [x] Resolves previous boot loop issues
- [x] Interactive shell with command input/output
- [x] Shell commands (version, hello, uptime, about, help)
- [x] Keyboard input support with character echo
- [x] Hardware cursor positioning management
- [x] **Memory management system (physical memory allocator)**
- [x] **Process management framework (PCB, scheduling)**
- [x] **System call interface**
- [x] **Separated shell architecture**

## Process and Memory Management

SlopOS v2.0 introduces fundamental operating system features:

### Memory Management
- Physical memory allocator with bitmap-based free page tracking
- Kernel heap allocator (kmalloc/kfree) for dynamic memory allocation
- Memory initialization and management for 32MB RAM
- Foundation for virtual memory management (paging support prepared)

### Process Management
- Process Control Block (PCB) structure for process metadata
- Process creation, termination, and state management
- Basic cooperative multitasking scheduler (round-robin)
- Context switching support with assembly implementation
- Support for up to 32 concurrent processes

### System Architecture
- Clear separation between kernel and user space
- System call interface for user-kernel communication
- Shell runs as integrated process with memory management
- Modular design allowing future enhancements

### System Calls
- sys_exit - Process termination  
- sys_write - Terminal output
- sys_read - Terminal input
- sys_yield - Cooperative multitasking

## Boot Loop Fix

This version of SlopOS resolves the previous boot loop issue by:
- Replacing the problematic custom bootloader with GRUB
- Using the multiboot specification for reliable kernel loading
- Implementing proper multiboot headers and entry points
- Providing a stable, tested boot process

The GRUB bootloader successfully loads the kernel and displays a working system with basic shell functionality.

## Shell Commands

SlopOS now includes an enhanced shell with process and memory management features:

- `version` - displays "slopOS 2.0 - Now with process management!"
- `hello` - displays "world"  
- `ps` - shows running processes (kernel and shell)
- `memory` - displays memory management status
- `yield` - demonstrates cooperative multitasking
- `exit` - exits the shell
- `help` - displays list of available commands

The shell features:
- Interactive command input with keyboard support
- Proper cursor positioning that follows text input/output
- Command parsing and execution
- Error handling for unknown commands
- Real-time user interaction
- Professional shell prompt "slopOS> "
- **Memory management integration**
- **Process management awareness**
