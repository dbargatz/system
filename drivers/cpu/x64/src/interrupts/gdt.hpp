#ifndef _INTERRUPTS_GDT_HPP
#define _INTERRUPTS_GDT_HPP

#include <cstdint>
#include "../logging/logger.hpp"

class gdt {
public:
    gdt();

    void dump(logging::logger& in_log);
    void install(std::uint8_t in_index, const void * in_base, std::uint32_t in_limit,
                 std::uint8_t in_access_byte, std::uint8_t in_flags);

private:
    struct _gdt_entry {
        std::uint64_t limit_0_15      : 16;
        std::uint64_t base_0_15       : 16;
        std::uint64_t base_16_23      : 8;
        std::uint64_t accessed        : 1;
        std::uint64_t read_write      : 1;
        std::uint64_t dir_conform     : 1;
        std::uint64_t executable      : 1;
        std::uint64_t descriptor_type : 1;
        std::uint64_t priv_level      : 2;
        std::uint64_t present         : 1;
        std::uint64_t limit_16_19     : 4;
        std::uint64_t reserved        : 1;
        std::uint64_t is_64bit_cs     : 1;
        std::uint64_t size_bit        : 1;
        std::uint64_t granularity     : 1;
        std::uint64_t base_24_31      : 8;
    } __attribute__((packed));

    struct _gdt {
        struct _gdt_entry entries[7];
    } __attribute__((packed));

    struct _gdt_descriptor {
        std::uint16_t limit;
        struct _gdt * offset;
    } __attribute__((packed));

    ///< Actual GDT for this core.
    struct _gdt& _our_gdt;

    void _dump_entry(logging::logger& in_log, const struct _gdt_entry& in_entry);
};

#endif // _INTERRUPTS_GDT_HPP
