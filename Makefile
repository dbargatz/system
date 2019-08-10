AS       = nasm
ASFLAGS  = -felf64 
CPPFLAGS =
CXX      = clang++
CXXFLAGS = -ffreestanding -fno-builtin -nostdlib -nostdinc -march=x86-64 -g \
		   -mcmodel=large -mno-sse -D x64 -std=c++17 -Wall
LD       = ld

BUILD_DIR  = build
SRC_DIR    = src
ISO_DIR    = $(BUILD_DIR)/iso

GDB_SCRIPT = $(SRC_DIR)/debug/gdb_script_pre
GRUB_CFG  := $(SRC_DIR)/grub.cfg
ISO       := $(BUILD_DIR)/system.iso

include $(SRC_DIR)/kernel/Makefile.mk

.PHONY: all clean qemu qemu-debug iso

all: $(ISO)

clean:
	@rm -rf $(BUILD_DIR)

qemu: $(ISO)
	qemu-system-x86_64 -m 1G -smp 3 -display curses -cdrom $(ISO) -serial mon:stdio

qemu-debug: $(ISO)
	qemu-system-x86_64 -m 1G -smp 3 -display curses -cdrom $(ISO) -serial mon:stdio -s -S -d int -no-reboot

gdb:
	gdb -q -x $(GDB_SCRIPT)

iso: $(ISO)

$(ISO): $(KERNEL) $(GRUB_CFG)
	@mkdir -p $(ISO_DIR)/boot/grub
	@cp $(GRUB_CFG) $(ISO_DIR)/boot/grub
	@cp $(KERNEL) $(ISO_DIR)/boot
	@grub-mkrescue -o $(ISO) $(ISO_DIR) 2> /dev/null
