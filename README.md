# system
A hobby microkernel operating system. Currently written in C, it runs only on QEMU.

## Building
___
Currently, the only supported build process is on Windows 10 with Windows Subsystem for Linux. In the future, this will change to include multiple Linux distributions.
### Windows 10 with Windows Subsystem for Linux
The primary development system is on Windows 10 using the Windows Subsystem for Linux; this is effectively Ubuntu running on top of Windows, so it should transfer easily to Ubuntu 16.04 and up.
#### Development Environment
This is how my development environment is configured on Windows 10. This is divided into two sections: software that must be installed on Windows 10 itself, and packages that must be installed in the Windows Subsystem for Linux environment.
##### Software On Windows 10
The following software must be installed:
+ [Windows Subsystem for Linux (WSL)](https://docs.microsoft.com/en-us/windows/wsl/install-win10)
  + If using Windows 10 Fall Creators' Update or later, use Ubuntu as the distribution.
+ [Git for Windows](https://git-scm.com/download/) configured to commit Unix-style line endings
+ [Visual Studio Code](https://code.visualstudio.com/) with Terminal set to WSL Bash
+ [Xming Server](http://www.straightrunning.com/XmingNotes/)
##### Software on Windows Subsystem for Linux
These can all be installed via sudo apt-get \<package-name\>.
+ clang (which should install LLVM along with it)
+ grub-mkrescue (in package grub-common)
+ grub-pc-bin
+ ld
+ make
+ nasm
+ qemu-system-x86_64
+ xorriso

## Running
___
At the moment, there is no source code, so there is no running!
