#ifndef _CORE_X64_MEMORY_PAGING_HPP
#define _CORE_X64_MEMORY_PAGING_HPP

#include <cstdint>
#include <format>

using physaddr_t = std::uint64_t;
constexpr static const physaddr_t nullphysaddr = (physaddr_t)0;

using virtaddr_t = std::uint64_t;
constexpr static const virtaddr_t nullvirtaddr = (virtaddr_t)0;

namespace core::x64::memory {

constexpr static const std::uint8_t  MAX_PHYSICAL_ADDRESS = 48;
constexpr static const std::uint16_t PAGE_SIZE_BYTES = 4096;
constexpr static const std::uint16_t PAGE_ALIGNMENT_BYTES = 4096;

class pml4 { // page map level 4 (CR3 contains physmem pointer to this table, only one in four-level paging)
private:
    struct _pml4_entry { // page map level 4 entry (physmem pointer to page directory pointer table)
        std::uint64_t present         : 1;
        std::uint64_t read_write      : 1;
        std::uint64_t user_supervisor : 1;
        std::uint64_t write_through   : 1;
        std::uint64_t cache_disable   : 1;
        std::uint64_t accessed        : 1;
        std::uint64_t _ignored_1      : 1;
        std::uint64_t _reserved_1     : 1;
        std::uint64_t _ignored_2      : 4;
        std::uint64_t pml3_physaddr   : (MAX_PHYSICAL_ADDRESS - 12);
        std::uint64_t _reserved_2     : (52 - MAX_PHYSICAL_ADDRESS);
        std::uint64_t _ignored_3      : 11;
        std::uint64_t execute_disable : 1;
    } __attribute__((packed));

    // in struct because it has to be 4KB page-aligned
    struct alignas(PAGE_ALIGNMENT_BYTES) _pml4 {
        struct _pml4_entry _entries[512];
    } __attribute__((packed));

    struct _pml4 * _pml4_val = nullptr;

public:
    pml4(void* in_existing_pml4 = nullptr);

    physaddr_t get_physaddr(virtaddr_t in_virtual_addr);
};

}; // namespace core::x64::memory

template <>
struct std::formatter<core::x64::memory::pml4> {
    formatter() { }

    void parse(const string::value_type* in_open_brace,
               const string::value_type* in_close_brace) { }
    
    string format(const core::x64::memory::pml4& in_arg) {
        return std::format(u8"{:#016X} (PML4)", (const void *)&in_arg);
    }
};

#endif // _CORE_X64_MEMORY_PAGING_HPP
