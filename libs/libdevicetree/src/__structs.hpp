#ifndef _DEVICETREE_INTERNAL_STRUCTS_HPP
#define _DEVICETREE_INTERNAL_STRUCTS_HPP

#include <cstdint>

namespace devicetree::internal {

struct fdt_header { 
    std::uint32_t magic;
    std::uint32_t totalsize;
    std::uint32_t off_dt_struct;
    std::uint32_t off_dt_strings;
    std::uint32_t off_mem_rsvmap;
    std::uint32_t version;
    std::uint32_t last_comp_version;
    std::uint32_t boot_cpuid_phys;
    std::uint32_t size_dt_strings;
    std::uint32_t size_dt_struct;
} __attribute__((packed));

struct fdt_memory_reserve_entry {
    std::uint64_t address;
    std::uint64_t size;
} __attribute__((packed));

struct fdt_begin_node {
    std::uint32_t token;
    char name[0];
} __attribute__((packed));

struct fdt_end_node {
    std::uint32_t token;
} __attribute__((packed));

struct fdt_property {
    std::uint32_t token;
    std::uint32_t len;
    std::uint32_t nameoff;
    char value[0];
} __attribute__((packed));

struct fdt_nop {
    std::uint32_t token;
} __attribute__((packed));

struct fdt_end {
    std::uint32_t token;
} __attribute__((packed));

}; // namespace devicetree::internal

#endif // _DEVICETREE_INTERNAL_STRUCTS_HPP