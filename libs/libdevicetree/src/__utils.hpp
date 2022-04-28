#ifndef _DEVICETREE_DETAILS_UTILS_HPP
#define _DEVICETREE_DETAILS_UTILS_HPP

#include <cstddef>
#include <cstdint>

namespace devicetree::details {

std::size_t align(const std::size_t in_num);
std::uint32_t be_to_host(std::uint32_t in_big_endian);
std::uint64_t be_to_host(std::uint64_t in_big_endian);

}; // namespace devicetree::details

#endif // _DEVICETREE_DETAILS_UTILS_HPP