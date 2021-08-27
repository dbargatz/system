#include <memory_resource>
#include "../../../logging/logger.hpp"

#include "std/panic.h"
#include "debug/uart.hpp"
#include "debug/uart_logger.hpp"
#include "display/vga.hpp"
#include "interrupts/gdt.hpp"
#include "interrupts/tss.hpp"
#include "core.hpp"
#include "timer/pit.hpp"
#include "keyboard/ps2_controller.hpp"
#include "keyboard/ps2_keyboard.hpp"
#include "multiboot/boot_info.hpp"

// TODO: store this in the FS or GS register so it's always accessible from
//       the core driver; make sure on returns to user mode FS/GS is restored
//       so we don't leak it to user mode.
core * this_core;

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
    auto uart_driver = new uart();
    auto uart_log = new uart_backend(*uart_driver);
    auto log = new logging::logger();
    log->add_backend(uart_log);

    // Parse the Multiboot 2 boot information and dump it to the log.
    boot_info* boot = boot_info::parse(*log, in_boot_info);

    // Create a VGA instance and clear the screen. Note that the VGA logging
    // backend is currently broken, so only the UART logging backend is used.
    auto vga_driver = new vga();
    vga_driver->clear_screen(vga::color::black);

    // Bind abstract classes to implementations and create the bootstrap core.
    auto gdt_obj = new gdt();
    auto idt_obj = new idt(*log);
    auto tss_obj = new tss(*gdt_obj);
    auto ps2_driver = new ps2_controller(*log);
    auto set2 = new scancode_set_2();
    auto keyboard_driver = new ps2_keyboard(*log, *ps2_driver, *set2);
    auto interrupt_driver = new pic(*log);
    auto timer = new pit(*log);
    auto bootstrap_core = new core(*log, *boot, *gdt_obj, *idt_obj, *keyboard_driver, *interrupt_driver, *ps2_driver, *timer, *tss_obj);

    // Save off the current core
    this_core = bootstrap_core;
    this_core->run();

    PANIC(u8"End of core_entry() reached!");
    return -1;
}
