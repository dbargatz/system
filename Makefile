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

clean:
	rm -rf ./build
	rm -rf ~/*.log
