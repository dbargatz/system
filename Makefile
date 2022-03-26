.PHONY: all %-build clean qemu_pc raspi3b

all: qemu_pc raspi3b

qemu_pc: SYSTEM_ARCHITECTURE=x64
qemu_pc: SYSTEM_NINJA_TARGET=system.iso
qemu_pc: SYSTEM_PLATFORM=qemu_pc
qemu_pc: qemu_pc-build
	@

raspi3b: SYSTEM_ARCHITECTURE=arm64
raspi3b: SYSTEM_NINJA_TARGET=kernel8.img
raspi3b: SYSTEM_PLATFORM=raspi3b
raspi3b: raspi3b-build
	@

%-build:
	meson setup ./build/${SYSTEM_PLATFORM}/ --buildtype debug --cross-file ./cross_base.ini --cross-file ./drivers/core/src/platform/${SYSTEM_PLATFORM}/platform.ini
	ninja -C ./build/${SYSTEM_PLATFORM} ${SYSTEM_NINJA_TARGET}

ASM_SRCS = drivers/core/src/platform/raspi3b/boot/start.asm drivers/core/src/platform/raspi3b/console/utils.asm
ASM_OBJS = $(ASM_SRCS:%.asm=build/raspi3b/%.o)
CPP_SRCS = drivers/core/src/platform/raspi3b/console/console.cpp drivers/core/src/platform/raspi3b/libcxx/abort.cpp drivers/core/src/libcxx/new.cpp drivers/core/src/memory/heap.cpp drivers/core/src/memory/manager.cpp drivers/core/src/main.cpp libs/libcxx/src/cstring.cpp libs/libcxx/src/cxa_atexit.cpp libs/libcxx/src/cxa_virtual.cpp libs/libcxx/src/memory_resource.cpp libs/libdevicetree/src/__utils.cpp libs/libdevicetree/src/fdt.cpp libs/libdevicetree/src/node.cpp libs/libdevicetree/src/property.cpp
CPP_OBJS = $(CPP_SRCS:%.cpp=build/raspi3b/%.o)

build/raspi3b/%.o : %.asm
	mkdir -p $(dir $@)
	/usr/bin/clang++-13 -flto=thin -fno-rtti -fpic -Wall --target=aarch64-elf64 -c $< -o $@

build/raspi3b/%.o : %.cpp
	mkdir -p $(dir $@)
	/usr/bin/clang++-13 -isystem libs/libcxx/src -mcmodel=small -fchar8_t -ffreestanding -flto=thin -fno-builtin -fno-exceptions -fno-rtti -fno-threadsafe-statics -fpic -nostdinc -nostdinc++ -std=c++2a -Wall -DPLATFORM_RASPI3B --target=aarch64-elf64 -c $< -o $@

kernel8.img: $(ASM_OBJS) $(CPP_OBJS)
	ld.lld-13 -m aarch64elf -nostdlib $(ASM_OBJS) $(CPP_OBJS) -T drivers/core/src/platform/raspi3b/linker.ld -o build/raspi3b/kernel8.elf
	llvm-objcopy-13 -O binary build/raspi3b/kernel8.elf build/raspi3b/kernel8.img

clean:
	rm -rf ./build
	rm -rf ~/*.log
