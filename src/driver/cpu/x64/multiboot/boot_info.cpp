#include "boot_info.hpp"
#include "multiboot2.h"

#define ALIGN_8_BYTE(x) (x + ((x % 8) ? (8 - (x % 8)) : 0))

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
            {
                auto cmd_tag = (multiboot_tag_string *)cur_ptr;
                in_log.debug("\tCommand line: {}", cmd_tag->string);
                break;
            }
            case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
            {
                auto name_tag = (multiboot_tag_string *)cur_ptr;
                in_log.debug("\tBootloader name: {}", name_tag->string);
                break;
            }
            case MULTIBOOT_TAG_TYPE_MMAP:
            {
                auto mmap_tag = (multiboot_tag_mmap *)cur_ptr;
                uint64_t num_entries = (mmap_tag->size - sizeof(multiboot_tag_mmap)) / mmap_tag->entry_size;
                in_log.debug("\tMemory map ({} entries):", num_entries);
                for(auto i = 0; i < num_entries; i++) {
                    auto typestr = text("");
                    switch(mmap_tag->entries[i].type) {
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
                        mmap_tag->entries[i].addr,
                        mmap_tag->entries[i].addr + mmap_tag->entries[i].len - 1,
                        typestr, mmap_tag->entries[i].len);
                }
                break;
            }
            case MULTIBOOT_TAG_TYPE_APM:
            {
                auto apm_tag = (multiboot_tag_apm *)cur_ptr;
                in_log.debug("\tAPM Information (version {}):", apm_tag->version);
                in_log.debug("\t\t32-bit CS:IP: {#04X}:{#08X} ({} bytes)",
                    apm_tag->cseg, apm_tag->offset, apm_tag->cseg_len);
                in_log.debug("\t\t16-bit CS   : {#04X} ({} bytes)",
                    apm_tag->cseg_16, apm_tag->cseg_16_len);
                in_log.debug("\t\t16-bit DS   : {#04X} ({} bytes)",
                    apm_tag->dseg, apm_tag->dseg_len);
                in_log.debug("\t\tFlags       : {#04X}", apm_tag->flags);
                break;
            }
            case MULTIBOOT_TAG_TYPE_LOAD_BASE_ADDR:
            {
                auto load_tag = (multiboot_tag_load_base_addr *)cur_ptr;
                in_log.debug("\tImage load physical base address: {#016X}",
                    load_tag->load_base_addr);
                break;
            }
            default:
                in_log.debug("Skipping tag {02d} ({} bytes)", tag->type, tag->size);
                break;
        }
        cur_ptr += ALIGN_8_BYTE(tag->size);
        total_size -= ALIGN_8_BYTE(tag->size);
    }
}
