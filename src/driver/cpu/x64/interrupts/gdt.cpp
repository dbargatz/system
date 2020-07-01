#include "gdt.hpp"

extern struct _gdt gdt64;

gdt::gdt(logger& in_log) : _log(in_log), _our_gdt(*(struct _gdt *)&gdt64) { }

void gdt::dump() {
    _log.debug("GDT           : {#016X}", (uint64_t)&gdt64);
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

    _dump_entry(entry);
}

void gdt::_dump_entry(const struct _gdt_entry& in_entry) {
    auto base = in_entry.base_0_15 + (in_entry.base_16_23 << 16) + (in_entry.base_24_31 << 24);
    auto limit = in_entry.limit_0_15 + (in_entry.limit_16_19 << 16);
    bool is_code = in_entry.executable;
    _log.debug("\t{#04X}:", (uint16_t)((uint8_t*)&in_entry - (uint8_t*)&_our_gdt));
    _log.debug("\t\tBase            : {#016X}", (uint64_t)base);
    _log.debug("\t\tLimit           : {#016X}", (uint32_t)limit);
    _log.debug("\t\tAccessed        : {}", in_entry.accessed ? "Yes" : "No");
    if(is_code) {
        _log.debug("\t\tRead-Accessible : {}", in_entry.read_write ? "Readable" : "Unreadable");
        _log.debug("\t\tConforming      : {}", in_entry.dir_conform ? "Yes" : "No");
    } else {
        _log.debug("\t\tRead/Write      : {}", in_entry.read_write ? "R/W" : "Read-Only");
        _log.debug("\t\tDirection       : {}", in_entry.dir_conform ? "Down" : "Up");
    }

    _log.debug("\t\tExecutable      : {}", in_entry.executable ? "Yes" : "No");
    _log.debug("\t\tDescriptor Type : {}", in_entry.descriptor_type ? "Code/Data" : "System");
    _log.debug("\t\tPrivilege Level : {}", (uint8_t)in_entry.priv_level);
    _log.debug("\t\tPresent         : {}", in_entry.present ? "Yes" : "No");
    _log.debug("\t\t64bit Code Seg  : {}", in_entry.is_64bit_cs ? "Yes" : "No");
    _log.debug("\t\tSize Bit        : {}", in_entry.size_bit ? "Set" : "Clear");
    _log.debug("\t\tGranularity     : {}", in_entry.size_bit ? "4KiB" : "1 byte");
}
