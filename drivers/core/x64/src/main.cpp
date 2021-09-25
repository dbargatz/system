#include <memory_resource>

#include "std/panic.h"
#include "logging/uart.hpp"
#include "logging/uart_logger.hpp"
#include "interrupts/gdt.hpp"
#include "interrupts/tss.hpp"
#include "core_manager.hpp"
#include "timer/pit.hpp"
#include "multiboot/boot_info.hpp"

// TODO: store this in the FS or GS register so it's always accessible from
//       the core driver; make sure on returns to user mode FS/GS is restored
//       so we don't leak it to user mode.
core::x64::core_manager * this_core;

void operator delete(void * in_ptr) {
    PANIC("delete() called in core driver!");
}

extern "C" void syscall_entry(const std::uint8_t in_syscall_id) {
    this_core->dispatch_syscall(in_syscall_id);
}

extern "C" void interrupt_entry(const void * in_frame_ptr) {
    this_core->dispatch_interrupt(in_frame_ptr);
}

extern "C" int core_entry(const void * in_boot_info) {
    // Set up the UART (serial port) logging backend and configure the logger
    // to use it. The add_backend() helper exists to allow multiple logging
    // backends to be added to a single logger; boost::di's implementation of
    // constructor injection with multiple bindings is currently broken, so
    // add_backend() is a workaround.
    auto uart_driver = new core::x64::logging::uart();
    auto uart_log = new core::x64::logging::uart_backend(*uart_driver);
    auto log = new logging::logger();
    log->add_backend(uart_log);

    // Parse the Multiboot 2 boot information and dump it to the log.
    auto boot = core::x64::multiboot::boot_info::parse(*log, in_boot_info);

    // Bind abstract classes to implementations and create the bootstrap core.
    auto gdt_obj = new core::x64::interrupts::gdt();
    auto idt_obj = new core::x64::interrupts::idt(*log);
    auto tss_obj = new core::x64::interrupts::tss(*gdt_obj);
    auto interrupt_driver = new core::x64::interrupts::pic(*log);
    auto timer = new core::x64::timer::pit(*log);
    auto bootstrap_core = new core::x64::core_manager(*log, *boot, *gdt_obj, *idt_obj, *interrupt_driver, *timer, *tss_obj);

    // Save off the current core
    this_core = bootstrap_core;
    this_core->run();

    PANIC(u8"End of core_entry() reached!");
    return -1;
}
