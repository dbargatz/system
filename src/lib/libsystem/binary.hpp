#ifndef _LOADER_BINARY_HPP
#define _LOADER_BINARY_HPP

#include <cstdint>
#include "../logging/logger.hpp"

namespace loader {

class binary {
public:
    binary(logging::logger& in_logger) : _log(in_logger) {}
    void dump();
    bool init(std::string* in_cmdline, const void * in_start_addr, const void * in_end_addr);
    void load();

private:
    logging::logger& _log;
    std::string* _cmdline = 0;
    std::uint64_t _elf_length = 0;
    const void * _elf_start_addr = 0;
    const void * _elf_end_addr = 0;
    const struct Elf64_Ehdr * _elf_header = nullptr;
};

}; //namespace loader

#endif // _LOADER_BINARY_HPP
