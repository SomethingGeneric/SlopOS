# SlopOS
x64 OS mostly developed by Copilot

A minimal C++ operating system that boots to protected mode and provides a basic shell implemented in C++ rather than assembly.

## Architecture

SlopOS now features a proper bootloader that transitions from 16-bit real mode to 32-bit protected mode and loads a C++ kernel from disk. This represents a significant architectural upgrade from the previous assembly-only implementation.

### Boot Process
1. **16-bit Bootloader**: Sets up segments, loads kernel from disk
2. **Protected Mode Transition**: Enables A20 line, sets up GDT, enters 32-bit mode  
3. **C++ Kernel**: Shell and system functionality implemented in C++

## Building and Running

### Prerequisites
- NASM assembler
- GCC/G++ compiler 
- QEMU x86-64 emulator
- Make

### Quick Start
```bash
# Build and run the OS
./run.sh

# Or manually:
make all
make run
```

### Project Structure
- `boot/` - 16-bit bootloader assembly code with protected mode transition
- `src/` - C++ kernel source code
  - `kernel.cpp` - Main kernel and shell implementation
  - `terminal.cpp` - VGA text mode terminal driver
  - `string.cpp` - String manipulation functions
  - `timer.cpp` - Timer functionality using RDTSC
  - `kernel_entry.asm` - Assembly entry point for C++ kernel
  - `kernel.ld` - Linker script for kernel
- `build/` - Generated build artifacts
- `Makefile` - Build configuration for 32-bit protected mode kernel
- `run.sh` - Build and run script

## Features
- [x] Bootable 512-byte boot sector with protected mode transition
- [x] 32-bit protected mode operation  
- [x] C++ kernel architecture (migrated from assembly)
- [x] VGA text mode terminal driver
- [x] Welcome message displayed from C++ kernel
- [x] Interactive shell implemented in C++
- [x] `uptime` command - shows seconds since boot using RDTSC
- [x] `version` command - shows "SlopOS 1.0 - C++ kernel edition"
- [x] `help` command - shows available commands
- [x] Enhanced error handling and user feedback
- [ ] 64-bit long mode (future)
- [ ] Memory management (future)
- [ ] Process management (future)
