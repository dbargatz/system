#include <cassert>
#include <cstdint>

inline std::uint16_t get_cpl() {
    std::uint16_t cs_selector = UINT16_MAX;
    asm volatile("mov %%cs, %0" : "=m"(cs_selector));

    // CS should only ever be 0x08 (Ring 0) or 0x2B (Ring 3); if not, this is a problem.
    assert((0x08 == cs_selector) || (0x2B == cs_selector));

    // Return Ring 0 for 0x08 and Ring 3 for 0x2B, which are the only options from the assert() above.
    return (0x08 == cs_selector ? 0 : 3);
}

inline void syscall(const std::uint8_t in_id) {
    asm volatile("xor %%rdi, %%rdi; movb %0, %%dil; syscall" : : "m"(in_id));
}

// Have to define because there is no nice libc that does this for us; see
// https://web.archive.org/web/20200421041419/http://dbp-consulting.com/tutorials/debugging/linuxProgramStartup.html
// for more info on what _start() does.
extern "C" void _start() {
    syscall(get_cpl());
    auto sysmem = (std::uint8_t *)0x11D0C0;
    syscall(*sysmem);
    *sysmem = 0x0;
    syscall(*sysmem);
}
