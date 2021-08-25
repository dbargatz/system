# system

A hobby multikernel inspired by Barrelfish<sup>[1](#barrelfish)</sup> and
seL4<sup>[2](#seL4)</sup>. It only supports x86_64 QEMU at the moment.

## Project Layout

This project is being refactored to conform to the [Pitchfork Layout](https://api.csswg.org/bikeshed/?force=1&url=https://raw.githubusercontent.com/vector-of-bool/pitchfork/develop/data/spec.bs):

+ `build/`: not checked into the repository, but generated at build-time.
            Contains intermediate artifacts and final binaries, as well as the
            ISO file to launch QEMU with.
+ `docs/`: contains datasheets, technical documentation, and generated Doxygen
           documentation.
+ `src/`: follows the "merged header placement" and "merged test placement"
          paradigms, meaning all header, source, and unit test files are located
          in this directory. There is no separate `include/` directory for
          public API headers; these are contained within the `src/` directory as
          well. The directory structure within `src/` aligns perfectly with the
          namespaces of the code they contain.
+ `tests/`: contains the `run.sh` test script for running QEMU.
+ `tools/`: contains the build/test environment configure script, Dockerfile,
            and Doxygen configuration file.

If you find something that doesn't conform yet, please point it out! It may have
slipped notice.

## Documentation

Doxygen-generated documentation is located at <https://dbargatz.me/system>.

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
