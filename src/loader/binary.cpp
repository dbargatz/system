#include "binary.hpp"

#include <cassert>
#include <cstring>

#include "__elf.hpp"

typedef void(*jump_usermode_fn)(void (*)(void));
extern void* jump_usermode;

void loader::binary::dump() {
    _log.info(u8"\t\tCommand line : {}", *_cmdline);
    _log.info(u8"\t\tStart address: {:#016X}", (std::uint64_t)_elf_start_addr);
    _log.info(u8"\t\tEnd address  : {:#016X}", (std::uint64_t)_elf_end_addr);
    _log.info(u8"\t\tLength       : {} bytes", _elf_length);
}

bool loader::binary::init(std::string* in_cmdline, const void * in_start_addr, const void * in_end_addr) {
    auto file_hdr = (const struct Elf64_Ehdr *)in_start_addr;
    assert(file_hdr->e_ident[(std::uint8_t)Elf_Ident::EI_MAG0]       == 0x7F);
    assert(file_hdr->e_ident[(std::uint8_t)Elf_Ident::EI_MAG1]       == 'E');
    assert(file_hdr->e_ident[(std::uint8_t)Elf_Ident::EI_MAG2]       == 'L');
    assert(file_hdr->e_ident[(std::uint8_t)Elf_Ident::EI_MAG3]       == 'F');
    assert(file_hdr->e_ident[(std::uint8_t)Elf_Ident::EI_CLASS]      == 0x02); // Verify 64-bit
    assert(file_hdr->e_ident[(std::uint8_t)Elf_Ident::EI_DATA]       == 0x01); // Verify little-endian
    assert(file_hdr->e_ident[(std::uint8_t)Elf_Ident::EI_VERSION]    == 0x01); // Verify ELF spec version
    assert(file_hdr->e_ident[(std::uint8_t)Elf_Ident::EI_OSABI]      == 0x00); // Verify OS ABI is System V
    assert(file_hdr->e_ident[(std::uint8_t)Elf_Ident::EI_ABIVERSION] == 0x00); // Verify System V ABI version

    assert(file_hdr->e_type    == 0x02); // Verify ET_EXEC
    assert(file_hdr->e_machine == 0x3E); // Verify machine is AMD64
    assert(file_hdr->e_version == 0x01); // Verify ELF spec version

    _cmdline = in_cmdline;
    _elf_header = file_hdr;
    _elf_start_addr = in_start_addr;
    _elf_end_addr = in_end_addr;
    _elf_length = (std::uint64_t)in_end_addr - (std::uint64_t)in_start_addr;
    return true;
}

void loader::binary::load() {
    auto prog_tbl = (std::uint8_t *)_elf_header + _elf_header->e_phoff;
    auto prog_hdr = (const struct Elf64_Phdr *)prog_tbl;
    auto prog_tbl_end = prog_hdr + _elf_header->e_phnum;
    
    while(prog_hdr < prog_tbl_end) {
        if(prog_hdr->p_type != loader::ProgHdrType::PT_LOAD) {
            prog_hdr++;
            continue;
        }
        auto vaddr = (std::uint8_t *)prog_hdr->p_vaddr;
        auto offset = (std::uint8_t *)_elf_header + prog_hdr->p_offset;
        std::memcpy(vaddr, offset, prog_hdr->p_filesz);
        if(prog_hdr->p_memsz > prog_hdr->p_filesz) {
            auto end_addr = vaddr + prog_hdr->p_filesz;
            auto amount = prog_hdr->p_memsz - prog_hdr->p_filesz;
            std::memset(end_addr, 0, amount);
        }
        prog_hdr++;
    }

    _log.info(u8"Heading to user mode...");
    auto entry = (void(*)(void))(_elf_header->e_entry);
    ((jump_usermode_fn)&jump_usermode)(entry);
}
