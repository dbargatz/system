#ifndef _DEVICETREE_DETAILS_STRUCTS_HPP
#define _DEVICETREE_DETAILS_STRUCTS_HPP

#include <cstdint>

namespace devicetree::details {

constexpr std::uint32_t FDT_BEGIN_NODE = 0x00000001;
constexpr std::uint32_t FDT_END_NODE = 0x00000002;
constexpr std::uint32_t FDT_PROP = 0x00000003;
constexpr std::uint32_t FDT_NOP = 0x00000004;
constexpr std::uint32_t FDT_END = 0x00000009;

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

struct fdt_property {
    std::uint32_t token;
    std::uint32_t len;
    std::uint32_t nameoff;
    char value[0];
} __attribute__((packed));

struct fdt_reg {
    std::uint32_t address;
    std::uint32_t length;
} __attribute__((packed));

struct fdt_range {
    std::uint32_t child_bus_address;
    std::uint32_t parent_bus_address;
    std::uint32_t length;
} __attribute__((packed));

}; // namespace devicetree::details

#endif // _DEVICETREE_DETAILS_STRUCTS_HPP