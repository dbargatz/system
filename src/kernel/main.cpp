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

    log.test(text("Default format string: {}, {}, \"{}\"\n", (uint64_t)42, (uint64_t)63, 
                  (const char *)"raw string 1"));

    log.test(text("With format specifiers: {x}, {X}, \"{s}\"\n", (uint64_t)42, (uint64_t)63, 
                  (const char *)"raw string 2"));

    log.test(text("With prefixes: {#x}, {#X}, \"{#s}\"\n", (uint64_t)42, (uint64_t)63, 
                  (const char *)"raw string 3"));
    
    while(true) {
        // Spin forever.
    }
}