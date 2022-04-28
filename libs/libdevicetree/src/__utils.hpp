#ifndef _DEVICETREE_DETAILS_UTILS_HPP
#define _DEVICETREE_DETAILS_UTILS_HPP

#include <bit>
#include <concepts>
#include <cstddef>

namespace devicetree::details {

constexpr std::size_t align(const std::size_t in_num) {
    auto remainder = in_num % 4;
    return (in_num + ((4 - remainder) % 4));
}

template<std::integral T>
constexpr T be_to_host(T in_big_endian) {
    switch(std::endian::native) {
    case std::endian::little:
        return std::byteswap(in_big_endian);
    case std::endian::big:
        return in_big_endian;
    default:
        static_assert(std::endian::native == std::endian::big || std::endian::native == std::endian::little, "Mixed-endian hosts not supported");
    }
}

}; // namespace devicetree::details

#endif // _DEVICETREE_DETAILS_UTILS_HPP