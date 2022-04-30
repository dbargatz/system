/**
 * @file
 * @brief Implementations of the type property templates, as defined by C++20
 * (N4849 draft) 20.15.4.3.
 * 
 * @note Some comments and implementations in this file were based off of or
 * taken directly from the descriptions on
 * https://en.cppreference.com/w/cpp/header/type_traits and various subpages.
 * All errors are mine, all credit is theirs.
 */
#ifndef _STD_TYPE_TRAITS_PROPERTIES_HPP
#define _STD_TYPE_TRAITS_PROPERTIES_HPP

#include <__type_traits/__helpers.hpp>

namespace std {

/**
 * @brief If `T` is a const-qualified type, member `value` is `true`; otherwise,
 * member `value` is `false`.
 * 
 * @tparam T possible const-qualified type
 */
template <typename T>
struct is_const : std::false_type {};

/**
 * @brief If `T` is a const-qualified type, member `value` is `true`; otherwise,
 * member `value` is `false`.
 * 
 * @tparam T possible const-qualified type
 */
template <typename T>
struct is_const<const T> : std::true_type {};

/**
 * @brief `True` if `T` is a const-qualified type; otherwise, `false`.
 * 
 * @tparam T possible const-qualified type
 */
template <typename T>
inline constexpr bool is_const_v = is_const<T>::value;

/**
 * @brief If `T` is a volatile-qualified type, member `value` is `true`; otherwise,
 * member `value` is `false`.
 * 
 * @tparam T possible volatile-qualified type
 */
template <typename T>
struct is_volatile : std::false_type {};

/**
 * @brief If `T` is a volatile-qualified type, member `value` is `true`; otherwise,
 * member `value` is `false`.
 * 
 * @tparam T possible volatile-qualified type
 */
template <typename T>
struct is_volatile<volatile T> : std::true_type {};

/**
 * @brief `True` if `T` is a volatile-qualified type; otherwise, `false`.
 * 
 * @tparam T possible volatile-qualified type
 */
template <typename T>
inline constexpr bool is_volatile_v = is_volatile<T>::value;

}; // namespace std

// UNIMPLEMENTED TRAITS:
// template<class T> struct is_trivial;
// template<class T> struct is_trivially_copyable;
// template<class T> struct is_standard_layout;
// template<class T> struct is_empty;
// template<class T> struct is_polymorphic;
// template<class T> struct is_abstract;
// template<class T> struct is_final;
// template<class T> struct is_aggregate;
// template<class T> struct is_signed;
// template<class T> struct is_unsigned;
// template<class T> struct is_bounded_array;
// template<class T> struct is_unbounded_array;
// template<class T> struct is_scoped_enum;
// template<class T>
// inline constexpr bool is_trivial_v = is_trivial<T>::value;
// template<class T>
// inline constexpr bool is_trivially_copyable_v = is_trivially_copyable<T>::value;
// template<class T>
// inline constexpr bool is_standard_layout_v = is_standard_layout<T>::value;
// template<class T>
// inline constexpr bool is_empty_v = is_empty<T>::value;
// template<class T>
// inline constexpr bool is_polymorphic_v = is_polymorphic<T>::value;
// template<class T>
// inline constexpr bool is_abstract_v = is_abstract<T>::value;
// template<class T>
// inline constexpr bool is_final_v = is_final<T>::value;
// template<class T>
// inline constexpr bool is_aggregate_v = is_aggregate<T>::value;
// template<class T>
// inline constexpr bool is_signed_v = is_signed<T>::value;
// template<class T>
// inline constexpr bool is_unsigned_v = is_unsigned<T>::value;
// template<class T>
// inline constexpr bool is_bounded_array_v = is_bounded_array<T>::value;
// template<class T>
// inline constexpr bool is_unbounded_array_v = is_unbounded_array<T>::value;
// template<class T>
// inline constexpr bool is_scoped_enum_v = is_scoped_enum<T>::value;

#endif // _STD_TYPE_TRAITS_PROPERTIES_HPP