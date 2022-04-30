/**
 * @file
 * @brief Implementations of the helper classes which form the base of the C++
 * type traits. As such, this file includes no other headers - it's included
 * by many other parts of the metaprogramming/type traits family, though.
 * 
 * @note Some comments and implementations in this file were based off of or
 * taken directly from the descriptions on
 * https://en.cppreference.com/w/cpp/header/type_traits and various subpages.
 * All errors are mine, all credit is theirs.
 */
#ifndef _STD_TYPE_TRAITS_HELPERS_HPP
#define _STD_TYPE_TRAITS_HELPERS_HPP

namespace std {

/**
 * @brief Wraps a static constant `v` of given type `T`. Serves as the base
 * class for all C++ type traits.
 * 
 * @details
 * Generally used to implement other type traits, but not useful on its own
 * otherwise.
 * 
 * @tparam T type of the constant
 * @tparam v value of the constant
 */
template <typename T, T v>
struct integral_constant {
    constexpr static T value = v;
    using value_type = T;
    using type = integral_constant;

    constexpr operator value_type() const noexcept { return value; }
    constexpr value_type operator ()() const noexcept { return value; }
};

/**
 * @brief Helper alias template for the common case where `T` is bool; in
 * other words, defines `std::integral_constant<bool, v>`, where `v` is either
 * `true` or `false`.
 * 
 * @see std::false_type
 * @see std::true_type
 * 
 * @tparam v `true` or `false`
 */
template <bool v>
using bool_constant = integral_constant<bool, v>;

/**
 * @brief Helper for the common case where `T` is `bool` and `v` is `false`; in
 * other words, defines `std::integral_constant<bool, false>`.
 */
using false_type = bool_constant<false>;

/**
 * @brief Helper for the common case where `T` is `bool` and `v` is `true`; in
 * other words, defines `std::integral_constant<bool, true`>`.
 */
using true_type  = bool_constant<true>;

}; // namespace std

#endif // _STD_TYPE_TRAITS_HELPERS_HPP