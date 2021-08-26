#include "boot_info.hpp"
#include <cstring>
#include "../../../../loader/binary.hpp"
#include "../../../../loader/__elf.hpp" // TODO: fix include of private header
#include "multiboot2.h"

#define ALIGN_8_BYTE(x) (x + ((x % 8) ? (8 - (x % 8)) : 0))

struct multiboot_tag_bootloader : multiboot_tag_string {};
struct multiboot_tag_cmdline : multiboot_tag_string {};

template <>
std::string* boot_info::_parse(logging::logger& in_log, const multiboot_tag_bootloader * in_tag) {
    return new std::string((const char8_t *)in_tag->string);
}

template <>
std::string* boot_info::_parse(logging::logger& in_log, const multiboot_tag_cmdline * in_tag) {
    return new std::string((const char8_t *)in_tag->string);
}

template <>
void boot_info::_dump(const multiboot_tag_mmap * in_tag) {
    std::uint64_t num_entries = (in_tag->size - sizeof(multiboot_tag_mmap)) / in_tag->entry_size;
    _log.debug(u8"\tMemory map ({} entries):", num_entries);
    for(auto i = 0; i < num_entries; i++) {
        auto typestr = u8"";
        switch(in_tag->entries[i].type) {
            case MULTIBOOT_MEMORY_AVAILABLE:
                typestr = u8"available";
                break;
            case MULTIBOOT_MEMORY_RESERVED:
                typestr = u8"reserved ";
                break;
            case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
                typestr = u8"ACPI reclaimable";
                break;
            case MULTIBOOT_MEMORY_NVS:
                typestr = u8"reserved (preserve on hibernation)";
                break;
            case MULTIBOOT_MEMORY_BADRAM:
                typestr = u8"bad RAM";
                break;
            default:
                typestr = u8"unknown";
                break;
        }
        _log.debug(u8"\t\tEntry {:02d}: {:#016X} - {:#016X} ({:16}: {} bytes)", i,
            in_tag->entries[i].addr,
            in_tag->entries[i].addr + in_tag->entries[i].len - 1,
            typestr, in_tag->entries[i].len);
    }
}

template <>
void boot_info::_dump(const multiboot_tag_framebuffer * in_tag) {
    auto common = in_tag->common;
    auto type = u8"UNKNOWN";
    switch(common.framebuffer_type) {
        case MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED:
            type = u8"Indexed";
            break;
        case MULTIBOOT_FRAMEBUFFER_TYPE_RGB:
            type = u8"RGB";
            break;
        case MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT:
            type = u8"EGA Text";
            break;
    }

    _log.debug(u8"\tFramebuffer: {}", type);
    _log.debug(u8"\t\tStart Address : {:#016X}", common.framebuffer_addr);
    _log.debug(u8"\t\tDimensions    : {}x{}x{} (width * height * pitch)", common.framebuffer_width, common.framebuffer_height, common.framebuffer_pitch);
    _log.debug(u8"\t\tBits per Pixel: {}", common.framebuffer_bpp);
}

template <>
loader::binary* boot_info::_parse(logging::logger& in_log, const multiboot_tag_module * in_tag) {
    auto monitor = new loader::binary(in_log);
    auto cmdline = new std::string((const char8_t *)in_tag->cmdline);
    auto start = (void *)static_cast<std::uint64_t>(in_tag->mod_start);
    auto end = (void *)static_cast<std::uint64_t>(in_tag->mod_end);
    monitor->init(cmdline, start, end);
    return monitor;
}

void boot_info::dump() {
    _log.info(u8"Boot info:");
    _log.info(u8"\tACPI v1.0 RSDP        : {:#016X}", _acpi_rsdp);
    _log.info(u8"\tBootloader name       : {}", *_bootloader);
    _log.info(u8"\tCPU Driver commandline: {}", *_cmdline);
    _log.info(u8"\tCPU Driver load addr  : {:#016X}", _load_base_addr);
    _log.info(u8"\tMonitor binary: ");
    _monitor->dump();
}

boot_info* boot_info::parse(logging::logger& in_log, const void * in_boot_info) {
    auto cur_ptr = (std::uint8_t *)in_boot_info;
    auto total_size = *(multiboot_uint32_t *)cur_ptr;

    // Move past the total_size and reserved fields
    cur_ptr += ALIGN_8_BYTE(sizeof(multiboot_uint32_t) * 2);
    total_size -= ALIGN_8_BYTE(sizeof(multiboot_uint32_t) * 2);

    // Loop through the tags
    in_log.debug(u8"Parsing Multiboot 2 Boot Info:");
    const void * acpi_rsdp;
    std::string * booter;
    std::string * cmdline;
    multiboot_uint32_t addr32;
    const void * load_base_addr;
    loader::binary * monitor;

    while(total_size > 0) {
        multiboot_tag * tag = (multiboot_tag *)cur_ptr;
        switch(tag->type) {
            case MULTIBOOT_TAG_TYPE_END:
                break;
            case MULTIBOOT_TAG_TYPE_CMDLINE:
                cmdline = _parse<std::string>(in_log, (const multiboot_tag_cmdline *)cur_ptr);
                break;
            case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
                booter = _parse<std::string>(in_log, (const multiboot_tag_bootloader *)cur_ptr);
                break;
            case MULTIBOOT_TAG_TYPE_MODULE:
                monitor = _parse<loader::binary>(in_log, (const multiboot_tag_module *)cur_ptr);
                break;
            // TODO: case MULTIBOOT_TAG_TYPE_MMAP:
            // TODO: case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
            case MULTIBOOT_TAG_TYPE_ACPI_OLD:
                acpi_rsdp = (void*)((const multiboot_tag_old_acpi *)cur_ptr)->rsdp;
                break;
            case MULTIBOOT_TAG_TYPE_LOAD_BASE_ADDR:
                addr32 = ((const multiboot_tag_load_base_addr *)cur_ptr)->load_base_addr;
                load_base_addr = (void*)static_cast<std::uint64_t>(addr32);
                break;
            default:
                in_log.debug(u8"\tSkipped tag {:2} ({} bytes).", tag->type, tag->size);
                break;
        }
        cur_ptr += ALIGN_8_BYTE(tag->size);
        total_size -= ALIGN_8_BYTE(tag->size);
    }

    return new boot_info(in_log, acpi_rsdp, booter, cmdline, load_base_addr, monitor);
}
