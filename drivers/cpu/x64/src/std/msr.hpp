#ifndef _STD_MSR_HPP
#define _STD_MSR_HPP

#include <cstdint>

static constexpr std::uint32_t MSR_IA32_EFER  = 0xC0000080;
static constexpr std::uint32_t MSR_IA32_STAR  = 0xC0000081;
static constexpr std::uint32_t MSR_IA32_LSTAR = 0xC0000082;
static constexpr std::uint32_t MSR_IA32_FMASK = 0xC0000084;

static inline std::uint64_t rdmsr(std::uint32_t in_msr) {
    std::uint32_t eax, edx;
    asm volatile("rdmsr"                        : // Instruction
                 "=a"(eax), "=d"(edx)           : // Outputs
                 "a"(in_msr)                    : // Inputs
                 );                               // No clobbers
    return ((std::uint64_t)edx << 32) | ((std::uint64_t)eax);
}

static inline void wrmsr(std::uint32_t in_msr, std::uint64_t in_value) {
    auto low = (std::uint32_t)in_value;
    auto high = (std::uint32_t)(in_value >> 32);
    asm volatile("wrmsr"                          : // Instruction
                                                  : // No outputs
                 "a"(low), "c"(in_msr), "d"(high) : // Inputs
                 );                                 // No clobbers
}

#endif // _STD_MSR_HPP
