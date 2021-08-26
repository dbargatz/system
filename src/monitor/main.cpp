#include <cstdint>

inline void syscall(const std::uint8_t in_id) {
    asm volatile("xor %%rdi, %%rdi; movb %0, %%dil; syscall" : : "m"(in_id));
}

// Have to define because there is no nice libc that does this for us; see
// https://web.archive.org/web/20200421041419/http://dbp-consulting.com/tutorials/debugging/linuxProgramStartup.html
// for more info on what _start() does.
extern "C" void _start() {
    syscall(0x22);
}
