.PHONY: build clean setup

build: setup
	ninja -C ./build/${SYSTEM_PLATFORM} ${SYSTEM_NINJA_TARGET}

build_qemu_pc: SYSTEM_ARCHITECTURE=x64
build_qemu_pc: SYSTEM_NINJA_TARGET=system.iso
build_qemu_pc: SYSTEM_PLATFORM=qemu-pc
build_qemu_pc: build
	@@

build_raspi3b: SYSTEM_ARCHITECTURE=arm64
build_raspi3b: SYSTEM_NINJA_TARGET=kernel8.img
build_raspi3b: SYSTEM_PLATFORM=raspi3b
build_raspi3b: build
	@@

clean:
	rm -rf ./build
	rm -rf ~/os_*.log
	rm -rf ~/qemu_*.log

setup:
	meson setup ./build/${SYSTEM_PLATFORM}/ --buildtype debug --cross-file ./cross_base.ini --cross-file ./drivers/core/${SYSTEM_ARCHITECTURE}/src/platform/${SYSTEM_PLATFORM}/platform.ini
