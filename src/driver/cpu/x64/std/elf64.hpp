#ifndef _STD_ELF64_HPP
#define _STD_ELF64_HPP

#include "stdint.h"

typedef uint64_t Elf64_Addr;
typedef uint64_t Elf64_Off;

struct Elf64_Shdr {
    uint32_t   sh_name;
    uint32_t   sh_type;
    uint64_t   sh_flags;
    Elf64_Addr sh_addr;
    Elf64_Off  sh_offset;
    uint64_t   sh_size;
    uint32_t   sh_link;
    uint32_t   sh_info;
    uint64_t   sh_addralign;
    uint64_t   sh_entsize;
};

#endif // _STD_ELF64_HPP
