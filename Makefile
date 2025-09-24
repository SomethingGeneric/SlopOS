# SlopOS Makefile
# Builds operating system with GRUB bootloader

ASM = nasm
CC = gcc  
LD = ld

# Directories
SRC_DIR = src
BUILD_DIR = build

# Output files
KERNEL_ELF = $(BUILD_DIR)/multiboot_kernel.elf
OS_IMAGE = $(BUILD_DIR)/slopos.iso

# Compiler flags for 32-bit multiboot kernel
CFLAGS = -m32 -ffreestanding -fno-stack-protector -fno-builtin -nostdlib -nostdinc -fno-pic

# Linker flags
LDFLAGS = -m elf_i386

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Build multiboot kernel (32-bit ELF)
$(KERNEL_ELF): $(BUILD_DIR)/multiboot_entry.o $(BUILD_DIR)/multiboot_kernel.o | $(BUILD_DIR)
	$(LD) $(LDFLAGS) -T $(SRC_DIR)/multiboot.ld -o $@ $^

# Compile multiboot entry point
$(BUILD_DIR)/multiboot_entry.o: $(SRC_DIR)/multiboot_entry.asm | $(BUILD_DIR)
	$(ASM) -f elf32 $< -o $@

# Compile multiboot kernel
$(BUILD_DIR)/multiboot_kernel.o: $(SRC_DIR)/multiboot_kernel.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create GRUB ISO image
$(OS_IMAGE): $(KERNEL_ELF) | $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/iso/boot/grub
	cp $< $(BUILD_DIR)/iso/boot/kernel.elf
	echo 'set timeout=3' > $(BUILD_DIR)/iso/boot/grub/grub.cfg
	echo 'set default=0' >> $(BUILD_DIR)/iso/boot/grub/grub.cfg
	echo 'menuentry "SlopOS" {' >> $(BUILD_DIR)/iso/boot/grub/grub.cfg
	echo '    multiboot /boot/kernel.elf' >> $(BUILD_DIR)/iso/boot/grub/grub.cfg
	echo '}' >> $(BUILD_DIR)/iso/boot/grub/grub.cfg
	grub-mkrescue -o $@ $(BUILD_DIR)/iso

# Build everything
all: $(OS_IMAGE)

# Clean build files
clean:
	rm -rf $(BUILD_DIR)

# Run SlopOS in QEMU
run: $(OS_IMAGE)
	qemu-system-x86_64 -cdrom $(OS_IMAGE) -nographic

# Run SlopOS in QEMU with graphics
run-gui: $(OS_IMAGE)
	qemu-system-x86_64 -cdrom $(OS_IMAGE)

.PHONY: all clean run run-gui