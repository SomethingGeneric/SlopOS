#!/bin/bash

# SlopOS Build and Run Script  
# Compiles and runs SlopOS with GRUB bootloader in QEMU

set -e  # Exit on any error

echo "Building SlopOS with GRUB bootloader..."

# Clean previous build
make clean

# Build the OS ISO image
make all

echo "Build complete! Starting SlopOS in QEMU..."
echo "The system will boot to GRUB menu and automatically start SlopOS"
echo "Press Ctrl+A then X to exit QEMU"
echo "----------------------------------------"

# Run the OS in QEMU
# Using -nographic for headless operation
make run