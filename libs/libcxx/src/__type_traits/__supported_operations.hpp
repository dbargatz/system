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

/**
 * @brief If `std::is_constructible<T>::value` is `true`, meaning `T` is
 * constructible via a no-args (default) constructor, then the member `value`
 * is equal to `true`; otherwise, member `value` is equal to `false`.
 * 
 * @tparam T possible default-constructible type
 */
template <typename T>
struct is_default_constructible : is_constructible<T> {};

/**
 * @brief `True` if `std::is_constructible<T>::value` is `true`, meaning `T` is
 * constructible via a no-args (default) constructor; otherwise, `false`.
 * 
 * @tparam T possible default-constructible type
 */
template <typename T>
inline constexpr bool is_default_constructible_v = is_default_constructible<T>::value;

/**
 * @brief If `T` is an object or reference type, and the variable definition
 * `T obj(std::declval<Args>()...)` is well-formed and noexcept, then the
 * member `value` is equal to `true`; otherwise, member `value` is equal to
 * `false`.
 * 
 * @note The `__is_nothrow_constructible()` function used in the definition of
 * this template is a compiler intrinsic. Compiler support isn't necessary for
 * a correct and conforming implementation of `std::is_nothrow_constructible`,
 * but it does make the implementation significantly simpler, as seen in this
 * block from clang's libcxx:
 * https://github.com/llvm/llvm-project/blob/a9d68a5524dea113cace5983697786599cbdce9a/libcxx/include/type_traits#L3067-L3112
 * 
 * @tparam T possible nothrow-constructible type
 * @tparam Args types of arguments a possible nothrow constructor of `T` would
 * accept
 */
template <typename T, typename... Args>
struct is_nothrow_constructible : std::bool_constant<__is_nothrow_constructible(T, Args...)> {};

/**
 * @brief `True` if `T` is an object or reference type and the variable
 * definition `T obj(std::declval<Args>()...)` is well-formed and noexcept;
 * otherwise, `false`.
 * 
 * @tparam T possible nothrow-constructible type
 * @tparam Args types of arguments a possible nothrow constructor of `T` would
 * accept
 */
template <typename T, typename... Args>
inline constexpr bool is_nothrow_constructible_v = is_nothrow_constructible<T, Args...>::value;

/**
 * @brief If `std::is_nothrow_constructible<T>::value` is `true`, meaning `T`
 * is constructible via a no-args (default) constructor marked as `noexcept`,
 * then the member `value` is equal to `true`; otherwise, member `value` is
 * equal to `false`.
 * 
 * @tparam T possible nothrow-default-constructible type
 */
template <typename T>
struct is_nothrow_default_constructible : is_nothrow_constructible<T> {};

/**
 * @brief `True` if `std::is_nothrow_constructible<T>::value` is `true`,
 * meaning `T` is constructible via a no-args (default) constructor marked as
 * `noexcept`; otherwise, `false`.
 * 
 * @tparam T possible nothrow-default-constructible type
 */
template <typename T>
inline constexpr bool is_nothrow_default_constructible_v = is_nothrow_default_constructible<T>::value;

/**
 * @brief If `T` is an object or reference type, and the variable definition
 * `T obj(std::declval<Args>()...)` is well-formed and only calls trivial
 * operations, then the member `value` is equal to `true`; otherwise, member
 * `value` is equal to `false`.
 * 
 * @note The `__is_trivially_constructible()` function used in the definition
 * of this template is a compiler intrinsic. Compiler support is required for a
 * correct and conforming implementation of `std::is_trivially_constructible`.
 * For details, see https://stackoverflow.com/a/65761122, which applies to
 * `std::is_trivially_constructible` as well.
 * 
 * @tparam T possible trivially-constructible type
 * @tparam Args types of arguments a possible trivial constructor of `T` would
 * accept
 */
template <typename T, typename... Args>
struct is_trivially_constructible : std::bool_constant<__is_trivially_constructible(T, Args...)> {};

/**
 * @brief `True` if `T` is an object or reference type and the variable
 * definition `T obj(std::declval<Args>()...)` is well-formed and only calls
 * trivial operations; otherwise, `false`.
 * 
 * @tparam T possible trivially-constructible type
 * @tparam Args types of arguments a possible trivial constructor of `T` would
 * accept
 */
template <typename T, typename... Args>
inline constexpr bool is_trivially_constructible_v = is_trivially_constructible<T, Args...>::value;

/**
 * @brief If `std::is_trivially_constructible<T>::value` is `true`, meaning `T`
 * is constructible via a no-args (default) constructor that only calls trivial
 * operations, then the member `value` is equal to `true`; otherwise, member
 * `value` is equal to `false`.
 * 
 * @tparam T possible trivially-default-constructible type
 */
template <typename T>
struct is_trivially_default_constructible : is_trivially_constructible<T> {};

/**
 * @brief `True` if `std::is_trivially_constructible<T>::value` is `true`,
 * meaning `T` is constructible via a no-args (default) constructor that only
 * calls trivial operations; otherwise, `false`.
 * 
 * @tparam T possible trivially-default-constructible type
 */
template <typename T>
inline constexpr bool is_trivially_default_constructible_v = is_trivially_default_constructible<T>::value;

}; // namespace std

#endif // _STD_TYPE_TRAITS_SUPPORTED_OPERATIONS_HPP