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

namespace std {

/**
 * @brief If `T` is a const-qualified type, member `value` is `true`; otherwise,
 * member `value` is `false`.
 * 
 * @tparam T possible const-qualified type
 */
template <typename T>
struct is_const : false_type {};

/**
 * @brief If `T` is a const-qualified type, member `value` is `true`; otherwise,
 * member `value` is `false`.
 * 
 * @tparam T possible const-qualified type
 */
template <typename T>
struct is_const<const T> : true_type {};

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
struct is_volatile : false_type {};

/**
 * @brief If `T` is a volatile-qualified type, member `value` is `true`; otherwise,
 * member `value` is `false`.
 * 
 * @tparam T possible volatile-qualified type
 */
template <typename T>
struct is_volatile<volatile T> : true_type {};

/**
 * @brief `True` if `T` is a volatile-qualified type; otherwise, `false`.
 * 
 * @tparam T possible volatile-qualified type
 */
template <typename T>
inline constexpr bool is_volatile_v = is_volatile<T>::value;

}; // namespace std

#endif // _STD_TYPE_TRAITS_PROPERTIES_HPP