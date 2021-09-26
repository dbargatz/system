project('system', 'cpp', version: '0.1.0')

grub_cfg = '../drivers/core/x64/grub.cfg'

# Libraries
subdir('libs/libcxx')
subdir('libs/libsystem')

# Drivers
subdir('drivers/core/arm64')
subdir('drivers/core/x64')
subdir('drivers/ps2_keyboard')
subdir('drivers/vga')

# Applications
subdir('apps/monitor')

iso_x64 = custom_target('iso_x64',
    input: [core_x64_bin, monitor_bin],
    output: 'system.iso',
    command: ['mkdir', '-p', 'iso/boot/grub', '&&',
              'cp', grub_cfg, 'iso/boot/grub', '&&',
              'cp', '@INPUT@', 'iso/boot', '&&',
              'grub-mkrescue', '--quiet', '-o', '@OUTPUT@', 'iso'],
    install: true,
    install_dir: 'build')

arm64 = custom_target('arm64',
    input: [core_arm64_bin],
    output: 'kernel8.img',
    command: ['cp', '@INPUT@', '@OUTPUT@'],
    install: true,
    install_dir: 'build')

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
