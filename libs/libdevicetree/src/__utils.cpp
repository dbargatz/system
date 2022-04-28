#include "__utils.hpp"

std::size_t devicetree::details::align(const std::size_t in_num) {
    auto remainder = in_num % 4;
    return (in_num + ((4 - remainder) % 4));
}

std::uint32_t devicetree::details::be_to_host(std::uint32_t in_big_endian) {
    auto be = (std::uint8_t *)&in_big_endian;
    std::uint32_t ret = (std::uint32_t)be[0] << 24;
    ret |= (std::uint32_t)be[1] << 16;
    ret |= (std::uint32_t)be[2] << 8;
    ret |= (std::uint32_t)be[3];
    return ret;
}

std::uint64_t devicetree::details::be_to_host(std::uint64_t in_big_endian) {
    auto be = (std::uint8_t *)&in_big_endian;
    std::uint64_t ret = (std::uint64_t)be[0] << 56;
    ret |= (std::uint64_t)be[1] << 48;
    ret |= (std::uint64_t)be[2] << 40;
    ret |= (std::uint64_t)be[3] << 32;
    ret |= (std::uint64_t)be[4] << 24;
    ret |= (std::uint64_t)be[5] << 16;
    ret |= (std::uint64_t)be[6] << 8;
    ret |= (std::uint64_t)be[7];
    return ret;
}