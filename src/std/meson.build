std_src = [
    'cstring.cpp',
    'string.cpp',
]

std_cpp_args = [
    '-ffreestanding',
    '-fno-builtin', '-fno-exceptions', '-fno-rtti', '-fno-threadsafe-statics',
    '-g',
    '-nostdlib', '-nostdinc', '-nostdinc++',
    '-std=c++2a',
    '-pedantic-errors', '-Wall', '-Werror'
]

std_lib = static_library('std', std_src, cpp_args: std_cpp_args)
