#include "boot_info.hpp"
#include <cstring>
#include "../../../../loader/__elf.hpp" // TODO: fix include of private header
#include "multiboot2.h"

#define ALIGN_8_BYTE(x) (x + ((x % 8) ? (8 - (x % 8)) : 0))

struct multiboot_tag_bootloader : multiboot_tag_string {};
template <>
void boot_info::_dump(logging::logger& in_log, const multiboot_tag_bootloader * in_tag) {
    in_log.debug(u8"\tBootloader name: {}", (const char*)in_tag->string);
}

struct multiboot_tag_cmdline : multiboot_tag_string {};
template <>
void boot_info::_dump(logging::logger& in_log, const multiboot_tag_cmdline * in_tag) {
    in_log.debug(u8"\tCommand line: {}", (const char*)in_tag->string);
}

template <>
void boot_info::_dump(logging::logger& in_log, const multiboot_tag_basic_meminfo * in_tag) {
    in_log.debug(u8"\tBasic Memory Info:");
    in_log.debug(u8"\t\tLower memory: {:#016X}", in_tag->mem_lower);
    in_log.debug(u8"\t\tUpper memory: {:#016X}", in_tag->mem_upper);
}

template <>
void boot_info::_dump(logging::logger& in_log, const multiboot_tag_bootdev * in_tag) {
    in_log.debug(u8"\tBoot Device:");
    in_log.debug(u8"\t\tBIOS Device: {:#08X}", in_tag->biosdev);
    in_log.debug(u8"\t\tSlice      : {}", in_tag->slice);
    in_log.debug(u8"\t\tPartition  : {}", in_tag->part);
}

template <>
void boot_info::_dump(logging::logger& in_log, const multiboot_tag_mmap * in_tag) {
    std::uint64_t num_entries = (in_tag->size - sizeof(multiboot_tag_mmap)) / in_tag->entry_size;
    in_log.debug(u8"\tMemory map ({} entries):", num_entries);
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
        in_log.debug(u8"\t\tEntry {:02d}: {:#016X} - {:#016X} ({:16}: {} bytes)", i,
            in_tag->entries[i].addr,
            in_tag->entries[i].addr + in_tag->entries[i].len - 1,
            typestr, in_tag->entries[i].len);
    }
}

template <>
void boot_info::_dump(logging::logger& in_log, const multiboot_tag_framebuffer * in_tag) {
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

    in_log.debug(u8"\tFramebuffer: {}", type);
    in_log.debug(u8"\t\tStart Address : {:#016X}", common.framebuffer_addr);
    in_log.debug(u8"\t\tDimensions    : {}x{}x{} (width * height * pitch)", common.framebuffer_width, common.framebuffer_height, common.framebuffer_pitch);
    in_log.debug(u8"\t\tBits per Pixel: {}", common.framebuffer_bpp);
}

template <>
void boot_info::_dump(logging::logger& in_log, const multiboot_tag_module * in_tag) {
    in_log.debug(u8"\tModule {}:", (const char*)in_tag->cmdline);
    in_log.debug(u8"\t\tStart physical address: {:#016X}", in_tag->mod_start);
    in_log.debug(u8"\t\tEnd physical address  : {:#016X}", in_tag->mod_end);

    // TODO: move this to a parser function.
    // TODO: verify this is actually the "monitor" module.
    monitor_start_addr = (void *)(std::uintptr_t)in_tag->mod_start;
    monitor_end_addr = (void *)(std::uintptr_t)in_tag->mod_end;
}

template <>
void boot_info::_dump(logging::logger& in_log, const multiboot_tag_elf_sections * in_tag) {
    in_log.debug(u8"\tELF Symbols ({} bytes, {} entries)", in_tag->size, in_tag->num);
    // auto sections = (const struct loader::Elf64_Shdr *)&in_tag->sections;
    // auto str_table = (const char8_t *)sections[in_tag->shndx].sh_addr;
    // char8_t name_buf[40] = {0};
    // for(auto i = 0; i < in_tag->num; i++) {
    //     auto section = sections[i];
    //     auto name = &(str_table[section.sh_name]);
    //     if(std::strlen(name) < sizeof(name_buf) - 1) {
    //         std::memcpy(name_buf, name, std::strlen(name));
    //     } else {
    //         std::memcpy(name_buf, name, sizeof(name_buf)-4);
    //         name_buf[sizeof(name_buf)-4] = '.';
    //         name_buf[sizeof(name_buf)-3] = '.';
    //         name_buf[sizeof(name_buf)-2] = '.';
    //         name_buf[sizeof(name_buf)-1] = '\0';
    //     }
    //     in_log.debug(u8"\t\t{:#08X}: {}", section.sh_addr, (const char8_t *)name_buf);
    //     std::memset(name_buf, 0, sizeof(name_buf));
    // }
}

template <>
void boot_info::_dump(logging::logger& in_log, const multiboot_tag_apm * in_tag) {
    in_log.debug(u8"\tAPM Information (version {}.{}):",
        in_tag->version >> 8, in_tag->version & 0x00FF);
    in_log.debug(u8"\t\t32-bit CS:IP: {:#04X}:{:#08X} ({} bytes)",
        in_tag->cseg, in_tag->offset, in_tag->cseg_len);
    in_log.debug(u8"\t\t16-bit CS   : {:#04X}          ({} bytes)",
        in_tag->cseg_16, in_tag->cseg_16_len);
    in_log.debug(u8"\t\t16-bit DS   : {:#04X}          ({} bytes)",
        in_tag->dseg, in_tag->dseg_len);
    in_log.debug(u8"\t\tFlags       : {:#04X}", in_tag->flags);
}

template <>
void boot_info::_dump(logging::logger& in_log, const multiboot_tag_load_base_addr * in_tag) {
    in_log.debug(u8"\tImage load physical base address: {:#016X}",
        in_tag->load_base_addr);
}

void boot_info::dump(logging::logger& in_log, const void * in_boot_info) {
    auto cur_ptr = (std::uint8_t *)in_boot_info;
    auto total_size = *(multiboot_uint32_t *)cur_ptr;

    // Move past the total_size and reserved fields
    cur_ptr += ALIGN_8_BYTE(sizeof(multiboot_uint32_t) * 2);
    total_size -= ALIGN_8_BYTE(sizeof(multiboot_uint32_t) * 2);

    // Loop through the tags
    in_log.debug(u8"Multiboot 2 Boot Info:");
    while(total_size > 0) {
        multiboot_tag * tag = (multiboot_tag *)cur_ptr;
        switch(tag->type) {
            case MULTIBOOT_TAG_TYPE_END:
                in_log.debug(u8"\tFound end tag.");
                break;
            case MULTIBOOT_TAG_TYPE_CMDLINE:
                _dump(in_log, (const multiboot_tag_cmdline *)cur_ptr);
                break;
            case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
                _dump(in_log, (const multiboot_tag_bootloader *)cur_ptr);
                break;
            case MULTIBOOT_TAG_TYPE_MODULE:
                _dump(in_log, (const multiboot_tag_module *)cur_ptr);
                break;
            case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
                _dump(in_log, (const multiboot_tag_basic_meminfo *)cur_ptr);
                break;
            case MULTIBOOT_TAG_TYPE_BOOTDEV:
                _dump(in_log, (const multiboot_tag_bootdev *)cur_ptr);
                break;
            case MULTIBOOT_TAG_TYPE_MMAP:
                _dump(in_log, (const multiboot_tag_mmap *)cur_ptr);
                break;
            case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
                _dump(in_log, (const multiboot_tag_framebuffer *)cur_ptr);
                break;
            case MULTIBOOT_TAG_TYPE_ELF_SECTIONS:
                _dump(in_log, (const multiboot_tag_elf_sections *)cur_ptr);
                break;
            case MULTIBOOT_TAG_TYPE_APM:
                _dump(in_log, (const multiboot_tag_apm *)cur_ptr);
                break;
            case MULTIBOOT_TAG_TYPE_LOAD_BASE_ADDR:
                _dump(in_log, (const multiboot_tag_load_base_addr *)cur_ptr);
                break;
            default:
                in_log.debug(u8"\tSkipping tag {:2} ({} bytes)", tag->type, tag->size);
                break;
        }
        cur_ptr += ALIGN_8_BYTE(tag->size);
        total_size -= ALIGN_8_BYTE(tag->size);
    }
}
