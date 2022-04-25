#ifndef _DEVICETREE_INTERNAL_UTILS_HPP
#define _DEVICETREE_INTERNAL_UTILS_HPP

#include <cstddef>
#include <cstdint>

namespace devicetree::internal {

std::size_t align(const std::size_t in_num);
std::uint32_t be_to_le(std::uint32_t in_big_endian);
std::uint64_t be_to_le(std::uint64_t in_big_endian);

}; // namespace devicetree::internal

#endif // _DEVICETREE_INTERNAL_UTILS_HPP