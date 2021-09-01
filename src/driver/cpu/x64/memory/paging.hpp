#ifndef _CPU_X64_MEMORY_PAGING_HPP
#define _CPU_X64_MEMORY_PAGING_HPP

#include <cstdint>
#include "../../../../logging/logger.hpp"

using physaddr_t = std::uint64_t;
constexpr static const physaddr_t nullphysaddr = (physaddr_t)0;

namespace cpu::x64::memory {

constexpr static const std::uint8_t MAX_PHYSICAL_ADDRESS = 52;

class _pml_base {
    public:
        _pml_base(logging::logger& in_log);
        virtual ~_pml_base();

        virtual void dump() = 0;
};

class pml4 : public _pml_base { // page map level 4 (CR3 contains physmem pointer to this table, only one in four-level paging)
    public:
        pml4(logging::logger& in_log);

        void dump();

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
        } __attribute__((packed));

        struct _pml4 { // in struct because it has to be 4KB page-aligned
            struct _pml4_entry _entries[512];
        } __attribute__((packed));
};

class pml3 : public _pml_base { // page directory pointer table
    public:
        pml3(logging::logger& in_log);

        void dump();

    private:
        struct _pml3_entry { // page directory pointer table entry (physmem pointer to page directory)

        } __attribute__((packed));

        struct _pml3 { // in struct because it has to be 4KB page-aligned
            struct _pml3_entry _entries[512];
        } __attribute__((packed));
};

class pml2 : public _pml_base { // page directory
    public:
        pml2(logging::logger& in_log);

        void dump();

    private:
        struct _pml2_entry { // page directory entry (physmem pointer to page table)

        } __attribute__((packed));

        struct _pml2 { // in struct because it has to be 4KB page-aligned
            struct _pml2_entry _entries[512];
        } __attribute__((packed));
};

class pml1 : public _pml_base { // page table
    public:
        pml1(logging::logger& in_log);

        void dump();

    private:
        struct _pml1_entry { // page table entry (physmem pointer to page frame)

        } __attribute__((packed));

        struct _pml1 { // in struct because it has to be 4KB page-aligned
            struct _pml1_entry _entries[512];
        } __attribute__((packed));
};

}; // namespace cpu::x64::memory

#endif // _CPU_X64_MEMORY_PAGING_HPP
