AS       = nasm
ASFLAGS  = -felf64 
CPPFLAGS =
CXX      = clang++-7
CXXFLAGS = -ffreestanding -fno-builtin -nostdlib -nostdinc -nostdinc++ \
		   -march=x86-64 -g -mcmodel=large -mno-sse -D x64 -std=c++17 \
		   -Wall -Werror
LD       = ld

BUILD_DIR  = build
SRC_DIR    = src
ISO_DIR    = $(BUILD_DIR)/iso

GDB_SCRIPT = $(SRC_DIR)/debug/gdb_script_pre
GRUB_CFG  := $(SRC_DIR)/grub.cfg
ISO       := $(BUILD_DIR)/system.iso
QEMU_LOG  := $(BUILD_DIR)/qemu-debug.log

include $(SRC_DIR)/driver/cpu/x64/Makefile.mk

.PHONY: all clean gdb iso qemu qemu-debug

all: $(ISO)

clean:
	@rm -rf $(BUILD_DIR)

gdb: $(GDB_SCRIPT)
	gdb -q -x $(GDB_SCRIPT)

iso: $(ISO)

qemu: $(ISO)
	qemu-system-x86_64 -m 1G -smp 3 -curses -cdrom $(ISO) -serial mon:stdio

qemu-debug: $(ISO)
	qemu-system-x86_64 -m 1G -smp 3 -curses -cdrom $(ISO) -serial mon:stdio -d int -D $(QEMU_LOG) -no-reboot

qemu-gdb: $(ISO)
	qemu-system-x86_64 -m 1G -smp 3 -nographic -cdrom $(ISO) -d int -D $(QEMU_LOG) -no-reboot -s -S

$(ISO): $(DRIVER_BIN) $(GRUB_CFG)
	@mkdir -p $(ISO_DIR)/boot/grub
	@cp $(GRUB_CFG) $(ISO_DIR)/boot/grub
	@cp $(DRIVER_BIN) $(ISO_DIR)/boot
	@grub-mkrescue -o $(ISO) $(ISO_DIR)
