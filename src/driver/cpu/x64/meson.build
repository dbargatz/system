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
    'debug/serial.cpp',
    'display/vga.cpp',
    'interrupts/frame.cpp',
    'interrupts/idt.cpp',
    'interrupts/interrupt_manager.cpp',
    'interrupts/pic.cpp',
    'std/cpp.cpp',
    'std/logger.cpp',
    'std/memcpy.cpp',
    'std/memset.cpp',
    'std/text.cpp',
    'timer/pit.cpp',
    'core.cpp',
    'main.cpp',
]
clang_pp = find_program('clang++-7')
gen_clang = generator(clang_pp,
    output: '@BASENAME@.o',
    arguments: [
        '-include', '../src/driver/cpu/x64/std/cpp.hpp', 
        '-ffreestanding', 
        '-fno-builtin', 
        '-fno-rtti', 
        '-fno-unwind-tables',
        '-fno-exceptions',
        '-nostdlib', 
        '-nostdinc', 
        '-nostdinc++', 
        '-march=x86-64', 
        '-mcmodel=large', 
        '-g', 
        '-std=c++17', 
        '-Wall', 
        '-Werror',
        '-Wl,-r',
        '@INPUT@',
        '-o',
        '@OUTPUT@'
    ])
clang_objs = gen_clang.process(driver_cpu_x64_cpp_src)

core_x64_obj = custom_target('core_x64_obj',
    input: [clang_objs, nasm_objs],
    output: 'core_x64.o',
    command: ['ld', '-r', '-o', '@OUTPUT@', '@INPUT@'])

driver_cpu_x64_linker_src = '../src/driver/cpu/x64/linker.ld'
core_x64_bin = custom_target('core_x64_bin',
    input: core_x64_obj,
    output: '@BASENAME@.bin',
    command: ['ld', '-n', '--gc-sections', '-T', driver_cpu_x64_linker_src, '@INPUT@', '-o', '@OUTPUT@'],
    install: true,
    install_dir: 'core')
