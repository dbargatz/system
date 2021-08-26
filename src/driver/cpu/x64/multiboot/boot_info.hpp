#ifndef _MULTIBOOT_BOOT_INFO_HPP
#define _MULTIBOOT_BOOT_INFO_HPP

#include "../../../../loader/binary.hpp"
#include "../../../../logging/logger.hpp"


class boot_info {
public:
    boot_info(logging::logger& in_log, const void* in_acpi_rsdp,
              std::string* in_bootloader, const void* in_load_base_addr,
              loader::binary* in_monitor) : 
              _log(in_log), _acpi_rsdp(in_acpi_rsdp), 
              _bootloader(in_bootloader), _load_base_addr(in_load_base_addr),
              _monitor(in_monitor) {}
    void dump();

    static boot_info* parse(logging::logger& in_log, const void * in_boot_info);

private:
    template <typename T>
    void _dump(const T * in_tag);

    template <typename R, typename T>
    static R * _parse(logging::logger& in_log, const T * in_tag);

    logging::logger _log;
    const void*     _acpi_rsdp;
    std::string*    _bootloader;
    // TODO: loader::binary _cpu_driver;
    const void*     _load_base_addr;
    loader::binary* _monitor;
    // TODO: framebuffer info
    // TODO: memory map info
};

#endif // _MULTIBOOT_BOOT_INFO_HPP
