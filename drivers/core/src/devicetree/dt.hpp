#ifndef _CORE_DEVICETREE_DT_HPP
#define _CORE_DEVICETREE_DT_HPP

#include <cstdint>
#include <format>
#include "../console/console.hpp"
#include "../memory/heap.hpp"
#include "../memory/types.hpp"
#include "utils.hpp"

namespace core::devicetree {

struct _fdt_header { 
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

struct _fdt_reserve_entry {
    std::uint64_t address;
    std::uint64_t size;
} __attribute__((packed));

struct _fdt_node {
    std::uint32_t token;
} __attribute__((packed));

struct _fdt_begin_node {
    std::uint32_t token;
    char name[0];
} __attribute__((packed));

struct _fdt_prop {
    std::uint32_t token;
    std::uint32_t len;
    std::uint32_t nameoff;
    char value[0];
} __attribute__((packed));

class dtb {
private:
    const struct _fdt_header * _header;
    const struct _fdt_reserve_entry * _mem_reserve_map;

public:
    dtb(core::memory::physical_addr_t in_dtb) {
        _header = (struct _fdt_header *)in_dtb;

        auto mem_reserve_map_offset = (std::uint64_t)(be_to_le(_header->off_mem_rsvmap));
        _mem_reserve_map = (struct _fdt_reserve_entry *)(in_dtb + mem_reserve_map_offset);
    }

    auto format() const {
        auto str = std::format(
            "Device Tree at 0x{:016X}:\n"
            "  Magic          : 0x{:08X}\n"
            "  Total Size     : {} bytes\n"
            "  Version        : {} (min: {})\n"
            "  Mem Reserve Map: 0x{:016X}\n",
            (core::memory::physical_addr_t)_header, be_to_le(_header->magic),
            be_to_le(_header->totalsize), be_to_le(_header->version),
            be_to_le(_header->last_comp_version), be_to_le(_header->off_mem_rsvmap));

        auto rsv_entry = _mem_reserve_map;
        while(true) {
            auto addr = be_to_le(rsv_entry->address);
            auto size = be_to_le(rsv_entry->size);
            if(addr == 0 && size == 0) {
                break;
            }
            str.append(std::format("    Entry: {:016X} ({} bytes)\n", addr, size));
            rsv_entry++;
        }

        str.append(std::format("  Struct Block   : 0x{:016X} ({} bytes)\n", be_to_le(_header->off_dt_struct), be_to_le(_header->size_dt_struct)));
        auto struct_ptr = (struct _fdt_node *)((std::uint8_t *)_header + be_to_le(_header->off_dt_struct));
        auto strings_block = (const char *)_header + be_to_le(_header->off_dt_strings);
        bool finished = false;
        auto depth = 0;
        while(!finished) {
            auto token = be_to_le(struct_ptr->token);

            switch(token) {
                case 0x01: {
                    // Print node name on BEGIN_NODE node.
                    auto node = (struct _fdt_begin_node *)struct_ptr;
                    auto name_len = std::strlen((const char *)node->name) + 1;
                    auto next = sizeof(*node) + core::memory::align_to((std::align_val_t)4, name_len);
                    auto indent = std::string(2 + (depth * 2), ' ');
                    auto nodestr = std::format("{}{}{\n", indent, (const char *)node->name);
                    str.append(nodestr);
                    depth++;
                    struct_ptr = (struct _fdt_node *)((std::uint8_t *)struct_ptr + next);
                    break;
                }
                case 0x02: {
                    // Continue on END_NODE node.
                    assert(depth > 0);
                    struct_ptr++;
                    depth--;
                    auto indent = std::string(2 + (depth * 2), ' ');
                    auto closestr = std::format("{}}\n", indent);
                    str.append(closestr);
                    break;
                }
                case 0x03: {
                    auto node = (struct _fdt_prop *)struct_ptr;
                    auto name = strings_block + be_to_le(node->nameoff);
                    auto len = be_to_le(node->len);
                    auto val = (const char *)node->value;
                    auto next = sizeof(*node) + core::memory::align_to((std::align_val_t)4, len);
                    auto indent = std::string(2 + (depth * 2), ' ');
                    auto propstr = std::format("{}{}: {}\n", indent, name, val);
                    str.append(propstr);
                    struct_ptr = (struct _fdt_node *)((std::uint8_t *)struct_ptr + next);
                    break;
                }
                case 0x04: {
                    // Skip over and continue on NOPs.
                    struct_ptr++;
                    break;
                }
                case 0x09: {
                    // Stop looping on an END node.
                    finished = true;
                    assert(depth == 0);
                    break;
                }
                default: {
                    assert(false);
                }
            }
        }

        return str;
    }
};

};

template <>
struct std::formatter<core::devicetree::dtb> {
    formatter() { }

    void parse(const string::value_type* in_open_brace,
               const string::value_type* in_close_brace) { }

    string format(const core::devicetree::dtb& in_arg) {
        return in_arg.format();
    }
};

#endif // _CORE_DEVICETREE_DT_HPP