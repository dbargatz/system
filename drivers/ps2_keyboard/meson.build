ps2_keyboard_src = [
    'src/keystate.cpp',
    'src/ps2_controller.cpp',
    'src/ps2_keyboard.cpp',
    'src/scancode_set_2.cpp',
]

link_with_objs = [libcxx_lib, libsystem_lib]
ps2_keyboard_bin = executable('ps2_keyboard.bin', ps2_keyboard_src, link_with: link_with_objs)
