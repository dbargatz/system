# system

A hobby multikernel inspired by Barrelfish<sup>[1](#barrelfish)</sup> and
seL4<sup>[2](#seL4)</sup>. It only supports x86_64 QEMU at the moment.

## Project Layout

This project is loosely based on the [Pitchfork Layout](https://api.csswg.org/bikeshed/?force=1&url=https://raw.githubusercontent.com/vector-of-bool/pitchfork/develop/data/spec.bs):

+ `apps/`: user-mode programs that can be run by the operating system. Each
           subdirectory contains an individual application, which may be
           linked against one or more libraries.
+ `build/`: not checked into the repository, but generated at build-time.
            Contains intermediate artifacts and final binaries, as well as the
            ISO file to launch QEMU with.
+ `docs/`: contains datasheets, technical documentation, and generated Doxygen
           documentation.
+ `drivers/`: generally user-mode programs which interface with the core driver
              for the current architecture in order to manage a specific
              hardware device via system calls. This directory also contains
              the architecture-specific core drivers, which run in supervisor
              mode. Each subdirectory contains one or more drivers; for
              example, the `drivers/core` directory contains a subdirectory
              with a driver for each supported processor architecture.
+ `libs/`: static libraries that apps or drivers can link against and use. Each
           subdirectory contains an individual library.
+ `tests/`: contains the `run.sh` test script for running QEMU.
+ `tools/`: contains the build/test environment configure script, Dockerfile,
            and Doxygen configuration file.

## Environment

The `tools/Dockerfile` defines a complete build and test environment in a
Debian-stable Docker container. The container has the following software
installed, which is required to build and test the OS:

+ clang++ 13.0.0 or newer
+ Doxygen 1.9.1 or newer
+ GDB 10.1.90.20210103-git or newer
+ Git 2.30.2 or newer
+ Graphviz 2.42.2-5 or newer
+ Grub-common 2.04-20 or newer
+ Grub-pc-bin 2.04-20 or newer
+ less 551 or newer
+ [Meson](https://mesonbuild.com/index.html) 0.59.1 or newer
+ nasm 2.15.05 or newer
+ [Ninja](https://ninja-build.org/) 1.10.2 or newer
+ Pip 20.3.4 or newer
+ Python 3.9.2 or newer
+ QEMU 5.2.0 or newer
+ Xorriso 1.5.2 or newer

## Building

Normal debug build:

```bash
> meson build/ --buildtype debug --cross-file x64.ini
> ninja -C build/
```

Release build:

```bash
> meson build/ --buildtype release --cross-file x64.ini
> ninja -C build/
```

Generating Doxygen documentation (output to `docs/doxygen`):

```bash
> meson build/ --cross-file x64.ini
> ninja -C build/ docs
```

## Running

Running QEMU with standard options and serial port output:

```bash
> tests/run.sh
```

Running QEMU with interrupt debugging turned on and logged to
`~/qemu.log`:

```bash
> tests/run.sh --debug
```

Running QEMU with interrupt debugging turned on and logged to
`~/qemu.log`, and GDB remote debugging:

```bash
> tests/run.sh --gdb
> gdb -q
(gdb) source src/debug/gdb_pre_script
(gdb) continue
```

**NOTE:** GDB remote debugging will pause on startup. When you connect to QEMU via
GDB, issue a `continue` at the GDB prompt to continue.

<a name="barrelfish">1</a>: [The Barrelfish Operating System](http://www.barrelfish.org)<br/>
<a name="seL4">2</a>: [The seL4 Microkernel](https://sel4.systems)
