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
$(KERNEL_ELF): $(BUILD_DIR)/multiboot_entry.o $(BUILD_DIR)/multiboot_kernel.o $(BUILD_DIR)/terminal.o $(BUILD_DIR)/string.o $(BUILD_DIR)/timer.o $(BUILD_DIR)/memory.o $(BUILD_DIR)/process.o $(BUILD_DIR)/context_switch.o $(BUILD_DIR)/syscall.o $(BUILD_DIR)/shell.o $(BUILD_DIR)/filesystem.o $(BUILD_DIR)/command_loader.o $(BUILD_DIR)/version_process.o $(BUILD_DIR)/hello_process.o $(BUILD_DIR)/help_process.o $(BUILD_DIR)/ps_process.o $(BUILD_DIR)/version.o $(BUILD_DIR)/hello.o $(BUILD_DIR)/ps.o $(BUILD_DIR)/uptime.o $(BUILD_DIR)/memory_cmd.o $(BUILD_DIR)/yield.o $(BUILD_DIR)/help.o | $(BUILD_DIR)
	$(LD) $(LDFLAGS) -T $(SRC_DIR)/multiboot.ld -o $@ $^

# Compile multiboot entry point
$(BUILD_DIR)/multiboot_entry.o: $(SRC_DIR)/multiboot_entry.asm | $(BUILD_DIR)
	$(ASM) -f elf32 $< -o $@

# Compile multiboot kernel
$(BUILD_DIR)/multiboot_kernel.o: $(SRC_DIR)/multiboot_kernel.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile terminal driver
$(BUILD_DIR)/terminal.o: $(SRC_DIR)/terminal.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile string functions
$(BUILD_DIR)/string.o: $(SRC_DIR)/string.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile timer functions
$(BUILD_DIR)/timer.o: $(SRC_DIR)/timer.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile memory management
$(BUILD_DIR)/memory.o: $(SRC_DIR)/memory.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile process management
$(BUILD_DIR)/process.o: $(SRC_DIR)/process.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile context switching
$(BUILD_DIR)/context_switch.o: $(SRC_DIR)/context_switch.asm | $(BUILD_DIR)
	$(ASM) -f elf32 $< -o $@

# Compile system calls
$(BUILD_DIR)/syscall.o: $(SRC_DIR)/syscall.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile shell
$(BUILD_DIR)/shell.o: $(SRC_DIR)/shell.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile filesystem
$(BUILD_DIR)/filesystem.o: $(SRC_DIR)/filesystem.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile command loader
$(BUILD_DIR)/command_loader.o: $(SRC_DIR)/command_loader.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile process-based commands
$(BUILD_DIR)/version_process.o: $(SRC_DIR)/commands/version_process.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/hello_process.o: $(SRC_DIR)/commands/hello_process.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/help_process.o: $(SRC_DIR)/commands/help_process.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/ps_process.o: $(SRC_DIR)/commands/ps_process.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile command modules
$(BUILD_DIR)/version.o: $(SRC_DIR)/commands/version.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/hello.o: $(SRC_DIR)/commands/hello.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/ps.o: $(SRC_DIR)/commands/ps.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/uptime.o: $(SRC_DIR)/commands/uptime.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/memory_cmd.o: $(SRC_DIR)/commands/memory.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/yield.o: $(SRC_DIR)/commands/yield.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/help.o: $(SRC_DIR)/commands/help.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Test kernel build target
test: $(BUILD_DIR)/multiboot_entry.o $(BUILD_DIR)/test_kernel.o $(BUILD_DIR)/terminal.o $(BUILD_DIR)/string.o | $(BUILD_DIR)
	$(LD) $(LDFLAGS) -T $(SRC_DIR)/multiboot.ld -o $(BUILD_DIR)/test_kernel.elf $^
	mkdir -p $(BUILD_DIR)/iso/boot/grub
	cp $(BUILD_DIR)/test_kernel.elf $(BUILD_DIR)/iso/boot/kernel.elf
	echo 'set timeout=3' > $(BUILD_DIR)/iso/boot/grub/grub.cfg
	echo 'set default=0' >> $(BUILD_DIR)/iso/boot/grub/grub.cfg
	echo 'menuentry "SlopOS Test" {' >> $(BUILD_DIR)/iso/boot/grub/grub.cfg
	echo '    multiboot /boot/kernel.elf' >> $(BUILD_DIR)/iso/boot/grub/grub.cfg
	echo '}' >> $(BUILD_DIR)/iso/boot/grub/grub.cfg
	grub-mkrescue -o $(BUILD_DIR)/test.iso $(BUILD_DIR)/iso

# Compile test kernel
$(BUILD_DIR)/test_kernel.o: $(SRC_DIR)/test_kernel.cpp | $(BUILD_DIR)
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

# Default target for faster local testing
.DEFAULT_GOAL := run-gui

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