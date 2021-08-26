#ifndef _MULTIBOOT_BOOT_INFO_HPP
#define _MULTIBOOT_BOOT_INFO_HPP

#include "../../../../logging/logger.hpp"


class boot_info {
public:
    boot_info(logging::logger& in_log, const void * in_boot_info) : _log(in_log), _boot_info(in_boot_info) {}
    void dump();

private:
    template <typename T>
    void _dump(const T * in_tag);

    logging::logger _log;
    const void *    _boot_info;
    // TODO: std::string _bootloader;
    // TODO: loader::binary _cpu_driver;
    // TODO: loader::binary _monitor;
    // TODO: framebuffer info
    // TODO: memory map info
};

#endif // _MULTIBOOT_BOOT_INFO_HPP
