/**
 * @file
 * @brief Implementations of the composite type category templates, as defined
 * by C++20 (N4849 draft) 20.15.4.2. These are effectively compositions of the
 * most commonly-used primary type categories defined, so users can simply ask
 * questions like "is this type arithmetic" rather than having to ask "is this
 * type floating point or integral".
 * 
 * @note Some comments and implementations in this file were based off of or
 * taken directly from the descriptions on
 * https://en.cppreference.com/w/cpp/header/type_traits and various subpages.
 * All errors are mine, all credit is theirs.
 */
#ifndef _STD_TYPE_TRAITS_COMPOSITE_CATEGORIES_HPP
#define _STD_TYPE_TRAITS_COMPOSITE_CATEGORIES_HPP

#include <__type_traits/__helpers.hpp>

namespace std {

/**
 * @brief If `T` is an lvalue reference type or rvalue reference type, member
 * `value` is `true`. Otherwise, member `value` is `false`.
 * 
 * @tparam T possible lvalue reference or rvalue reference type
 */
template <typename T>
struct is_reference : false_type {};

/**
 * @brief If `T` is an lvalue reference type or rvalue reference type, member
 * `value` is `true`. Otherwise, member `value` is `false`.
 * 
 * @tparam T possible lvalue reference or rvalue reference type
 */
template <typename T>
struct is_reference<T&> : true_type {};

/**
 * @brief If `T` is an lvalue reference type or rvalue reference type, member
 * `value` is `true`. Otherwise, member `value` is `false`.
 * 
 * @tparam T possible lvalue reference or rvalue reference type
 */
template <typename T>
struct is_reference<T&&> : true_type {};

/**
 * @brief `True` if `T` is an lvalue reference type or rvalue reference type;
 * otherwise, `false`.
 * 
 * @tparam T possible lvalue reference or rvalue reference type
 */
template <typename T>
inline constexpr bool is_reference_v = is_reference<T>::value;

}; // namespace std

#endif // _STD_TYPE_TRAITS_COMPOSITE_CATEGORIES_HPP