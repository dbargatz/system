#include "std/logger.hpp"
#include "std/panic.h"
#include "debug/uart_logger.hpp"
#include "display/vga_logger.hpp"
#include "interrupts/gdt.hpp"
#include "interrupts/interrupt_manager.hpp"
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
Core * this_core;

extern "C" int kmain(const void * in_boot_info) {
    const auto injector = di::make_injector(
        // TODO: once we can parse Multiboot args, make the selection of logger
        //       backends dependent on args.
        di::bind<logger_backend*[]>.to<vga_logger, uart_logger>()
    );
    auto log = injector.create<logger>();

    gdt g(log);
    tss t(log, g);
    IDT idt(log);
    PIC pic(log);
    InterruptManager intmgr(log, idt, pic);
    PIT pit(log);

    ps2_controller ps2(log);
    ps2_port kbd_port = ps2_port::INVALID;
    ps2_device_type port1 = ps2.get_type(ps2_port::PORT1);
    ps2_device_type port2 = ps2.get_type(ps2_port::PORT2);
    log.debug("PS/2 port 1: {}", ps2.get_type_str(port1));
    log.debug("PS/2 port 2: {}", ps2.get_type_str(port2));

    if(port1 == ps2_device_type::KEYBOARD_STANDARD) {
        kbd_port = ps2_port::PORT1;
    } else if(port2 == ps2_device_type::KEYBOARD_STANDARD) {
        kbd_port = ps2_port::PORT2;
    }
    ps2_keyboard kbd(log, ps2, kbd_port);

    Core bootstrap_core(log, g, t, in_boot_info, intmgr, pit, kbd);
    this_core = &bootstrap_core;

    bootstrap_core.run();

    PANIC("End of kmain reached!");
    return -1;
}
