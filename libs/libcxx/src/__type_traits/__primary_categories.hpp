/**
 * @file
 * @brief Implementations and internal helpers for the primary type category
 * templates and functions, as defined by C++20 (N4849 draft) 20.15.4.1.
 * 
 * @note Some comments and implementations in this file were based off of or
 * taken directly from the descriptions on
 * https://en.cppreference.com/w/cpp/header/type_traits and various subpages.
 * All errors are mine, all credit is theirs.
 */
#ifndef _STD_TYPE_TRAITS_PRIMARY_CATEGORIES_HPP
#define _STD_TYPE_TRAITS_PRIMARY_CATEGORIES_HPP

#include <__type_traits/__helpers.hpp>
#include <__type_traits/__reference_modifiers.hpp>

namespace std {

namespace details {

template <typename T>
struct __is_void : public false_type {};

template <>
struct __is_void<void> : public true_type {};

}; // namespace details

/**
 * @brief If `T` is an lvalue reference type, member `value` is `true`.
 * Otherwise, member `value` is `false`.
 * 
 * @tparam T possible lvalue reference type
 */
template <typename T>
struct is_lvalue_reference : false_type {};

/**
 * @brief If `T` is an lvalue reference type, member `value` is `true`.
 * Otherwise, member `value` is `false`.
 * 
 * @tparam T possible lvalue reference type
 */
template <typename T>
struct is_lvalue_reference<T&> : true_type {};

/**
 * @brief `True` if `T` is an lvalue reference type; otherwise, `false`.
 * 
 * @tparam T possible lvalue reference type
 */
template <typename T>
inline constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;

/**
 * @brief If `T` is an rvalue reference type, member `value` is `true`.
 * Otherwise, member `value` is `false`.
 * 
 * @tparam T possible rvalue reference type
 */
template <typename T>
struct is_rvalue_reference : false_type {};

/**
 * @brief If `T` is an rvalue reference type, member `value` is `true`.
 * Otherwise, member `value` is `false`.
 * 
 * @tparam T possible rvalue reference type
 */
template <typename T>
struct is_rvalue_reference<T&&> : true_type {};

/**
 * @brief `True` if `T` is an rvalue reference type; otherwise, `false`.
 * 
 * @tparam T possible rvalue reference type
 */
template <typename T>
inline constexpr bool is_rvalue_reference_v = is_rvalue_reference<T>::value;

/**
 * @brief If `T` is a `void` type with optional const/volatile qualifiers, then
 * `value` is `true`. Otherwise, `value` is `false`.
 *
 * @details
 * The types considered void that result in `value` being `true` are:
 *
 * - `void`
 * - `const void`
 * - `volatile void`
 * - `const volatile void`
 *
 * All other types result in `value` being `false`.
 * 
 * @tparam T possible void type with optional const/volatile qualifiers
 * @param value `true` if `T` is a void type with optional const/volatile
 * qualifiers; `false` otherwise
 */
template <typename T>
struct is_void : public details::__is_void<std::remove_cv_t<T>>::type {};

/**
 * @brief `True` if `T` is a void type with optional const/volatile qualifiers;
 * otherwise, `false`.
 * 
 * @tparam T possible void type with optional const/volatile qualifiers
 */
template <typename T>
inline constexpr bool is_void_v = is_void<T>::value;

}; // namespace std

#endif // _STD_TYPE_TRAITS_PRIMARY_CATEGORIES_HPP