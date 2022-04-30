/**
 * @file
 * @brief Templates to determine the relationship between two given types, as
 * defined by C++20 (N4849 draft) 20.15.6.
 * 
 * @note Some comments and implementations in this file were based off of or
 * taken directly from the descriptions on
 * https://en.cppreference.com/w/cpp/header/type_traits and various subpages.
 * All errors are mine, all credit is theirs.
 */
#ifndef _STD_TYPE_TRAITS_RELATIONSHIPS_HPP
#define _STD_TYPE_TRAITS_RELATIONSHIPS_HPP

#include <__type_traits/__helpers.hpp>

namespace std {

/**
 * @brief If type `From` can be converted to type `To` via implicit conversion
 * such that the function definition `To f() { return std::declval<From>(); }`
 * is well-formed, then member `value` is equal to `true`. Otherwise, `value`
 * is `false`.
 * 
 * @tparam From type which may be implicitly converted to type `To`
 * @tparam To type which may be implicitly converted from type `From`
 */
template <typename From, typename To>
struct is_convertible : std::bool_constant<__is_convertible(From, To)> {};

/**
 * @brief `True` if type `From` can be converted to type `To` via implicit
 * conversion such that the function definition
 * `To f() { return std::declval<From>(); }` is well-formed; otherwise,
 * `false`.
 * 
 * @tparam From type which may be implicitly converted to type `To`
 * @tparam To type which may be implicitly converted from type `From`
 */
template <typename From, typename To>
inline constexpr bool is_convertible_v = is_convertible<From, To>::value;

/**
 * @brief If `T` and `U` are the same type (taking into account const/volatile
 * qualifications), then `value` is `true`. Otherwise, `value` is `false`.
 *
 * @details
 * Commutativity is satisfied; i.e. for any two types `T` and `U`, the
 * following must hold:
 *   is_same<T, U>::value == is_same<U, T>::value
 * 
 * @tparam T type with optional const/volatile qualifiers
 * @tparam U type with optional const/volatile qualifiers
 * @param value `true` if `T` and `U` are the same type, taking into account
 * const/volatile qualifications; `false` otherwise
 */
template <typename T, typename U>
struct is_same : std::false_type {};
 
 /**
 * @brief If `T` and `U` are the same type (taking into account const/volatile
 * qualifications), then `value` is `true`. Otherwise, `value` is `false`.
 *
 * @details
 * Commutativity is satisfied; i.e. for any two types `T` and `U`, the
 * following must hold:
 *   is_same<T, U>::value == is_same<U, T>::value
 * 
 * @tparam T type with optional const/volatile qualifiers
 * @tparam U type with optional const/volatile qualifiers
 * @param value `true` if `T` and `U` are the same type, taking into account
 * const/volatile qualifications; `false` otherwise
 */
template <typename T>
struct is_same<T, T> : std::true_type {};

/**
 * @brief `True` if `T` and `U` are the same type, taking into account const/
 * volatile qualifications; otherwise, `false`.
 * 
 * @tparam T type with optional const/volatile qualifiers
 * @tparam U type with optional const/volatile qualifiers
 */
template <typename T, typename U>
inline constexpr bool is_same_v = is_same<T, U>::value;

}; // namespace std

// UNIMPLEMENTED TRAITS
// template<class Base, class Derived> struct is_base_of;
// template<class From, class To> struct is_nothrow_convertible;
// template<class T, class U> struct is_layout_compatible;
// template<class Base, class Derived> struct is_pointer_interconvertible_base_of;
// template<class Fn, class... ArgTypes> struct is_invocable;
// template<class R, class Fn, class... ArgTypes> struct is_invocable_r;
// template<class Fn, class... ArgTypes> struct is_nothrow_invocable;
// template<class R, class Fn, class... ArgTypes> struct is_nothrow_invocable_r;
// template<class Base, class Derived>
// inline constexpr bool is_base_of_v = is_base_of<Base, Derived>::value;
// template<class From, class To>
// inline constexpr bool is_nothrow_convertible_v = is_nothrow_convertible<From, To>::value;
// template<class T, class U>
// inline constexpr bool is_layout_compatible_v = is_layout_compatible<T, U>::value;
// template<class Base, class Derived>
// inline constexpr bool is_pointer_interconvertible_base_of_v = is_pointer_interconvertible_base_of<Base, Derived>::value;
// template<class Fn, class... ArgTypes>
// inline constexpr bool is_invocable_v = is_invocable<Fn, ArgTypes...>::value;
// template<class R, class Fn, class... ArgTypes>
// inline constexpr bool is_invocable_r_v = is_invocable_r<R, Fn, ArgTypes...>::value;
// template<class Fn, class... ArgTypes>
// inline constexpr bool is_nothrow_invocable_v = is_nothrow_invocable<Fn, ArgTypes...>::value;
// template<class R, class Fn, class... ArgTypes>
// inline constexpr bool is_nothrow_invocable_r_v = is_nothrow_invocable_r<R, Fn, ArgTypes...>::value;

#endif // _STD_TYPE_TRAITS_RELATIONSHIPS_HPP