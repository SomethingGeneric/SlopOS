# SlopOS
x64 OS mostly developed by Copilot

A minimal C++ operating system that boots to real mode and displays "hello from SlopOS".

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
- [x] "hello from SlopOS" message display
- [ ] Protected mode (future)
- [ ] C++ kernel integration (future)
