#include "platform/qemu-system-x86_64/boot/logger.hpp"
#include "platform/qemu-system-x86_64/boot/panic.h"
#include "platform/qemu-system-x86_64/types/text.hpp"

extern "C" int kmain(const void * in_boot_info) {
    using kernel::platform::x86_64::logger;
    using kernel::platform::x86_64::types::text;
    using kernel::platform::x86_64::types::uint64_t;

    logger log;

    log.debug("Default format string: {}, {}, \"{}\"\n", (uint64_t)42, (uint64_t)63, (const char *)"raw string 1");
    log.info("With format specifiers: {x}, {X}, \"{s}\"\n", (uint64_t)42, (uint64_t)63, (const char *)"raw string 2");
    log.warn("With prefixes: {#x}, {#X}, \"{#s}\"\n", (uint64_t)42, (uint64_t)63, (const char *)"raw string 3");
    log.error("Here is some error text\n");

    unsigned long long x = 1000000001;
    while(x-- > 0) {
        // Loop for a bit
        if(0 == x % 250000000) {
            log.debug("Panic in {}\n", x / 250000000);
        }
    }

    panic("Here is panic!");
}