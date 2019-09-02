#include "platform/qemu-system-x86_64/boot/logger.hpp"

extern "C" int kmain(const void * in_boot_info) {
    using kernel::platform::x86_64::logger;

    logger log;
    log.dbg("Hello, world!\n");
    log.inf("Hello, world!\n");
    log.wrn("Hello, world!\n");
    log.err("Hello, world!\n");
    
    while(true) {
        // Spin forever.
    }
}