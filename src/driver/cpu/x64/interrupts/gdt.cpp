#include "gdt.hpp"

extern struct _gdt gdt64;

gdt::gdt(logger& in_log) : _log(in_log), _our_gdt(*(struct _gdt *)&gdt64) { }

void gdt::dump() {
    _log.debug("GDT: {#016X}", (uint64_t)&gdt64);
    for(auto& entry : _our_gdt.entries) {
        _dump_entry(entry);
    }
}

void gdt::install(uint8_t in_index, const void * in_base, uint32_t in_limit,
                  uint8_t in_access_byte, uint8_t in_flags) {
    auto idx = in_index / sizeof(_our_gdt.entries[0]);
    auto base = (uint64_t)in_base;
    struct _gdt_entry& entry = (_our_gdt.entries[idx]);
    entry.base_0_15       = (base & 0x0000FFFF) >> 0;
    entry.base_16_23      = (base & 0x00FF0000) >> 16;
    entry.base_24_31      = (base & 0xFF000000) >> 24;
    entry.limit_0_15      = (in_limit & 0x0000FFFF) >> 0;
    entry.limit_16_19     = (in_limit & 0x00FF0000) >> 16;
    entry.accessed        = (in_access_byte & 0x01) >> 0;
    entry.read_write      = (in_access_byte & 0x02) >> 1;
    entry.dir_conform     = (in_access_byte & 0x04) >> 2;
    entry.executable      = (in_access_byte & 0x08) >> 3;
    entry.descriptor_type = (in_access_byte & 0x10) >> 4;
    entry.priv_level      = (in_access_byte & 0x60) >> 5;
    entry.present         = (in_access_byte & 0x80) >> 7;
    entry.reserved        = (in_flags & 0x01) >> 0;
    entry.is_64bit_cs     = (in_flags & 0x02) >> 1;
    entry.size_bit        = (in_flags & 0x04) >> 2;
    entry.granularity     = (in_flags & 0x08) >> 3;
}

void gdt::_dump_entry(const struct _gdt_entry& in_entry) {
    auto entry_idx = (uint16_t)((uint8_t*)&in_entry - (uint8_t*)&_our_gdt);
    auto limit = in_entry.limit_0_15 + (in_entry.limit_16_19 << 16);
    auto base = in_entry.base_0_15  +
        (in_entry.base_16_23 << 16) +
        (in_entry.base_24_31 << 24);

    uint8_t access = in_entry.accessed  +
        (in_entry.read_write      << 1) +
        (in_entry.dir_conform     << 2) +
        (in_entry.executable      << 3) +
        (in_entry.descriptor_type << 4) +
        (in_entry.priv_level      << 5) +
        (in_entry.present         << 7);

    uint8_t flags = in_entry.reserved +
        (in_entry.is_64bit_cs << 1)   +
        (in_entry.size_bit    << 2)   +
        (in_entry.granularity << 3);

    _log.debug("\t{#04X}: {016X}:{04X} Access: {02X} Flags: {01X}", entry_idx,
        (uint64_t)base, (uint32_t)limit, access, flags);
}
