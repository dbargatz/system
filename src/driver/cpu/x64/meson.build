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

driver_cpu_x64_src = [
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
    'std/text.cpp',
    'timer/pit.cpp',
    'core.cpp',
    'main.cpp',
]

driver_cpu_x64_link_script = '../src/driver/cpu/x64/linker.ld'
link_with_objs = [abi_lib, loader_lib, logging_lib, std_lib]
core_x64_bin = executable('core_x64.bin', [driver_cpu_x64_src, nasm_objs],
                          link_args: ['-Wl,-T' + driver_cpu_x64_link_script],
                          link_with: link_with_objs)
