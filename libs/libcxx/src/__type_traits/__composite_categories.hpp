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

#include <__type_traits/__cv_modifications.hpp>
#include <__type_traits/__helpers.hpp>
#include <__type_traits/__is_reference.hpp>
#include <__type_traits/__primary_categories.hpp>

namespace std {

namespace details {

template <typename T>
struct __is_member_pointer : std::false_type {};

template <typename T, typename U>
struct __is_member_pointer<T U::*> : std::true_type {};

}; // namespace details

/**
 * @brief If `T` is an integer or floating-point type, optionally with const
 * and/or volatile qualifiers, then member `value` is equal to `true`.
 * Otherwise, member `value` is equal to `false`.
 * 
 * @tparam T possible arithmetic type
 */
template <typename T>
struct is_arithmetic : std::bool_constant<std::is_integral_v<T> || std::is_floating_point_v<T>> {};

/**
 * @brief `True` if `T` is an integer or floating-point type with optional
 * const/volatile qualifiers; otherwise, `false`.
 * 
 * @tparam T possible arithmetic type
 */
template <typename T>
inline constexpr bool is_arithmetic_v = is_arithmetic<T>::value;

/**
 * @brief If `T` is an arithmetic, void, or null pointer type, optionally with
 * const and/or volatile qualifiers, then member `value` is equal to `true`.
 * Otherwise, member `value` is equal to `false`.
 * 
 * @tparam T possible fundamental type
 */
template <typename T>
struct is_fundamental : std::bool_constant<
                            std::is_arithmetic_v<T> ||
                            std::is_void_v<T> ||
                            std::is_null_pointer_v<T>
                        > {};

/**
 * @brief `True` if `T` is an arithmetic, void, or null pointer type with
 * optional const/volatile qualifiers; otherwise, `false`.
 * 
 * @tparam T possible fundamental type
 */
template <typename T>
inline constexpr bool is_fundamental_v = is_fundamental<T>::value;

/**
 * @brief If `T` is a pointer to a non-static member object/function, then
 * `value` is `true`. Otherwise, `value` is `false`.
 *
 * @details
 * Valid types may also have a const and/or volatile qualifier. All other types
 * result in `value` being `false`.
 * 
 * @tparam T possible pointer type to a non-static member object/function with
 * optional const/volatile qualifiers
 * @param value `true` if `T` is a pointer type to a non-static member object/
 * function with optional const/volatile qualifiers; `false` otherwise
 */
template <typename T>
struct is_member_pointer : details::__is_member_pointer<std::remove_cv_t<T>>::type {};

/**
 * @brief `True` if `T` is a pointer type to a non-static member object/
 * function with optional const/volatile qualifiers; otherwise, `false`.
 * 
 * @tparam T possible pointer type to a non-static member object/function with
 * optional const/volatile qualifiers
 */
template <typename T>
inline constexpr bool is_member_pointer_v = is_member_pointer<T>::value;

/**
 * @brief If `T` is an arithmetic, enum, pointer, member pointer, or null
 * pointer type, optionally with const and/or volatile qualifiers, then member
 * `value` is equal to `true`. Otherwise, member `value` is equal to `false`.
 * 
 * @tparam T possible scalar type
 */
template <typename T>
struct is_scalar : std::bool_constant<
                        std::is_arithmetic_v<T> ||
                        std::is_enum_v<T> ||
                        std::is_pointer_v<T> ||
                        std::is_member_pointer_v<T> ||
                        std::is_null_pointer_v<T>
                    > {};

/**
 * @brief `True` if `T` is an arithmetic, enum, pointer, member pointer, or
 * null pointer type with optional const/volatile qualifiers; otherwise,
 * `false`.
 * 
 * @tparam T possible scalar type
 */
template <typename T>
inline constexpr bool is_scalar_v = is_scalar<T>::value;

/**
 * @brief If `T` is any type other than function, reference, or void, and
 * optionally with const and/or volatile qualifiers, then member `value` is
 * equal to `true`. Otherwise, member `value` is equal to `false`.
 * 
 * @tparam T possible object type
 */
template <typename T>
struct is_object : std::bool_constant<
                        std::is_scalar_v<T> ||
                        std::is_array_v<T> ||
                        std::is_union_v<T> ||
                        std::is_class_v<T>
                    > {};

/**
 * @brief `True` if `T` is any type other than function, reference, or void,
 * with optional const/volatile qualifiers; otherwise, `false`.
 * 
 * @tparam T possible object type
 */
template <typename T>
inline constexpr bool is_object_v = is_object<T>::value;

}; // namespace std

#endif // _STD_TYPE_TRAITS_COMPOSITE_CATEGORIES_HPP