project('system', 'cpp', version: '0.1.0')

grub_cfg = '../../src/grub.cfg'
subdir('src/driver/cpu/x64')

iso = custom_target('iso',
    input: core_x64_bin,
    output: 'system.iso',
    command: ['mkdir', '-p', 'iso/boot/grub', '&&',
              'cp', grub_cfg, 'iso/boot/grub', '&&', 
              'cp', '@INPUT@', 'iso/boot', '&&',
              'grub-mkrescue', '--quiet', '-o', '@OUTPUT@', 'iso'],
    install: true,
    install_dir: 'build/artifacts')