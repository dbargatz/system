AS = nasm
ASFLAGS = -felf64 
CPPFLAGS =
CXX = clang
CXXFLAGS = -ffreestanding -fno-builtin -nostdlib -nostdinc -march=x86-64 -g -mcmodel=large -mno-sse -D x64
LD = ld

BUILD_DIR = ./temp
SRC_DIR = ../src

iso := $(BUILD_DIR)/system.iso
iso_dir := $(BUILD_DIR)/isofiles

grub_cfg := run/grub.cfg

include $(SRC_DIR)/kernel/Makefile.mk

.PHONY: all clean run iso directories

all: directories $(iso)

clean:
	@rm -r $(BUILD_DIR)

directories: 
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(KERNEL_BUILD_DIR)

qemu: $(iso)
	qemu-system-x86_64 -m 1G -smp 3 -display curses -cdrom $(iso)

qemu-debug: $(iso)
	qemu-system-x86_64 -m 1G -smp 3 -display curses -cdrom $(iso) -s -S -d int -no-reboot 

iso: $(iso)

$(iso): directories $(KERNEL_ELF) $(grub_cfg)
	@mkdir -p $(iso_dir)/boot/grub
	@cp $(KERNEL_ELF) $(iso_dir)/boot/kernel.bin
	@cp $(grub_cfg) $(iso_dir)/boot/grub
	@grub-mkrescue -o $(iso) $(iso_dir) 2> /dev/null
	@rm -r $(iso_dir)
