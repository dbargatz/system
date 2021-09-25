#ifndef _CORE_X64_CORE_MANAGER_HPP
#define _CORE_X64_CORE_MANAGER_HPP

#include <cstdint>

#include "interrupts/gdt.hpp"
#include "interrupts/idt.hpp"
#include "interrupts/pic.hpp"
#include "interrupts/tss.hpp"
#include "logging/logger.hpp"
#include "multiboot/boot_info.hpp"
#include "timer/timer.hpp"
#include "memory/paging.hpp"

namespace core::x64 {

class core_manager {
private:
    ///< If bit 9 is set in CPUID:01H:EDX, a local APIC is present on this core.
    static const std::uint32_t CPUID_01_EDX_LOCAL_APIC_PRESENT = (1 << 9);
    
    static const std::uint32_t CPUID_80000001 = 0x80000001;
    
    ///< If bit 11 is set in CPUID:80000001H:EDX, the syscall/sysret
    ///< instructions are supported by this core.
    static const std::uint32_t CPUID_80000001_EDX_SYSCALL_SUPPORTED = (1 << 11);

    ///< If bit 9 (IF) is set in RFLAGS, interrupts are enabled on this core.
    static const std::uint64_t RFLAGS_INTERRUPTS_ENABLED_BIT   = (1 << 9);

    memory::pml4 _pml4;

    logging::logger& _log;
    multiboot::boot_info& _boot;
    interrupts::gdt& _gdt;
    interrupts::idt& _idt;
    interrupts::pic& _pic;
    timer::timer& _timer;
    interrupts::tss& _tss;

    bool _disable_interrupts();
    bool _enable_interrupts();
    bool _interrupts_enabled();

public:

    core_manager(logging::logger& in_log,
        multiboot::boot_info& in_boot,
        interrupts::gdt& in_gdt,
        interrupts::idt& in_idt,
        interrupts::pic& in_pic,
        timer::timer& in_timer,
        interrupts::tss& in_tss);

    void dispatch_interrupt(const void * in_frame_ptr);
    void dispatch_syscall(const std::uint8_t in_syscall_id);
    void panic_handler(interrupts::stack_frame& in_frame);
    void run();
};

}; // namespace core::x64

#endif // _CORE_X64_CORE_MANAGER_HPP
