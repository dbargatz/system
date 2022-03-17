# Generic source code and static libraries used for all platforms. --------------------------------
core_cpp_src = [
    'src/libcxx/new.cpp',
    'src/memory/heap.cpp',
    'src/memory/manager.cpp',
    'src/main.cpp'
]
core_linker_objs = [libcxx_lib, libdevicetree_lib]


# Meson generators to compile assembly for different processor architectures. ---------------------
# arm64_assembler = find_program('clang-13')
arm64_assembler = generator(find_program('clang-13'),
    output: '@BASENAME@.o',
    arguments: ['-target', 'aarch64', '-c', '@INPUT@', '-o', '@OUTPUT@'])

# x64_assembler = find_program('nasm')
x64_assembler = generator(find_program('nasm'),
    output: '@BASENAME@.o',
    arguments: ['-f', 'elf64', '@INPUT@', '-o', '@OUTPUT@'])


# Build for the QEMU-PC target. -------------------------------------------------------------------
core_qemu_pc_asm_src = [
    'src/platform/qemu_pc/boot/multiboot2_header.asm',
    'src/platform/qemu_pc/boot/start.asm',
]
core_qemu_pc_cpp_src = core_cpp_src + [
    'src/platform/qemu_pc/console/console.cpp',
    'src/platform/qemu_pc/libcxx/abort.cpp',
    'src/platform/qemu_pc/ports/io_port.cpp',
]

core_qemu_pc_linker_script = '../../drivers/core/src/platform/qemu_pc/linker.ld'
core_qemu_pc_asm_objs = x64_assembler.process(core_qemu_pc_asm_src)
core_qemu_pc_bin = executable('core_qemu_pc.bin', [core_qemu_pc_cpp_src, core_qemu_pc_asm_objs],
                          link_args: ['-Wl,-T' + core_qemu_pc_linker_script],
                          link_with: core_linker_objs)


# Build for the Raspberry Pi 3b target. -----------------------------------------------------------
core_raspi3b_asm_src = [
    'src/platform/raspi3b/boot/start.asm',
    'src/platform/raspi3b/console/utils.asm',
]
core_raspi3b_cpp_src = core_cpp_src + [
    'src/platform/raspi3b/console/console.cpp',
    'src/platform/raspi3b/libcxx/abort.cpp',
]

core_raspi3b_linker_script = '../../drivers/core/src/platform/raspi3b/linker.ld'
core_raspi3b_asm_objs = arm64_assembler.process(core_raspi3b_asm_src)
core_raspi3b_bin = executable('core_raspi3b.bin', [core_raspi3b_cpp_src, core_raspi3b_asm_objs],
                          link_args: ['-Wl,-T' + core_raspi3b_linker_script],
                          link_with: core_linker_objs)
