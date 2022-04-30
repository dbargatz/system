/**
 * @file
 * @brief Templates to determine the operations that can be performed on a
 * given type, such as assignment and construction/destruction, as defined by
 * C++20 (N4849 draft) 20.15.4.3.
 * 
 * @note Some comments and implementations in this file were based off of or
 * taken directly from the descriptions on
 * https://en.cppreference.com/w/cpp/header/type_traits and various subpages.
 * All errors are mine, all credit is theirs.
 */
#ifndef _STD_TYPE_TRAITS_SUPPORTED_OPERATIONS_HPP
#define _STD_TYPE_TRAITS_SUPPORTED_OPERATIONS_HPP

#include <__type_traits/__helpers.hpp>

namespace std {

/**
 * @brief If `T` is an object or reference type, and the variable definition
 * `T obj(std::declval<Args>()...)` is well-formed, then the member `value` is
 * equal to `true`; otherwise, member `value` is equal to `false`.
 * 
 * @note The `__is_constructible()` function used in the definition of this
 * template is a compiler intrinsic. Compiler support is required for a correct
 * and conforming implementation of `std::is_constructible`. For details, see
 * https://stackoverflow.com/a/65761122.
 * 
 * @tparam T possible constructible type
 * @tparam Args types of arguments a possible constructor of `T` would accept
 */
template <typename T, typename... Args>
struct is_constructible : std::bool_constant<__is_constructible(T, Args...)> {};

/**
 * @brief `True` if `T` is an object or reference type and the variable
 * definition `T obj(std::declval<Args>()...)` is well-formed; otherwise,
 * `false`.
 * 
 * @tparam T possible constructible type
 * @tparam Args types of arguments a possible constructor of `T` would accept
 */
template <typename T, typename... Args>
inline constexpr bool is_constructible_v = is_constructible<T, Args...>::value;

}; // namespace std

#endif // _STD_TYPE_TRAITS_SUPPORTED_OPERATIONS_HPP