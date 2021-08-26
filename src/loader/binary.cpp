#include "binary.hpp"

#include <cassert>

#include "__elf.hpp"

void loader::binary::dump() {
    _log.info(u8"\t\tStart Address: {:#016X}", (std::uint64_t)_elf_start_addr);
    _log.info(u8"\t\tEnd Address  : {:#016X}", (std::uint64_t)_elf_end_addr);
    _log.info(u8"\t\tLength       : {} bytes", _elf_length);
}

bool loader::binary::init(const void * in_start_addr, const void * in_end_addr) {
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

    _elf_header = file_hdr;
    _elf_start_addr = in_start_addr;
    _elf_end_addr = in_end_addr;
    _elf_length = (std::uint64_t)in_end_addr - (std::uint64_t)in_start_addr;
    return true;
}
