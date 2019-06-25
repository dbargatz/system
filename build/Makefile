build_dir := temp
os := $(build_dir)/system.a
kernel = $(build_dir)/system.elf
iso := $(build_dir)/system.iso
iso_dir := $(build_dir)/isofiles
target ?= x64-unknown-linux-gnu

grub_cfg := run/grub.cfg
linker_script := linker.ld
include_dir := ../src

kernel_asm_files = $(shell find ../src/kernel -name *.asm)
kernel_source_files = $(shell find ../src/kernel -name *.c)
kernel_build_dir = $(build_dir)/kernel
kernel_include_dir = $(include_dir)/kernel
kernel_obj = $(kernel_build_dir)/kernel.o

.PHONY: all clean run iso

all: $(iso)

clean:
	@rm -r build

qemu: $(iso)
	qemu-system-x86_64 -m 1G -smp 3 -cdrom $(iso)

qemu-debug: $(iso)
	qemu-system-x86_64 -m 1G -smp 3 -s -S -d int -no-reboot -cdrom $(iso)

iso: $(iso)

$(iso): $(kernel) $(grub_cfg)
	@mkdir -p $(iso_dir)/boot/grub
	@cp $(kernel) $(iso_dir)/boot/kernel.bin
	@cp $(grub_cfg) $(iso_dir)/boot/grub
	@grub-mkrescue -o $(iso) $(iso_dir) 2> /dev/null
	@rm -r $(iso_dir)

$(kernel): $(kernel_obj)
	@ld -n --gc-sections -T $(linker_script) -o $(kernel) $(kernel_obj)

$(kernel_obj): $(kernel_asm_files) $(kernel_source_files)
	@mkdir -p $(kernel_build_dir)
	@for f in $(kernel_asm_files); do \
		nasm -felf64 $$f -o $(kernel_build_dir)/$$(basename $$f).o; \
		done
	@for f in $(kernel_source_files); do \
		clang -ffreestanding -fno-builtin -nostdlib -nostdinc -march=x86-64 -g -mcmodel=large -mno-sse -D x64 -I$(kernel_include_dir) -c $$f -o $(kernel_build_dir)/$$(basename $$f).o; \
		done
	@ld -r -o $(kernel_obj) $(kernel_build_dir)/*.o
