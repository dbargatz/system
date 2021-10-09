.PHONY: all %-build %-set-env clean

all: qemu-pc raspi3b

qemu-pc: SYSTEM_ARCHITECTURE=x64
qemu-pc: SYSTEM_NINJA_TARGET=system.iso
qemu-pc: SYSTEM_PLATFORM=qemu-pc
qemu-pc: qemu-pc-build
	@

raspi3b: SYSTEM_ARCHITECTURE=arm64
raspi3b: SYSTEM_NINJA_TARGET=kernel8.img
raspi3b: SYSTEM_PLATFORM=raspi3b
raspi3b: raspi3b-build
	@

%-build:
	meson setup ./build/${SYSTEM_PLATFORM}/ --buildtype debug --cross-file ./cross_base.ini --cross-file ./drivers/core/${SYSTEM_ARCHITECTURE}/src/platform/${SYSTEM_PLATFORM}/platform.ini
	ninja -C ./build/${SYSTEM_PLATFORM} ${SYSTEM_NINJA_TARGET}

clean:
	rm -rf ./build
	rm -rf ~/os_*.log
	rm -rf ~/qemu_*.log
