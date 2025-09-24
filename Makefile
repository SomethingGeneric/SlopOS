# SlopOS Makefile
# Builds operating system with Limine bootloader

ASM = nasm
CC = gcc
LD = ld

# Directories
BOOT_DIR = boot
SRC_DIR = src
BUILD_DIR = build
LIMINE_DIR = limine

# Output files - Original bootloader version
BOOTLOADER = $(BUILD_DIR)/bootloader.bin
KERNEL_ENTRY = $(BUILD_DIR)/kernel_entry.o
KERNEL_OBJECTS = $(BUILD_DIR)/kernel.o
KERNEL = $(BUILD_DIR)/kernel.bin
OS_IMAGE = $(BUILD_DIR)/slopos.img

# Output files - Limine version  
KERNEL_ELF = $(BUILD_DIR)/kernel.elf
LIMINE_OS_IMAGE = $(BUILD_DIR)/slopos-limine.img

# Compiler flags for 64-bit kernel (Limine)
CFLAGS_LIMINE = -m64 -ffreestanding -fno-stack-protector -fno-builtin -nostdlib -nostdinc -fno-pic -mcmodel=kernel -mno-red-zone

# Compiler flags for 32-bit kernel (original)
CFLAGS = -m32 -ffreestanding -fno-stack-protector -fno-builtin -nostdlib -nostdinc -fno-pic

# Linker flags  
LDFLAGS = -m elf_i386
LDFLAGS_LIMINE = -m elf_x86_64

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Limine bootloader targets
limine: $(LIMINE_OS_IMAGE)

# Build kernel for Limine (64-bit ELF)
$(KERNEL_ELF): $(BUILD_DIR)/entry_limine.o $(BUILD_DIR)/kernel_simple.o | $(BUILD_DIR)
	$(LD) $(LDFLAGS_LIMINE) -T $(SRC_DIR)/kernel_limine.ld -o $@ $^

# Compile Limine entry point
$(BUILD_DIR)/entry_limine.o: $(SRC_DIR)/entry_limine.asm | $(BUILD_DIR)
	$(ASM) -f elf64 $< -o $@

# Compile Limine kernel  
$(BUILD_DIR)/kernel_simple.o: $(SRC_DIR)/kernel_simple.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS_LIMINE) -c $< -o $@

# Compile C++ files for Limine (64-bit)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS_LIMINE) -c $< -o $@

# Create Limine OS image
$(LIMINE_OS_IMAGE): $(KERNEL_ELF) | $(BUILD_DIR)
	# Create a GPT disk image
	dd if=/dev/zero of=$@ bs=1M count=64
	# Create partition table  
	sgdisk $@ -n 1:2048 -t 1:ef00
	# Format the partition
	mformat -i $@@@1M -F -v "SLOPOSLIM" ::
	# Copy kernel
	mcopy -i $@@@1M $(KERNEL_ELF) ::/kernel.elf
	# Copy Limine config
	mcopy -i $@@@1M limine.cfg ::/limine.cfg
	# Install Limine bootloader
	$(LIMINE_DIR)/limine bios-install $@
	# Copy Limine binary
	mcopy -i $@@@1M $(LIMINE_DIR)/limine-bios.sys ::/limine-bios.sys

# Original bootloader targets (preserved for reference)
original: $(OS_IMAGE)

# Compile bootloader
$(BOOTLOADER): $(BOOT_DIR)/bootloader.asm | $(BUILD_DIR)
	$(ASM) -f bin $< -o $@

# Compile kernel entry
$(KERNEL_ENTRY): $(SRC_DIR)/kernel_entry.asm | $(BUILD_DIR)
	$(ASM) -f elf32 $< -o $@

# Compile kernel assembly files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.asm | $(BUILD_DIR)
	$(ASM) -f elf32 $< -o $@

# Compile kernel C files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link kernel
$(KERNEL): $(KERNEL_ENTRY) $(KERNEL_OBJECTS) | $(BUILD_DIR)
	$(LD) $(LDFLAGS) -T $(SRC_DIR)/kernel.ld -o $@.elf $^
	objcopy -O binary $@.elf $@

# Create OS image (bootloader + kernel)
$(OS_IMAGE): $(BOOTLOADER) $(KERNEL) | $(BUILD_DIR)
	cp $(BOOTLOADER) $(OS_IMAGE)
	# Pad bootloader to 512 bytes
	truncate -s 512 $(OS_IMAGE)
	# Append kernel
	cat $(KERNEL) >> $(OS_IMAGE)
	# Pad to sector boundary and add extra sectors for safety
	truncate -s %512 $(OS_IMAGE)
	truncate -s +1024 $(OS_IMAGE)

# Build everything (default to Limine)
all: limine

# Clean build files
clean:
	rm -rf $(BUILD_DIR)

# Run Limine version in QEMU
run: $(LIMINE_OS_IMAGE)
	qemu-system-x86_64 -drive format=raw,file=$(LIMINE_OS_IMAGE) -nographic

# Run Limine version in QEMU with graphics
run-gui: $(LIMINE_OS_IMAGE)
	qemu-system-x86_64 -drive format=raw,file=$(LIMINE_OS_IMAGE)

# Run original version in QEMU  
run-original: $(OS_IMAGE)
	qemu-system-x86_64 -drive format=raw,file=$(OS_IMAGE) -nographic

.PHONY: all clean run run-gui run-original limine original