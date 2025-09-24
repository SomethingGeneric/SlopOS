# SlopOS Makefile
# Builds a minimal x86 operating system

ASM = nasm
CC = gcc
LD = ld

# Directories
BOOT_DIR = boot
SRC_DIR = src
BUILD_DIR = build

# Output files
BOOTLOADER = $(BUILD_DIR)/bootloader.bin
KERNEL = $(BUILD_DIR)/kernel.o
OS_IMAGE = $(BUILD_DIR)/slopos.img

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile bootloader
$(BOOTLOADER): $(BOOT_DIR)/bootloader.asm | $(BUILD_DIR)
	$(ASM) -f bin $< -o $@

# Compile kernel (currently not used but ready for future expansion)
$(KERNEL): $(SRC_DIR)/kernel.cpp | $(BUILD_DIR)
	$(CC) -m32 -c -fno-stack-protector -fno-builtin -nostdlib -nostdinc -o $@ $<

# Create OS image (for now just the bootloader)
$(OS_IMAGE): $(BOOTLOADER) | $(BUILD_DIR)
	cp $(BOOTLOADER) $(OS_IMAGE)
	# Ensure the image is exactly 512 bytes (one sector)
	truncate -s 512 $(OS_IMAGE)

# Build everything
all: $(OS_IMAGE)

# Clean build files
clean:
	rm -rf $(BUILD_DIR)

# Run in QEMU
run: $(OS_IMAGE)
	qemu-system-x86_64 -drive format=raw,file=$(OS_IMAGE) -nographic

# Run in QEMU with graphics
run-gui: $(OS_IMAGE)
	qemu-system-x86_64 -drive format=raw,file=$(OS_IMAGE)

.PHONY: all clean run run-gui