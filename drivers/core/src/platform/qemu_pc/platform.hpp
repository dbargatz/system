#ifndef _CORE_PLATFORM_QEMU_PC_PLATFORM_HPP
#define _CORE_PLATFORM_QEMU_PC_PLATFORM_HPP

#include <cstddef>
#include <cstdint>

constexpr static const std::uint16_t COM1_IO_PORT = 0x03F8;

constexpr static const std::size_t PAGE_SIZE_BYTES = 4096;

std::uint32_t get_permission_level();

#endif // _CORE_PLATFORM_QEMU_PC_PLATFORM_HPP