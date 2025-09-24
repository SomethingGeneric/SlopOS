#!/bin/bash

# SlopOS Build and Run Script
# Compiles and runs SlopOS in QEMU x86-64 emulator

set -e  # Exit on any error

echo "Building SlopOS..."

# Clean previous build
make clean

# Build the OS image
make all

echo "Build complete! Starting SlopOS in QEMU..."
echo "Press Ctrl+A then X to exit QEMU"
echo "----------------------------------------"

# Run the OS in QEMU
# Using -nographic for headless operation
# Add -monitor stdio for debugging if needed
make run