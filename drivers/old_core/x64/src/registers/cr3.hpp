#ifndef _CORE_X64_REGISTERS_CR3_HPP
#define _CORE_X64_REGISTERS_CR3_HPP

#include <cstdint>
#include <cstring>
#include <tuple>
#include "../memory/paging.hpp"

namespace core::x64::registers {

class cr3 {
private:
    union _cr3_fields { // assumes PCID disabled (CR4.PCIDE = 0), see Intel SDM Vol 3A tables 4-12/4-13
        struct {
            std::uint64_t _ignored_1         : 3;
            std::uint64_t write_through      : 1;
            std::uint64_t cache_disable      : 1;
            std::uint64_t _ignored_2         : 7;
            std::uint64_t pml4_physical_addr : (core::x64::memory::MAX_PHYSICAL_ADDRESS - 12);
            std::uint64_t _reserved          : (64 - core::x64::memory::MAX_PHYSICAL_ADDRESS);
        } __attribute__((packed));

        std::uint64_t raw;
    };

    union _cr3_fields _stored_value = { .raw=nullphysaddr };

public:
        cr3(std::uint64_t in_raw_value) {
            _stored_value.raw = in_raw_value;
        }

        constexpr cr3(physaddr_t in_pml4_addr, bool in_cache_disable, bool in_write_through) {
            _stored_value.pml4_physical_addr = in_pml4_addr >> 12;
            _stored_value.cache_disable = in_cache_disable ? 1 : 0;
            _stored_value.write_through = in_write_through ? 1 : 0;
        }

        const std::tuple<physaddr_t, bool, bool, std::uint64_t> value() const {
            auto pml4_addr = physaddr_t(_stored_value.pml4_physical_addr << 12);
            auto cd = (bool)_stored_value.cache_disable;
            auto wt = (bool)_stored_value.write_through;
            return { pml4_addr, cd, wt, _stored_value.raw };
        }

        static const cr3 get() {
            union _cr3_fields current = { .raw=nullphysaddr };
            // Use "=r" to move CR3's value into a register; "=m" (storing the
            // value of CR3 in memory instead of a register) is an invalid
            // encoding of "movq".
            asm volatile("movq %%cr3, %0" : "=r"(current.raw));
            return cr3(current.raw);
        }

        static cr3 set(const cr3& in_value) {
            auto old = cr3::get();
            asm volatile("movq %0, %%cr3" : : "r"(in_value._stored_value.raw));
            return old;
        }
};

}; // namespace core::x64::registers

template <>
struct std::formatter<core::x64::registers::cr3> {
    formatter() { }

    void parse(const string::value_type* in_open_brace,
               const string::value_type* in_close_brace) { }
    
    string format(const core::x64::registers::cr3& in_arg) {
        auto [pml4, cd, wt, raw] = in_arg.value();
        return std::format(u8"{:#016X} (PML4={:#X}, CD={}, WT={})", raw, pml4, cd, wt);
    }
};

#endif // _CORE_X64_REGISTERS_CR3_HPP
