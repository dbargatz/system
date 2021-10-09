#ifndef _CORE_PLATFORM_RASPI3B_PLATFORM_HPP
#define _CORE_PLATFORM_RASPI3B_PLATFORM_HPP

#include <cstdint>

constexpr std::uint32_t MMIO_BASE_PHYSADDR = 0x3F000000;

#include "console/uart.hpp"

#endif // _CORE_PLATFORM_RASPI3B_PLATFORM_HPP