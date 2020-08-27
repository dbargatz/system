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
    // Create a VGA instance and clear the screen.
    const auto log_injector = di::make_injector();
    auto vga_ref = log_injector.create<vga&>();
    vga_ref.clear_screen(vga::color::black);

    // The VGA logging backend is currently broken, so only the UART logging
    // backend is enabled. The add_backend() helper exists to allow multiple
    // logging backends to be added to a single logger; boost::di's 
    // implementation of constructor injection with multiple bindings is
    // currently broken, so add_backend() is a workaround.
    auto uart_log = log_injector.create<uart_backend&>();
    auto log = log_injector.create<logging::logger&>();
    log.add_backend(&uart_log);

    // Bind abstract classes to implementations and create the bootstrap core.
    const auto core_injector = di::make_injector(
        di::bind<logging::logger>().to(log),
        di::bind<keyboard>().to<ps2_keyboard>(),
        di::bind<scancode_set>().to<scancode_set_2>(),
        di::bind<timer>().to<pit>()
    );
    auto bootstrap_core = core_injector.create<core>();

    // Save off the current core 
    this_core = &bootstrap_core;
    this_core->run(in_boot_info);

    PANIC(u8"End of core_entry() reached!");
    return -1;
}
