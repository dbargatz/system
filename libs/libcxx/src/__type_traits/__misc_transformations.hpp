/**
 * @file
 * @brief Implementations of the miscellaneous/other transformations templates,
 * as defined by C++20 (N4849 draft) 20.15.7.6.
 * 
 * @note Some comments and implementations in this file were based off of or
 * taken directly from the descriptions on
 * https://en.cppreference.com/w/cpp/header/type_traits and various subpages.
 * All errors are mine, all credit is theirs.
 */
#ifndef _STD_TYPE_TRAITS_MISC_TRANSFORMATIONS_HPP
#define _STD_TYPE_TRAITS_MISC_TRANSFORMATIONS_HPP

namespace std {

/**
 * @brief Provides the member `type` with value `T`. Also known as the identity
 * transformation.
 * 
 * @tparam T type to set member `type` to
 */
template <typename T>
struct type_identity { using type = T; };

}; // namespace std

#endif // _STD_TYPE_TRAITS_MISC_TRANSFORMATIONS_HPP