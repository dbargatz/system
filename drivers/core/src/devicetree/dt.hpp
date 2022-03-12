#ifndef _CORE_DEVICETREE_DT_HPP
#define _CORE_DEVICETREE_DT_HPP

#include <cstdint>
#include <format>
#include "../console/console.hpp"
#include "../memory/heap.hpp"
#include "../memory/types.hpp"

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

    std::uint32_t _be_to_le(std::uint32_t in_big_endian) const {
        auto be = (std::uint8_t *)&in_big_endian;
        std::uint32_t ret = (std::uint32_t)be[0] << 24;
        ret |= (std::uint32_t)be[1] << 16;
        ret |= (std::uint32_t)be[2] << 8;
        ret |= (std::uint32_t)be[3];
        return ret;
    }

    std::uint64_t _be_to_le(std::uint64_t in_big_endian) const {
        auto be = (std::uint8_t *)&in_big_endian;
        std::uint64_t ret = (std::uint64_t)be[0] << 56;
        ret |= (std::uint64_t)be[1] << 48;
        ret |= (std::uint64_t)be[2] << 40;
        ret |= (std::uint64_t)be[3] << 32;
        ret |= (std::uint64_t)be[4] << 24;
        ret |= (std::uint64_t)be[5] << 16;
        ret |= (std::uint64_t)be[6] << 8;
        ret |= (std::uint64_t)be[7];
        return ret;
    }

public:
    dtb(core::memory::physical_addr_t in_dtb) {
        _header = (struct _fdt_header *)in_dtb;

        auto mem_reserve_map_offset = (std::uint64_t)(_be_to_le(_header->off_mem_rsvmap));
        _mem_reserve_map = (struct _fdt_reserve_entry *)(in_dtb + mem_reserve_map_offset);
    }

    auto format() const {
        auto str = std::format(
            "Device Tree at 0x{:016X}:\n"
            "\tMagic          : 0x{:08X}\n"
            "\tTotal Size     : {} bytes\n"
            "\tVersion        : {} (min: {})\n"
            "\tMem Reserve Map: 0x{:016X}\n",
            (core::memory::physical_addr_t)_header, _be_to_le(_header->magic),
            _be_to_le(_header->totalsize), _be_to_le(_header->version),
            _be_to_le(_header->last_comp_version), _be_to_le(_header->off_mem_rsvmap));

        auto rsv_entry = _mem_reserve_map;
        while(true) {
            auto addr = _be_to_le(rsv_entry->address);
            auto size = _be_to_le(rsv_entry->size);
            if(addr == 0 && size == 0) {
                break;
            }
            str.append(std::format("\t  Entry: {:016X} ({} bytes)\n", addr, size));
            rsv_entry++;
        }

        auto struct_ptr = (struct _fdt_node *)((std::uint8_t *)_header + _be_to_le(_header->off_dt_struct));
        auto strings_block = (const char *)_header + _be_to_le(_header->off_dt_strings);
        bool finished = false;
        str.append(std::format("\tStruct Block   : 0x{:016X} ({} bytes)\n", _be_to_le(_header->off_dt_struct), _be_to_le(_header->size_dt_struct)));
        while(!finished) {
            auto token = _be_to_le(struct_ptr->token);

            switch(token) {
                case 0x01: {
                    // Print node name on BEGIN_NODE node.
                    auto node = (struct _fdt_begin_node *)struct_ptr;
                    auto name_len = std::strlen((const char *)node->name) + 1;
                    auto next = sizeof(*node) + core::memory::align_to((std::align_val_t)4, name_len);
                    auto nodestr = std::format("\t  Node: {}", (const char *)node->name);
                    str.append(nodestr);
                    struct_ptr = (struct _fdt_node *)((std::uint8_t *)struct_ptr + next);
                    break;
                }
                case 0x02: {
                    // Continue on END_NODE node.
                    // TODO: add asserts to make sure we're closing out an open node block.
                    struct_ptr++;
                    break;
                }
                case 0x03: {
                    auto node = (struct _fdt_prop *)struct_ptr;
                    auto name = strings_block + _be_to_le(node->nameoff);
                    auto len = _be_to_le(node->len);
                    auto val = (const char *)node->value;
                    auto next = sizeof(*node) + core::memory::align_to((std::align_val_t)4, len);
                    auto propstr = std::format("\t\t{}: {}", name, val);
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
                    // TODO: add asserts to make sure we're at the end of the struct block, and
                    //       that we've closed all BEGIN_NODEs with END_NODEs.
                    finished = true;
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