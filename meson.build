project('system', 'cpp', version: '0.1.0')

# Libraries
subdir('libs/libcxx')
subdir('libs/libdevicetree')
subdir('libs/libsystem')

# Drivers
subdir('drivers/core')
subdir('drivers/ps2_keyboard')
subdir('drivers/vga')

# Applications
subdir('apps/monitor')

grub_cfg = '../../drivers/core/src/platform/qemu_pc/boot/grub.cfg'
qemu_pc = custom_target('qemu_pc',
    input: [core_qemu_pc_bin],
    output: 'system.iso',
    command: ['mkdir', '-p', 'iso/boot/grub', '&&',
              'cp', grub_cfg, 'iso/boot/grub', '&&',
              'cp', '@INPUT@', 'iso/boot', '&&',
              'grub-mkrescue', '--quiet', '-o', '@OUTPUT@', 'iso'],
    install: true,
    install_dir: 'build/qemu_pc')

raspi3b = custom_target('raspi3b',
    input: [core_raspi3b_bin],
    output: 'kernel8.img',
    command: ['llvm-objcopy', '-O', 'binary', '@INPUT@', '@OUTPUT@'],
    install: true,
    install_dir: 'build/raspi3b')

doxygen = find_program('doxygen')
doxygen_data = configuration_data()
doxygen_data.set('TOP_SRCDIR', meson.source_root())
doxygen_data.set('TOP_BUILDDIR', meson.build_root())
doxygen_data.set('OUTPUT_DIR', join_paths(meson.source_root(), 'docs/doxygen'))
doxygen_data.set('README_PATH', join_paths(meson.source_root(), 'README.md'))
doxygen_data.set('VERSION', meson.project_version())
doxygen_data.set('PROJECT_NAME', meson.project_name())

doxyfile = configure_file(input:         'tools/Doxyfile.in',
                          output:        'Doxyfile',
                          configuration: doxygen_data,
                          install:       false)

docs_target = run_target('docs', command: [doxygen, doxyfile])
