#ifndef _CORE_MEMORY_TYPES_HPP
#define _CORE_MEMORY_TYPES_HPP

#include <cassert>
#include <cstdint>
#include <format>
#include "../console/console.hpp"
#include "heap.hpp"

namespace core::memory {

typedef std::uint8_t * physical_addr_t;

};

template <>
struct std::formatter<core::memory::physical_addr_t> : public std::formatter<std::uint64_t> {
    string format(core::memory::physical_addr_t in_arg) {
        return std::formatter<std::uint64_t>::format((std::uint64_t)in_arg);
    }
};

#endif // _CORE_MEMORY_TYPES_HPP