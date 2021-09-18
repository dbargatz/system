#ifndef _LOADER_ELF_HPP
#define _LOADER_ELF_HPP

#include <cstdint>

namespace loader {

enum class Elf_Ident : std::uint8_t {
    EI_MAG0 = 0,
    EI_MAG1,
    EI_MAG2,
    EI_MAG3,
    EI_CLASS,
    EI_DATA,
    EI_VERSION,
    EI_OSABI,
    EI_ABIVERSION,
    EI_PAD,
    EI_MAX = 16
};

enum class ProgHdrType : std::uint32_t {
    PT_NULL = 0,
    PT_LOAD,
    PT_DYNAMIC,
    PT_INTERP,
    PT_NOTE,
    PT_SHLIB,
    PT_PHDR,
    PT_LOOS = 0x60000000,
    PT_HIOS = 0x6FFFFFFF,
    PT_LOPROC = 0x70000000,
    PT_HIPROC = 0x7FFFFFFF,
};

struct Elf64_Ehdr {
    std::uint8_t  e_ident[(std::uint8_t)Elf_Ident::EI_MAX];
    std::uint16_t e_type;
    std::uint16_t e_machine;
    std::uint32_t e_version;
    std::uint64_t e_entry;
    std::uint64_t e_phoff;
    std::uint64_t e_shoff;
    std::uint32_t e_flags;
    std::uint16_t e_ehsize;
    std::uint16_t e_phentsize;
    std::uint16_t e_phnum;
    std::uint16_t e_shentsize;
    std::uint16_t e_shnum;
    std::uint16_t e_shstrndx;
};

struct Elf64_Phdr {
    ProgHdrType p_type;
    std::uint32_t p_flags;
    std::uint64_t p_offset;
    std::uint64_t p_vaddr;
    std::uint64_t p_paddr;
    std::uint64_t p_filesz;
    std::uint64_t p_memsz;
    std::uint64_t p_align;
};

struct Elf64_Shdr {
    std::uint32_t sh_name;
    std::uint32_t sh_type;
    std::uint64_t sh_flags;
    std::uint64_t sh_addr;
    std::uint64_t sh_offset;
    std::uint64_t sh_size;
    std::uint32_t sh_link;
    std::uint32_t sh_info;
    std::uint64_t sh_addralign;
    std::uint64_t sh_entsize;
};

}; // namespace loader::elf

#endif // _LOADER_ELF_HPP
