#ifndef _MULTIBOOT_BOOT_INFO_HPP
#define _MULTIBOOT_BOOT_INFO_HPP

#include "../logging/logger.hpp"


class boot_info {
public:
    boot_info(logging::logger& in_log, const void* in_acpi_rsdp,
              std::string* in_bootloader, std::string* in_cmdline,
              const void* in_load_base_addr, std::string* in_monitor_cmd,
              const void* in_monitor_start, const void* in_monitor_end) :
              _log(in_log), _acpi_rsdp(in_acpi_rsdp), 
              _bootloader(in_bootloader), _cmdline(in_cmdline),
              _load_base_addr(in_load_base_addr), _monitor_cmd(in_monitor_cmd),
              _monitor_start_addr(in_monitor_start), _monitor_end_addr(in_monitor_end) {}
    void dump();

    static boot_info* parse(logging::logger& in_log, const void * in_boot_info);

private:
    template <typename T>
    void _dump(const T * in_tag);

    template <typename R, typename T>
    static R _parse(logging::logger& in_log, const T * in_tag);

    logging::logger _log;
    const void*     _acpi_rsdp;
    std::string*    _bootloader;
    std::string*    _cmdline;
    // TODO: loader::binary _cpu_driver;
    const void*     _load_base_addr;
    // TODO: framebuffer info
    // TODO: memory map info
    std::string*    _monitor_cmd;
    const void*     _monitor_start_addr;
    const void*     _monitor_end_addr;
};

#endif // _MULTIBOOT_BOOT_INFO_HPP