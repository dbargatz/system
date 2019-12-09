#include "platform/qemu-system-x86_64/boot/logger.hpp"

extern "C" int kmain(const void * in_boot_info) {
    using kernel::platform::x86_64::logger;

    logger log;
    log.debug("Hello, world!\n");
    log.info("Hello, world!\n");
    log.warn("Hello, world!\n");
    log.error("Hello, world!\n");
    
    while(true) {
        // Spin forever.
    }
}