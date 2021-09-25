nasm = find_program('nasm')
gen_nasm = generator(nasm,
    output: '@BASENAME@.o',
    arguments: ['-f', 'elf64', '@INPUT@', '-o', '@OUTPUT@'])
driver_cpu_x64_asm_src = [
    'src/interrupts/handlers.asm',
    'src/multiboot/multiboot2_header.asm',
    'src/start.asm'
]
nasm_objs = gen_nasm.process(driver_cpu_x64_asm_src)

driver_cpu_x64_src = [
    'src/interrupts/gdt.cpp',
    'src/interrupts/frame.cpp',
    'src/interrupts/idt.cpp',
    'src/interrupts/pic.cpp',
    'src/interrupts/tss.cpp',
    'src/logging/uart.cpp',
    'src/logging/uart_logger.cpp',
    'src/memory/new.cpp',
    'src/memory/paging.cpp',
    'src/multiboot/boot_info.cpp',
    'src/ports/io_port.cpp',
    'src/timer/pit.cpp',
    'src/core.cpp',
    'src/main.cpp',
]

driver_cpu_x64_link_script = '../drivers/cpu/x64/linker.ld'
link_with_objs = [libcxx_lib]
core_x64_bin = executable('core_x64.bin', [driver_cpu_x64_src, nasm_objs],
                          link_args: ['-Wl,-T' + driver_cpu_x64_link_script],
                          link_with: link_with_objs)
