#ifndef _INTERRUPTS_GDT_HPP
#define _INTERRUPTS_GDT_HPP

#include "../std/logger.hpp"
#include "../std/stdint.h"
#include "../std/text.hpp"

class gdt {
public:
    gdt(logger& in_log);

    void dump();
    void install(uint8_t in_index, const void * in_base, uint32_t in_limit,
                 uint8_t in_access_byte, uint8_t in_flags);

private:
    struct _gdt_entry {
        uint64_t limit_0_15      : 16;
        uint64_t base_0_15       : 16;
        uint64_t base_16_23      : 8;
        uint64_t accessed        : 1;
        uint64_t read_write      : 1;
        uint64_t dir_conform     : 1;
        uint64_t executable      : 1;
        uint64_t descriptor_type : 1;
        uint64_t priv_level      : 2;
        uint64_t present         : 1;
        uint64_t limit_16_19     : 4;
        uint64_t reserved        : 1;
        uint64_t is_64bit_cs     : 1;
        uint64_t size_bit        : 1;
        uint64_t granularity     : 1;
        uint64_t base_24_31      : 8;
    } __attribute__((packed));

    struct _gdt {
        struct _gdt_entry entries[7];
    } __attribute__((packed));

    struct _gdt_descriptor {
        uint16_t limit;
        struct _gdt * offset;
    } __attribute__((packed));

    ///< Logger used by class.
    logger& _log;

    ///< Actual GDT for this core.
    struct _gdt& _our_gdt;

    void _dump_entry(const struct _gdt_entry& in_entry);
};

#endif // _INTERRUPTS_GDT_HPP