# SlopOS
x64 OS mostly developed by Copilot

A minimal C++ operating system that boots to real mode and provides a basic shell with welcome message from the kernel.

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
- `boot/` - Bootloader assembly code
- `src/` - C++ kernel source code (for future expansion)
- `build/` - Generated build artifacts
- `Makefile` - Build configuration
- `run.sh` - Build and run script

## Features
- [x] Bootable 512-byte boot sector
- [x] Real mode operation
- [x] BIOS text output
- [x] Welcome message displayed from kernel
- [x] Basic interactive shell
- [x] `uptime` command - shows seconds since boot
- [x] `version` command - shows "SlopOS 1.0"
- [x] Unknown command error handling
- [ ] Protected mode (future)
- [ ] C++ kernel integration (future)
