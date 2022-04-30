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

#include <cstddef>
#include <__type_traits/__helpers.hpp>
#include <__type_traits/__is_reference.hpp>
#include <__type_traits/__properties.hpp>
#include <__type_traits/__reference_modifications.hpp>

namespace std {

namespace details {

template <typename T>
struct __is_floating_point : public false_type {};

template <>
struct __is_floating_point<float> : public true_type {};

template <>
struct __is_floating_point<double> : public true_type {};

template <>
struct __is_floating_point<long double> : public true_type {};

template <typename T>
struct __is_integral : public false_type {};

template <>
struct __is_integral<bool> : public true_type {};

template <>
struct __is_integral<char> : public true_type {};

template <>
struct __is_integral<signed char> : public true_type {};

template <>
struct __is_integral<unsigned char> : public true_type {};

template <>
struct __is_integral<char8_t> : public true_type {};

template <>
struct __is_integral<char16_t> : public true_type {};

template <>
struct __is_integral<char32_t> : public true_type {};

template <>
struct __is_integral<wchar_t> : public true_type {};

template <>
struct __is_integral<short> : public true_type {};

template <>
struct __is_integral<unsigned short> : public true_type {};

template <>
struct __is_integral<int> : public true_type {};

template <>
struct __is_integral<unsigned int> : public true_type {};

template <>
struct __is_integral<long> : public true_type {};

template <>
struct __is_integral<unsigned long> : public true_type {};

template <>
struct __is_integral<long long> : public true_type {};

template <>
struct __is_integral<unsigned long long> : public true_type {};

template <typename T>
struct __is_null_pointer : public false_type {};

template <>
struct __is_null_pointer<nullptr_t> : public true_type {};

template <typename T>
struct __is_void : public false_type {};

template <>
struct __is_void<void> : public true_type {};

}; // namespace details

/**
 * @brief If `T` is an array type, member `value` is `true`; otherwise, member
 * `value` is `false`.
 * 
 * @tparam T possible array type
 */
template <typename T>
struct is_array : false_type {};

/**
 * @brief If `T` is an array type, member `value` is `true`; otherwise, member
 * `value` is `false`.
 * 
 * @tparam T possible array type
 */
template <typename T>
struct is_array<T[]> : true_type {};

/**
 * @brief If `T` is an array type, member `value` is `true`; otherwise, member
 * `value` is `false`.
 * 
 * @tparam T possible array type
 */
template <typename T, std::size_t N>
struct is_array<T[N]> : true_type {};

/**
 * @brief `True` if `T` is an array type; otherwise, `false`.
 * 
 * @tparam T possible array type
 */
template <typename T>
inline constexpr bool is_array_v = is_array<T>::value;

/**
 * @brief If `T` is an enum type, member `value` is `true`; otherwise, member
 * `value` is `false`.
 * 
 * @note The `__is_enum()` function used in the definition of this template is
 * a compiler intrinsic. Compiler support isn't necessary for a correct and
 * conforming implementation of `std::is_enum`, but it does make the
 * implementation significantly simpler, as seen in this block from clang's
 * libcxx:
 * https://github.com/llvm/llvm-project/blob/a9d68a5524dea113cace5983697786599cbdce9a/libcxx/include/type_traits#L998-L1029
 * 
 * @tparam T possible enum type
 */
template <typename T>
struct is_enum : std::bool_constant<__is_enum(T)> {};

/**
 * @brief `True` if `T` is an enum type; otherwise, `false`.
 * 
 * @tparam T possible enum type
 */
template <typename T>
inline constexpr bool is_enum_v = is_enum<T>::value;

/**
 * @brief If `T` is a floating-point type with optional const/volatile
 * qualifiers, then `value` is `true`. Otherwise, `value` is `false`.
 *
 * @details
 * The types considered floating-point that result in `value` being `true` are:
 *
 * - `float`
 * - `double`
 * - `long double`
 *
 * Any of these types may also have a const and/or volatile qualifier. All
 * other types result in `value` being `false`.
 * 
 * @tparam T possible floating-point type with optional const/volatile
 * qualifiers
 * @param value `true` if `T` is a floating-point type with optional const/
 * volatile qualifiers; `false` otherwise
 */
template <typename T>
struct is_floating_point : public details::__is_floating_point<std::remove_cv_t<T>>::type {};

/**
 * @brief `True` if `T` is a floating-point type with optional const/volatile
 * qualifiers; otherwise, `false`.
 * 
 * @tparam T possible floating-point type with optional const/volatile
 * qualifiers
 */
template <typename T>
inline constexpr bool is_floating_point_v = is_floating_point<T>::value;

/**
 * @brief If `T` is a function type, member `value` is `true`; otherwise,
 * member `value` is `false`.
 * 
 * @note Lambdas, classes with `operator()` overloaded, pointers to functions,
 * and `std::function` do not count as functions for the purposes of this type
 * check.
 * 
 * @tparam T possible function type (see note)
 */
template <typename T>
struct is_function : std::integral_constant<bool, !std::is_const<const T>::value && !std::is_reference<T>::value> {};

/**
 * @brief `True` if `T` is a function type; otherwise, `false`.
 * 
 * @note Lambdas, classes with `operator()` overloaded, pointers to functions,
 * and `std::function` do not count as functions for the purposes of this type
 * check.
 * 
 * @tparam T possible function type (see note)
 */
template <typename T>
inline constexpr bool is_function_v = is_function<T>::value;

/**
 * @brief If `T` is an integral type with optional const/volatile qualifiers,
 * then `value` is `true`. Otherwise, `value` is `false`.
 *
 * @details
 * The types considered integral that result in `value` being `true` are:
 *
 * - `bool`
 * - `char` (signed, unsigned, or unspecified)
 * - `char8_t`
 * - `char16_t`
 * - `char32_t`
 * - `wchar_t`
 * - `short` (signed, unsigned, or unspecified)
 * - `int` (signed, unsigned, or unspecified)
 * - `long` (signed, unsigned, or unspecified)
 * - `long long` (signed, unsigned, or unspecified)
 *
 * Any of these types may also have a const and/or volatile qualifier. All
 * other types result in `value` being `false`.
 * 
 * @tparam T possible integral type with optional const/volatile qualifiers
 * @param value `true` if `T` is an integral type with optional const/volatile
 * qualifiers; `false` otherwise
 */
template <typename T>
struct is_integral : public details::__is_integral<std::remove_cv_t<T>>::type {};

/**
 * @brief `True` if `T` is an integral type with optional const/volatile
 * qualifiers; otherwise, `false`.
 * 
 * @tparam T possible integral type with optional const/volatile qualifiers
 */
template <typename T>
inline constexpr bool is_integral_v = is_integral<T>::value;

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
 * @brief If `T` is the `std::nullptr_t` type with optional const/volatile
 * qualifiers, then `value` is `true`. Otherwise, `value` is `false`.
 *
 * @details
 * The types considered `std::nullptr_t` that result in `value` being `true`
 * are:
 *
 * - `std::nullptr_t`
 * - `const std::nullptr_t`
 * - `volatile std::nullptr_t`
 * - `const volatile std::nullptr_t`
 *
 * All other types result in `value` being `false`.
 * 
 * @tparam T possible `std::nullptr_t` type with optional const/volatile
 * qualifiers
 * @param value `true` if `T` is the `std::nullptr_t` type with optional const/
 * volatile qualifiers; `false` otherwise
 */
template <typename T>
struct is_null_pointer : public details::__is_null_pointer<std::remove_cv_t<T>>::type {};

/**
 * @brief `True` if `T` is the `std::nullptr_t` type with optional const/
 * volatile qualifiers; otherwise, `false`.
 * 
 * @tparam T possible `std::nullptr_t` type with optional const/volatile
 * qualifiers
 */
template <typename T>
inline constexpr bool is_null_pointer_v = is_null_pointer<T>::value;

/**
 * @brief If `T` is a pointer type, member `value` is `true`; otherwise, member
 * `value` is `false`.
 * 
 * @note The `__is_pointer()` function used in the definition of this template
 * is a compiler intrinsic. Compiler support isn't necessary for a correct and
 * conforming implementation of `std::is_pointer`, but it does make the
 * implementation significantly simpler, as seen in this block from clang's
 * libcxx:
 * https://github.com/llvm/llvm-project/blob/a9d68a5524dea113cace5983697786599cbdce9a/libcxx/include/type_traits#L763-L798
 * 
 * @tparam T possible pointer type
 */
template <typename T>
struct is_pointer : std::bool_constant<__is_pointer(T)> {};

/**
 * @brief `True` if `T` is a pointer type; otherwise, `false`.
 * 
 * @tparam T possible pointer type
 */
template <typename T>
inline constexpr bool is_pointer_v = is_pointer<T>::value;

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