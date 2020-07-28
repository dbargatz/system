#ifndef _INTERRUPTS_FRAME_HPP
#define _INTERRUPTS_FRAME_HPP

#include <cstdint>
#include "../../../../logging/logger.hpp"

class interrupt_frame {
public:
    struct _interrupt_frame {
        std::uint64_t r15;
        std::uint64_t r14;
        std::uint64_t r13;
        std::uint64_t r12;
        std::uint64_t rbx;
        std::uint64_t r11;
        std::uint64_t r10;
        std::uint64_t r9;
        std::uint64_t r8;
        std::uint64_t rax;
        std::uint64_t rcx;
        std::uint64_t rdx;
        std::uint64_t rsi;
        std::uint64_t rdi;
        std::uint64_t interrupt_number;
        std::uint64_t error_code;
        std::uint64_t rip;
        std::uint64_t cs;
        std::uint64_t rflags;
        std::uint64_t rsp;
        std::uint64_t ss;
    } __attribute__((packed));

    const struct _interrupt_frame * frame;

    interrupt_frame(const void * in_frame_ptr);

    void dump(logging::logger& in_log);
};

#endif // _INTERRUPTS_FRAME_HPP
