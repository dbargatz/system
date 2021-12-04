#ifndef _CORE_PLATFORM_RASPI3B_PLATFORM_HPP
#define _CORE_PLATFORM_RASPI3B_PLATFORM_HPP

#include <cstdint>

constexpr std::uint32_t MMIO_BASE_PHYSADDR = 0x3F000000;

constexpr static const std::size_t PAGE_SIZE_BYTES = 4096;

#endif // _CORE_PLATFORM_RASPI3B_PLATFORM_HPP