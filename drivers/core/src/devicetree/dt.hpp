#ifndef _CORE_DEVICETREE_DT_HPP
#define _CORE_DEVICETREE_DT_HPP

#include <cstdint>
#include <format>
#include "../console/console.hpp"
#include "../memory/types.hpp"

namespace core::devicetree {

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

class dtb {
private:
    struct fdt_header _header;

    std::uint32_t _be_to_le(std::uint32_t in_big_endian) {
        auto be = (std::uint8_t *)&in_big_endian;
        std::uint32_t ret = (std::uint32_t)be[0] << 24;
        ret |= (std::uint32_t)be[1] << 16;
        ret |= (std::uint32_t)be[2] << 8;
        ret |= (std::uint32_t)be[3];
        return ret;
    }

    std::uint64_t _be_to_le(std::uint64_t in_big_endian) {
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
        auto header_big_endian = (std::uint32_t *)in_dtb;
        for(auto i = 0; i < 10; i++) {
            ((std::uint32_t *)&_header)[i] = _be_to_le(header_big_endian[i]);
        }
    }

    auto format() const {
        return std::format("Magic: {:08X} | Total Size: {} | Version: {} | Struct Block Offset/Size: {:08X}/{} | String Block Offset/Size: {:08X}/{}", _header.magic, _header.totalsize, _header.version, _header.off_dt_struct, _header.size_dt_struct, _header.off_dt_strings, _header.size_dt_strings);
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