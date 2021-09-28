drivers_core_arm64_asm_src = [
    'src/start.s',
]

aarch64_assembler = find_program('clang-13')
gen_aarch64_asm = generator(aarch64_assembler,
    output: '@BASENAME@.o',
    arguments: ['-target', 'aarch64', '-c', '@INPUT@', '-o', '@OUTPUT@'])
drivers_core_arm64_asm_src = [
    'src/start.asm',
]
asm_objs = gen_aarch64_asm.process(drivers_core_arm64_asm_src)

drivers_core_arm64_src = [
    'src/main.cpp',
]

drivers_core_arm64_link_script = '../../drivers/core/arm64/linker.ld'
link_with_objs = [libcxx_lib]
core_arm64_bin = executable('core_arm64.bin', [drivers_core_arm64_src, asm_objs],
                          link_args: ['-Wl,-T' + drivers_core_arm64_link_script],
                          link_with: link_with_objs)
