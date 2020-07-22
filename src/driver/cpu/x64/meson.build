nasm = find_program('nasm')
gen_nasm = generator(nasm,
    output: '@BASENAME@.o',
    arguments: ['-f', 'elf64', '@INPUT@', '-o', '@OUTPUT@'])
driver_cpu_x64_asm_src = [
    'interrupts/handlers.asm',
    'multiboot/multiboot2_header.asm',
    'start.asm'
]
nasm_objs = gen_nasm.process(driver_cpu_x64_asm_src)

driver_cpu_x64_cpp_src = [
    'debug/uart.cpp',
    'debug/uart_logger.cpp',
    'display/vga.cpp',
    'display/vga_logger.cpp',
    'interrupts/gdt.cpp',
    'interrupts/frame.cpp',
    'interrupts/idt.cpp',
    'interrupts/pic.cpp',
    'interrupts/tss.cpp',
    'keyboard/keystate.cpp',
    'keyboard/ps2_controller.cpp',
    'keyboard/ps2_keyboard.cpp',
    'keyboard/scancode_set_2.cpp',
    'multiboot/boot_info.cpp',
    'ports/io_port.cpp',
    'std/cpp.cpp',
    'std/logger.cpp',
    'std/memcpy.cpp',
    'std/memset.cpp',
    'std/string.cpp',
    'std/text.cpp',
    'timer/pit.cpp',
    'core.cpp',
    'main.cpp',
]
clang_pp = find_program('clang++')
gen_clang = generator(clang_pp,
    output: '@BASENAME@.o',
    arguments: [
        '-include', '../../src/driver/cpu/x64/std/cpp.hpp',
        '-ffreestanding',
        '-fno-builtin',
        '-fno-exceptions',
        '-fno-rtti',
        '-fno-threadsafe-statics',
        '-nostdlib',
        '-nostdinc',
        '-nostdinc++',
        '-march=x86-64',
        '-mcmodel=large',
        '-g',
        '-std=c++17',
        '-Wall',
        '-Werror',
        '-c',
        '@INPUT@',
        '-o',
        '@OUTPUT@'
    ])
clang_objs = gen_clang.process(driver_cpu_x64_cpp_src)

core_x64_obj = custom_target('core_x64_obj',
    input: [clang_objs, nasm_objs],
    output: 'core_x64.o',
    command: ['ld', '-r', '-o', '@OUTPUT@', '@INPUT@'])

driver_cpu_x64_linker_src = '../../src/driver/cpu/x64/linker.ld'
core_x64_bin = custom_target('core_x64_bin',
    input: core_x64_obj,
    output: '@BASENAME@.bin',
    command: ['ld', '-n', '--gc-sections', '-T', driver_cpu_x64_linker_src, '@INPUT@', '-o', '@OUTPUT@'])
