# SlopOS Makefile
# Builds a 64-bit x86 operating system

ASM = nasm
CC = gcc
LD = ld

# Directories
BOOT_DIR = boot
SRC_DIR = src
BUILD_DIR = build

# Output files
BOOTLOADER = $(BUILD_DIR)/bootloader.bin
KERNEL_ENTRY = $(BUILD_DIR)/kernel_entry.o
KERNEL_OBJECTS = $(BUILD_DIR)/kernel.o $(BUILD_DIR)/terminal.o $(BUILD_DIR)/string.o $(BUILD_DIR)/timer.o
KERNEL = $(BUILD_DIR)/kernel.bin
OS_IMAGE = $(BUILD_DIR)/slopos.img

# Compiler flags for 32-bit kernel
CFLAGS = -m32 -ffreestanding -fno-stack-protector -fno-builtin -nostdlib -nostdinc -fno-pic

# Linker flags
LDFLAGS = -m elf_i386

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile bootloader
$(BOOTLOADER): $(BOOT_DIR)/bootloader.asm | $(BUILD_DIR)
	$(ASM) -f bin $< -o $@

# Compile kernel entry
$(KERNEL_ENTRY): $(SRC_DIR)/kernel_entry.asm | $(BUILD_DIR)
	$(ASM) -f elf32 $< -o $@

# Compile kernel C++ files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link kernel
$(KERNEL): $(KERNEL_ENTRY) $(KERNEL_OBJECTS) | $(BUILD_DIR)
	$(LD) $(LDFLAGS) -T $(SRC_DIR)/kernel.ld -o $@ $^

# Create OS image (bootloader + kernel)
$(OS_IMAGE): $(BOOTLOADER) $(KERNEL) | $(BUILD_DIR)
	cp $(BOOTLOADER) $(OS_IMAGE)
	# Pad bootloader to 512 bytes
	truncate -s 512 $(OS_IMAGE)
	# Append kernel
	cat $(KERNEL) >> $(OS_IMAGE)
	# Pad to sector boundary
	truncate -s %512 $(OS_IMAGE)

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