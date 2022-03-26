.PHONY: all %-build clean qemu_pc raspi3b

ASM_OBJS = $(PLATFORM_ASM_SRCS:%.asm=$(BUILD_ROOT_DIR)/%.o)
CPP_SRCS = drivers/core/src/libcxx/new.cpp drivers/core/src/memory/heap.cpp drivers/core/src/memory/manager.cpp drivers/core/src/main.cpp libs/libcxx/src/cstring.cpp libs/libcxx/src/cxa_atexit.cpp libs/libcxx/src/cxa_virtual.cpp libs/libcxx/src/memory_resource.cpp libs/libdevicetree/src/__utils.cpp libs/libdevicetree/src/fdt.cpp libs/libdevicetree/src/node.cpp libs/libdevicetree/src/property.cpp
CPP_OBJS = $(PLATFORM_CPP_SRCS:%.cpp=$(BUILD_ROOT_DIR)/%.o) $(CPP_SRCS:%.cpp=$(BUILD_ROOT_DIR)/%.o)

all: qemu_pc raspi3b

build:
	@mkdir -p $(BUILD_ROOT_DIR)
	@bear --output $(BUILD_ROOT_DIR)/compile_commands.json -- $(MAKE) --no-print-directory $(PLATFORM_NAME)-build

qemu_pc: SYSTEM_ARCHITECTURE=x64
qemu_pc: SYSTEM_NINJA_TARGET=system.iso
qemu_pc: SYSTEM_PLATFORM=qemu_pc
qemu_pc: qemu_pc-build


qemu_pc-build:
	@meson setup ./build/${SYSTEM_PLATFORM}/ --buildtype debug --cross-file ./cross_base.ini --cross-file ./drivers/core/src/platform/${SYSTEM_PLATFORM}/platform.ini
	@ninja -C ./build/${SYSTEM_PLATFORM} ${SYSTEM_NINJA_TARGET}

raspi3b: export PLATFORM_NAME=raspi3b
raspi3b: export PLATFORM_ARCH=aarch64-elf64
raspi3b: export PLATFORM_LD_ARCH=aarch64elf
raspi3b: export BUILD_ROOT_DIR=build/$(PLATFORM_NAME)
raspi3b: export PLATFORM_ASM_SRCS=drivers/core/src/platform/$(PLATFORM_NAME)/boot/start.asm drivers/core/src/platform/$(PLATFORM_NAME)/console/utils.asm
raspi3b: export PLATFORM_CPP_SRCS=drivers/core/src/platform/$(PLATFORM_NAME)/console/console.cpp drivers/core/src/platform/$(PLATFORM_NAME)/libcxx/abort.cpp
raspi3b: clean build

raspi3b-build: $(ASM_OBJS) $(CPP_OBJS)
	@ld.lld-13 -m $(PLATFORM_LD_ARCH) -nostdlib $(ASM_OBJS) $(CPP_OBJS) -T drivers/core/src/platform/$(PLATFORM_NAME)/linker.ld -o $(BUILD_ROOT_DIR)/kernel8.elf
	@llvm-objcopy-13 -O binary $(BUILD_ROOT_DIR)/kernel8.elf $(BUILD_ROOT_DIR)/kernel8.img

$(BUILD_ROOT_DIR)/%.o : %.asm
	@mkdir -p $(dir $@)
	/usr/bin/clang++-13 -flto=thin -fno-rtti -fpic -Wall --target=$(PLATFORM_ARCH) -c $< -o $@

$(BUILD_ROOT_DIR)/%.o : %.cpp
	@mkdir -p $(dir $@)
	/usr/bin/clang++-13 -g -isystem libs/libcxx/src -mcmodel=small -fchar8_t -ffreestanding -flto=thin -fno-builtin -fno-exceptions -fno-rtti -fno-threadsafe-statics -fpic -nostdinc -nostdinc++ -std=c++2a -Wall -DPLATFORM_RASPI3B --target=$(PLATFORM_ARCH) -c $< -o $@

clean:
	@rm -rf ./build
	@rm -rf ~/*.log
