# SlopOS
Minimal operating system built in C/C++

Use resources from people who've done this before when implementing new features or chasing bugs.

## If you're refactoring in a major way,
make sure to check the run.sh and makefile for any arguements that might need to be changed

## Some good resources:
* OS development wiki - https://wiki.osdev.org/Expanded_Main_Page

Please try testing your changes where possible, using the run script and/or Makefile, but don't be afraid to ask the user to test with a full desktop system if appropriate.

## TIMEOUT
Please don't worry about long runtimes. If you're debugging something yourself, it might take a while to build or to see results. Be patient, and feel free to update the user without exiting. They will be happy for progress reports even if it's taking you a while to solve the problem.

## HEADLESS DEVELOPMENT ENVIRONMENT
The Copilot development environment is headless and cannot display graphical output from QEMU. When working on SlopOS:

* A job should be considered **complete** as long as the project **builds successfully** (make all succeeds)
* If there are issues with running the OS (make run fails or times out), this can be **ignored** in the headless environment
* Focus on ensuring code compiles, links, and produces valid build artifacts (ISO files, ELF binaries)
* The user can provide feedback if there are logic errors or graphical problems when testing on their desktop system
* Use `make run` with timeouts for basic smoke testing, but don't expect interactive functionality in CI

## REQUIRED DEVELOPMENT PACKAGES
The following packages are pre-installed in the GitHub Actions environment for SlopOS development:
* `gcc` - C/C++ compiler
* `nasm` - Netwide Assembler
* `make` - Build automation tool
* `xorriso` - ISO 9660 filesystem utilities
* `grub-pc-bin` - GRUB bootloader binaries
* `grub-common` - GRUB common files
* `mtools` - Tools for accessing MS-DOS disks
* `qemu-system-x86` - x86 system emulator
* `qemu-system-i386` - i386 system emulator  
* `libc6-dev-i386` - 32-bit development libraries
* `build-essential` - Essential build tools
