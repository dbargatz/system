#include "std/array.hpp"
#include "std/logger.hpp"
#include "std/panic.h"
#include "debug/uart_logger.hpp"
#include "display/vga_logger.hpp"
#include "interrupts/gdt.hpp"
#include "interrupts/tss.hpp"
#include "core.hpp"
#include "timer/pit.hpp"
#include "keyboard/ps2_controller.hpp"
#include "keyboard/ps2_keyboard.hpp"

#include "../../../boost/di.hpp"
namespace di = boost::di;

// TODO: store this in the FS or GS register so it's always accessible from
//       the core driver; make sure on returns to user mode FS/GS is restored
//       so we don't leak it to user mode.
core * this_core;

void operator delete(void * in_ptr) {
    PANIC("delete() called in core driver!");
}

extern "C" void interrupt_entry(const void * in_frame_ptr) {
    this_core->dispatch_interrupt(in_frame_ptr);
}

extern "C" int core_entry(const void * in_boot_info) {
    // To work around the fact that boost::di isn't working right now with
    // multiple bindings, create a logger instance and add both backends to
    // it via the add_backend() helper (instead of the desired constructor
    // injection technique), then register that logger as a singleton instance
    // with a separate core injector.
    const auto log_injector = di::make_injector();
    auto vga_log = log_injector.create<vga_logger&>();
    auto uart_log = log_injector.create<uart_logger&>();
    auto log = log_injector.create<logger&>();
    log.add_backend(&vga_log);
    log.add_backend(&uart_log);

    // Bind abstract classes to implementations and create the bootstrap core.
    const auto core_injector = di::make_injector(
        di::bind<logger>().to(log),
        di::bind<keyboard>().to<ps2_keyboard>(),
        di::bind<scancode_set>().to<scancode_set_2>(),
        di::bind<timer>().to<pit>()
    );
    auto bootstrap_core = core_injector.create<core>();

    // Save off the current core 
    this_core = &bootstrap_core;
    this_core->run(in_boot_info);

    PANIC("End of core_entry() reached!");
    return -1;
}
