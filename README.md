# system

A hobby multikernel inspired by Barrelfish<sup>[1](#barrelfish)</sup> and
seL4<sup>[2](#seL4)</sup>. It only supports x86_64 QEMU at the moment.

## Documentation

Doxygen-generated documentation is located at <https://dbargatz.me/system>.

## Environment

The `./configure` script assumes the following:

+ Debian-based Linux

The `./configure` script will install or upgrade the following:

+ clang++ 7.0.1 or newer
+ Doxygen 1.8.13 or newer
+ GDB 8.2.1 or newer
+ Git 2.20.1 or newer
+ Graphviz 2.40.1-6 or newer
+ Grub-common 2.02 or newer
+ Grub-pc-bin 2.02 or newer
+ less 487-0.1 or newer
+ [Meson](https://mesonbuild.com/index.html) 0.54.3 or newer
+ nasm 2.14-1 or newer
+ [Ninja](https://ninja-build.org/) 1.10.0 or newer
+ Pip 18.1-5 or newer
+ Python 3.7.3 or newer
+ QEMU 3.1.0 or newer
+ Xorriso 1.5.0-1 or newer

## Building

Normal debug build:

```bash
> ./configure
> meson build/artifacts
```

Generating Doxygen documentation (output to `docs/`):

```bash
> meson build/artifacts
> cd build/artifacts && ninja docs
```

## Running

Running QEMU with standard options and serial port output:

```bash
> test/run.sh
```

Running QEMU with interrupt debugging turned on and logged to
`test/artifacts/qemu.log`:

```bash
> test/run.sh --debug
```

Running QEMU with interrupt debugging turned on and logged to
`test/artifacts/qemu.log`, and GDB remote debugging:

```bash
> test/run.sh --gdb
> gdb src/debug/gdb_pre_script
(gdb) break kmain
(gdb) source src/debug/gdb_post_script
(gdb) continue
```

**NOTE:** GDB remote debugging will pause on startup. When you connect to QEMU via
GDB, issue a `continue` at the GDB prompt to continue.

<a name="barrelfish">1</a>: [The Barrelfish Operating System](http://www.barrelfish.org)<br/>
<a name="seL4">2</a>: [The seL4 Microkernel](https://sel4.systems)
