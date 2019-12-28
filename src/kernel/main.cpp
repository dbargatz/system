#include "platform/qemu-system-x86_64/boot/logger.hpp"
#include "platform/qemu-system-x86_64/types/text.hpp"

extern "C" int kmain(const void * in_boot_info) {
    using kernel::platform::x86_64::logger;
    using kernel::platform::x86_64::types::text;
    using kernel::platform::x86_64::types::uint64_t;

    logger log;
    log.debug("Hello, world!\n");
    log.info("Hello, world!\n");
    log.warn("Hello, world!\n");
    log.error("Hello, world!\n");

    log.test(text("This is a format string: {}, {}, \"{}\"\n", (uint64_t)42, (uint64_t)32, 
                  (const char *)"booze ain't food!"));

    log.test(text("This one has formatting: {:x}, {:X}, \"{:s}\"",
                  (uint8_t)0xFF, (uint16_t)0xBEEF, (const char *)"raw string"));
    
    while(true) {
        // Spin forever.
    }
}