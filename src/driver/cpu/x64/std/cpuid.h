#ifndef _STD_CPUID_H
#define _STD_CPUID_H

#include "stdint.h"

/**
 * @brief Executes the CPUID instruction with the given code, returning the
 * values of the EAX/EDX registers.
 * 
 * @param in_code the CPUID code to execute
 * @param out_eax pointer to value to contain CPUID's EAX result
 * @param out_edx pointer to value to contain CPUID's EDX result
 */
static inline void cpuid(uint32_t in_code, uint32_t * out_eax, uint32_t * out_edx)
{
    asm volatile("cpuid"                        : // Instruction
                 "=a"(*out_eax), "=d"(*out_edx) : // Outputs
                 "a"(in_code)                   : // Inputs
                 "ecx", "ebx");                   // Clobbered
}

#endif // _STD_CPUID_H
