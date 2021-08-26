#include <memory_resource>
#include "../../../logging/logger.hpp"

#include "std/panic.h"
#include "debug/uart_logger.hpp"
#include "display/vga.hpp"
#include "interrupts/gdt.hpp"
#include "interrupts/tss.hpp"
#include "core.hpp"
#include "timer/pit.hpp"
#include "keyboard/ps2_controller.hpp"
#include "keyboard/ps2_keyboard.hpp"
#include "multiboot/boot_info.hpp"

#include "../../../boost/di.hpp"
namespace di = boost::di;

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
    const auto log_injector = di::make_injector();
    auto uart_log = log_injector.create<uart_backend&>();
    auto log = log_injector.create<logging::logger&>();
    log.add_backend(&uart_log);

    // Parse the Multiboot 2 boot information and dump it to the log.
    boot_info* boot = boot_info::parse(log, in_boot_info);

    // Create a VGA instance and clear the screen. Note that the VGA logging
    // backend is currently broken, so only the UART logging backend is used.
    auto vga_ref = log_injector.create<vga&>();
    vga_ref.clear_screen(vga::color::black);

    // Bind abstract classes to implementations and create the bootstrap core.
    di::aux::owner<boot_info*> boot_ptr{boot};
    const auto core_injector = di::make_injector(
        di::bind<std::pmr::memory_resource>().to<std::pmr::new_delete_memory_resource>(),
        di::bind<boot_info>().to(*boot_ptr),
        di::bind<keyboard>().to<ps2_keyboard>(),
        di::bind<logging::logger>().to(log),
        di::bind<scancode_set>().to<scancode_set_2>(),
        di::bind<timer>().to<pit>()
    );
    auto bootstrap_core = core_injector.create<core>();

    // Save off the current core 
    this_core = &bootstrap_core;
    this_core->run();

    PANIC(u8"End of core_entry() reached!");
    return -1;
}
