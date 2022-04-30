/**
 * @file
 * @brief Implementation of `std::is_reference`, as defined by C++20 (N4849
 * draft) 20.15.4.2.
 * 
 * @details
 * This is pulled into its own header because some primary type categories
 * such as `std::is_function` are defined much more easily with
 * `std::is_reference`, but <__type_traits/__composite_categories.hpp> has to
 * include <__type_traits/__primary_categories.hpp>. To avoid the resulting
 * circular reference, `std::is_reference` is defined in this header, and
 * included by both primary and composite type category headers.
 * 
 * @note Some comments and implementations in this file were based off of or
 * taken directly from the descriptions on
 * https://en.cppreference.com/w/cpp/header/type_traits and various subpages.
 * All errors are mine, all credit is theirs.
 */
#ifndef _STD_TYPE_TRAITS_IS_REFERENCE_HPP
#define _STD_TYPE_TRAITS_IS_REFERENCE_HPP

#include <__type_traits/__helpers.hpp>

namespace std {

/**
 * @brief If `T` is an lvalue reference type or rvalue reference type, member
 * `value` is `true`. Otherwise, member `value` is `false`.
 * 
 * @tparam T possible lvalue reference or rvalue reference type
 */
template <typename T>
struct is_reference : std::false_type {};

/**
 * @brief If `T` is an lvalue reference type or rvalue reference type, member
 * `value` is `true`. Otherwise, member `value` is `false`.
 * 
 * @tparam T possible lvalue reference or rvalue reference type
 */
template <typename T>
struct is_reference<T&> : std::true_type {};

/**
 * @brief If `T` is an lvalue reference type or rvalue reference type, member
 * `value` is `true`. Otherwise, member `value` is `false`.
 * 
 * @tparam T possible lvalue reference or rvalue reference type
 */
template <typename T>
struct is_reference<T&&> : std::true_type {};

/**
 * @brief `True` if `T` is an lvalue reference type or rvalue reference type;
 * otherwise, `false`.
 * 
 * @tparam T possible lvalue reference or rvalue reference type
 */
template <typename T>
inline constexpr bool is_reference_v = is_reference<T>::value;

}; // namespace std

#endif // _STD_TYPE_TRAITS_IS_REFERENCE_HPP