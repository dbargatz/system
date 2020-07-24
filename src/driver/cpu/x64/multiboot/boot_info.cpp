#include "boot_info.hpp"
#include "multiboot2.h"
#include "../std/elf64.hpp"
#include "../std/memcpy.hpp"
#include "../std/memset.hpp"
#include "../../../../std/cstring.hpp" // TODO: include with angle-brackets

#define ALIGN_8_BYTE(x) (x + ((x % 8) ? (8 - (x % 8)) : 0))

struct multiboot_tag_bootloader : multiboot_tag_string {};
template <>
void boot_info::_dump(logger& in_log, const multiboot_tag_bootloader * in_tag) {
    in_log.debug("\tBootloader name: {}", in_tag->string);
}

struct multiboot_tag_cmdline : multiboot_tag_string {};
template <>
void boot_info::_dump(logger& in_log, const multiboot_tag_cmdline * in_tag) {
    in_log.debug("\tCommand line: {}", in_tag->string);
}

template <>
void boot_info::_dump(logger& in_log, const multiboot_tag_mmap * in_tag) {
    uint64_t num_entries = (in_tag->size - sizeof(multiboot_tag_mmap)) / in_tag->entry_size;
    in_log.debug("\tMemory map ({} entries):", num_entries);
    for(auto i = 0; i < num_entries; i++) {
        auto typestr = text("");
        switch(in_tag->entries[i].type) {
            case MULTIBOOT_MEMORY_AVAILABLE:
                typestr = "available";
                break;
            case MULTIBOOT_MEMORY_RESERVED:
                typestr = "reserved ";
                break;
            case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
                typestr = "ACPI reclaimable";
                break;
            case MULTIBOOT_MEMORY_NVS:
                typestr = "reserved (preserve on hibernation)";
                break;
            case MULTIBOOT_MEMORY_BADRAM:
                typestr = "bad RAM";
                break;
            default:
                typestr = "unknown";
                break;
        }
        in_log.debug("\t\tEntry {02d}: {#016X} - {#016X} ({16}: {} bytes)", i,
            in_tag->entries[i].addr,
            in_tag->entries[i].addr + in_tag->entries[i].len - 1,
            typestr, in_tag->entries[i].len);
    }
}

template <>
void boot_info::_dump(logger& in_log, const multiboot_tag_elf_sections * in_tag) {
    in_log.debug("\tELF Symbols ({} bytes, {} entries):", in_tag->size, in_tag->num);
    auto sections = (const struct Elf64_Shdr *)&in_tag->sections;
    auto str_table = (const char *)sections[in_tag->shndx].sh_addr;
    char name_buf[40] = {0};
    for(auto i = 0; i < in_tag->num; i++) {
        auto section = sections[i];
        auto name = &(str_table[section.sh_name]);
        if(strlen(name) < sizeof(name_buf) - 1) {
            memcpy(name_buf, name, strlen(name));
        } else {
            memcpy(name_buf, name, sizeof(name_buf)-4);
            name_buf[sizeof(name_buf)-4] = '.';
            name_buf[sizeof(name_buf)-3] = '.';
            name_buf[sizeof(name_buf)-2] = '.';
            name_buf[sizeof(name_buf)-1] = '\0';
        }
        in_log.debug("\t\t{#08X}: {}", section.sh_addr, (const char *)name_buf);
        memset(name_buf, 0, sizeof(name_buf));
    }
}

template <>
void boot_info::_dump(logger& in_log, const multiboot_tag_apm * in_tag) {
    in_log.debug("\tAPM Information (version {}.{}):",
        in_tag->version >> 8, in_tag->version & 0x00FF);
    in_log.debug("\t\t32-bit CS:IP: {#04X}:{#08X} ({} bytes)",
        in_tag->cseg, in_tag->offset, in_tag->cseg_len);
    in_log.debug("\t\t16-bit CS   : {#04X} ({} bytes)",
        in_tag->cseg_16, in_tag->cseg_16_len);
    in_log.debug("\t\t16-bit DS   : {#04X} ({} bytes)",
        in_tag->dseg, in_tag->dseg_len);
    in_log.debug("\t\tFlags       : {#04X}", in_tag->flags);
}

template <>
void boot_info::_dump(logger& in_log, const multiboot_tag_load_base_addr * in_tag) {
    in_log.debug("\tImage load physical base address: {#016X}",
        in_tag->load_base_addr);
}

void boot_info::dump(logger& in_log, const void * in_boot_info) {
    auto cur_ptr = (uint8_t *)in_boot_info;
    auto total_size = *(multiboot_uint32_t *)cur_ptr;

    // Move past the total_size and reserved fields
    cur_ptr += ALIGN_8_BYTE(sizeof(multiboot_uint32_t) * 2);
    total_size -= ALIGN_8_BYTE(sizeof(multiboot_uint32_t) * 2);

    // Loop through the tags
    in_log.debug("Multiboot 2 Boot Info:");
    while(total_size > 0) {
        multiboot_tag * tag = (multiboot_tag *)cur_ptr;
        switch(tag->type) {
            case MULTIBOOT_TAG_TYPE_CMDLINE:
                _dump(in_log, (const multiboot_tag_cmdline *)cur_ptr);
                break;
            case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
                _dump(in_log, (const multiboot_tag_bootloader *)cur_ptr);
                break;
            case MULTIBOOT_TAG_TYPE_MMAP:
                _dump(in_log, (const multiboot_tag_mmap *)cur_ptr);
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
                in_log.debug("Skipping tag {02d} ({} bytes)", tag->type, tag->size);
                break;
        }
        cur_ptr += ALIGN_8_BYTE(tag->size);
        total_size -= ALIGN_8_BYTE(tag->size);
    }
}
