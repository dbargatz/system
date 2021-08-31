#ifndef _CPU_X64_MEMORY_CR3_HPP
#define _CPU_X64_MEMORY_CR3_HPP

#include <cstdint>
#include <cstring>
#include <tuple>
#include "../../../../logging/logger.hpp"

namespace cpu::x64::memory {

constexpr static const std::uint8_t MAX_PHYSICAL_ADDRESS = 52;
using physaddr_t = std::uint64_t;

class cr3 {
    public:
        cr3(logging::logger& in_log) : _log(in_log) { }

        void dump() {
            auto [raw, pml4, cd, wt] = get();
            _log.debug(u8"CR3:");
            _log.debug(u8"\tRaw Value         : {:#016X}", raw);
            _log.debug(u8"\tPML4 Physical Addr: {:#016X}", pml4);
            _log.debug(u8"\tCache Disable     : {}", cd);
            _log.debug(u8"\tWrite-Through     : {}", wt);
        }

        std::tuple<std::uint64_t, physaddr_t, bool, bool> get() {
            union _cr3_value value;
            std::memset(&value, 0, sizeof(value));

            // Use "=r" to move CR3's value into a register; "=m" (storing the
            // value of CR3 in memory instead of a register) is an invalid
            // encoding of "movq".
            asm volatile("movq %%cr3, %0" : "=r"(value.raw));
            auto pml4_addr = physaddr_t(value.pml4_physical_addr << 12);
            auto cd = (bool)value.cache_disable;
            auto wt = (bool)value.write_through;
            return { value.raw, pml4_addr, cd, wt };
        }

        void set(physaddr_t in_pml4_addr, bool in_cache_disable, bool in_write_through) {
            union _cr3_value value;
            std::memset(&value, 0, sizeof(value));

            value.pml4_physical_addr = in_pml4_addr >> 12;
            value.cache_disable = in_cache_disable ? 1 : 0;
            value.write_through = in_write_through ? 1 : 0;

            asm volatile("movq %0, %%cr3" : : "r"(value.raw));
        }

    private:
        union _cr3_value { // assumes PCID disabled (CR4.PCIDE = 0), see Intel SDM Vol 3A tables 4-12/4-13
            struct {
                std::uint64_t _ignored_1         : 3;
                std::uint64_t write_through      : 1;
                std::uint64_t cache_disable      : 1;
                std::uint64_t _ignored_2         : 7;
                std::uint64_t pml4_physical_addr : (MAX_PHYSICAL_ADDRESS - 12);
                std::uint64_t _reserved          : (64 - MAX_PHYSICAL_ADDRESS);
            } __attribute__((packed));

            std::uint64_t raw;
        };

        logging::logger&  _log;
};

}; // namespace cpu::x64::memory

#endif // _CPU_X64_MEMORY_CR3_HPP