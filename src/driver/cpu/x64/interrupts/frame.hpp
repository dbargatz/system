#ifndef _INTERRUPTS_FRAME_HPP
#define _INTERRUPTS_FRAME_HPP

#include "../std/logger.hpp"

class interrupt_frame {
private:
    logger& _log;

public:
    struct _interrupt_frame {
        uint64_t r15;
        uint64_t r14;
        uint64_t r13;
        uint64_t r12;
        uint64_t rbx;
        uint64_t r11;
        uint64_t r10;
        uint64_t r9;
        uint64_t r8;
        uint64_t rax;
        uint64_t rcx;
        uint64_t rdx;
        uint64_t rsi;
        uint64_t rdi;
        uint64_t interrupt_number;
        uint64_t error_code;
        uint64_t rip;
        uint64_t cs;
        uint64_t rflags;
        uint64_t rsp;
        uint64_t ss;
    } __attribute__((packed));

    const struct _interrupt_frame * frame;

    interrupt_frame(logger& in_log, const void * in_frame_ptr);

    void dump(logger::level in_level, uint8_t in_indent=0);
};

#endif // _INTERRUPTS_FRAME_HPP
