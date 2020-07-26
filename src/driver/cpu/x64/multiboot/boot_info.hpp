#ifndef _MULTIBOOT_BOOT_INFO_HPP
#define _MULTIBOOT_BOOT_INFO_HPP

#include "../std/logger.hpp"

class boot_info {
public:
    void * monitor_start_addr;
    void * monitor_end_addr;

    boot_info() { monitor_start_addr = nullptr; monitor_end_addr = nullptr; }
    void dump(logger& in_log, const void * in_boot_info);

private:
    template <typename T>
    void _dump(logger& in_log, const T * in_tag);
};

#endif // _MULTIBOOT_BOOT_INFO_HPP
