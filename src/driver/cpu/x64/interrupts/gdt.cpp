#include "gdt.hpp"

extern struct _gdt gdt64;

gdt::gdt(logger& in_log) : _log(in_log), _our_gdt(*(struct _gdt *)&gdt64) {
    // TODO: super-hacky way of hardcoding working TSS in for now.
    _our_gdt.tss.limit_0_15 = 104 - 1;
    _our_gdt.tss.base_0_15 = (uint16_t)(0x00108F50 & 0x0000FFFF);
    _our_gdt.tss.base_16_23 = (uint8_t)((0x00108F50 & 0x00FF0000) >> 16);
}

void gdt::dump() {
    _log.debug("GDT           : {#016X}", (uint64_t)&gdt64);
    _dump_entry("Null", _our_gdt.null);
    _dump_entry("Ring 0: Code", _our_gdt.ring0_code);
    _dump_entry("Ring 0: Data", _our_gdt.ring0_data);
    _dump_entry("Ring 3: Code", _our_gdt.ring3_code);
    _dump_entry("Ring 3: Data", _our_gdt.ring3_data);
    _dump_entry("TSS", _our_gdt.tss);
}

void gdt::_dump_entry(const text& in_name, struct _gdt_entry& in_entry) {
    uint16_t offset = (uint16_t)((uint8_t *)&in_entry - (uint8_t *)&_our_gdt);
    auto base = in_entry.base_0_15 + (in_entry.base_16_23 << 16) + (in_entry.base_24_31 << 24);
    auto limit = in_entry.limit_0_15 + (in_entry.limit_16_19 << 16);
    bool is_code = in_entry.executable;

    _log.debug("\t{}           : {#04X}", in_name, offset);
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
