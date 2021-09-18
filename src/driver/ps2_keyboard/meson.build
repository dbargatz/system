ps2_keyboard_src = [
    'keystate.cpp',
    'ps2_controller.cpp',
    'ps2_keyboard.cpp',
    'scancode_set_2.cpp',
]

link_with_objs = [libcxx_lib, libloader_lib, libsystem_lib]
ps2_keyboard_bin = executable('ps2_keyboard.bin', ps2_keyboard_src, link_with: link_with_objs)
