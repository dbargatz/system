#include "binary.hpp"

#include <cassert.hpp>

#include "__elf.hpp"

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
    // TODO: verify ABI version

    assert(file_hdr->e_type    == 0x02); // Verify ET_EXEC
    assert(file_hdr->e_machine == 0x3E); // Verify machine is AMD64
    assert(file_hdr->e_version == 0x01); // Verify ELF spec version

    return true;
}
