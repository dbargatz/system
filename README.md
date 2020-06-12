# system

A hobby multikernel operating system. Currently written in C, it runs only on x86_64 QEMU.

## Building

The build host is Ubuntu 18.04; system does not currently host a compiler and cannot compile itself.

### Development Environment

Building and running takes place in an Ubuntu 18.04 VM hosted in Hyper-V on a Windows 10 Professional host. Editing is done via [VS Code](https://code.visualstudio.com/) using the [Remote Development extension over SSH](https://code.visualstudio.com/docs/remote/ssh#_getting-started) to connect to the Ubuntu 18.04 VM and edit the source directly on the VM.

### Required Packages

These can all be installed via `sudo apt install <package-name>`.

+ clang (which should install LLVM along with it)
+ grub-common (for grub-mkrescue; likely already installed)
+ grub-pc-bin
+ make
+ nasm
+ qemu-system-x86 (for qemu-system-x86_64)
+ qemu-system-arm (for qemu-system-aarch64)
+ xorriso

## Running

On the Ubuntu 18.04 build VM, issue a `make qemu` in the `system/build` directory. A qemu window should appear, with `OK NTR`
in white text on a red background; this indicates the kernel is safely in 64-bit mode!
