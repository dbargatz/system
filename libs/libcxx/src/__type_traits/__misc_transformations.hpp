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
 * @brief If `B` is `true` at compile-time, then member `type` is `T`.
 * Otherwise, `type` is `F`.
 *
 * @tparam B compile-time boolean value which is `true` or `false`
 * @tparam T type to set member `type` to if `B` is `true`
 * @tparam F type to set member `type` to if `B` is `false`
 * @param type `T` if `B` is `true`; otherwise, `F`
 */
template <bool B, typename T, typename F>
struct conditional { using type = T; };

/**
 * @brief If `B` is `true` at compile-time, then member `type` is `T`.
 * Otherwise, `type` is `F`.
 *
 * @tparam B compile-time boolean value which is `true` or `false`
 * @tparam T type to set member `type` to if `B` is `true`
 * @tparam F type to set member `type` to if `B` is `false`
 * @param type `T` if `B` is `true`; otherwise, `F`
 */
template <typename T, typename F>
struct conditional<false, T, F> { using type = F; };

/**
 * @brief `T` if `B` is `true` at compile-time; otherwise, `F`.
 * 
 * @tparam B compile-time boolean value which is `true` or `false`
 * @tparam T type if `B` is `true`
 * @tparam F type if `B` is `false`
 */
template <bool B, typename T, typename F>
using conditional_t = typename conditional<B,T,F>::type;

/**
 * @brief If `B` is `true` at compile-time, then member `type` is `T`.
 * Otherwise, `type` is not defined.
 *
 * @tparam B compile-time boolean value which is `true` or `false`
 * @tparam T type to set member `type` to if `B` is `true`
 * @param type `T` if `B` is `true`; otherwise, not defined
 */
template <bool B, typename T = void>
struct enable_if {};

/**
 * @brief If `B` is `true` at compile-time, then member `type` is `T`.
 * Otherwise, `type` is not defined.
 *
 * @tparam B compile-time boolean value which is `true` or `false`
 * @tparam T type to set member `type` to if `B` is `true`
 * @param type `T` if `B` is `true`; otherwise, not defined
 */
template <typename T>
struct enable_if<true, T> { using type = T; };

/**
 * @brief `T` if `B` is `true` at compile-time; otherwise, not defined.
 * 
 * @tparam B compile-time boolean value which is `true` or `false`
 * @tparam T type if `B` is `true`
 */
template <bool B, typename T = void>
using enable_if_t = typename enable_if<B,T>::type;

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