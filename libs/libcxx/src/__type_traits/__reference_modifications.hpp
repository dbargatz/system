/**
 * @file
 * @brief Templates which convert a given type to or from various reference
 * types, as defined by C++20 (N4849 draft) 20.15.7.2.
 * 
 * @note Some comments and implementations in this file were based off of or
 * taken directly from the descriptions on
 * https://en.cppreference.com/w/cpp/header/type_traits and various subpages.
 * All errors are mine, all credit is theirs.
 */
#ifndef _STD_TYPE_TRAITS_REFERENCE_MODIFIERS_HPP
#define _STD_TYPE_TRAITS_REFERENCE_MODIFIERS_HPP

#include <__type_traits/__misc_transformations.hpp>

namespace std {

namespace details {

template <typename T>
auto __add_lvalue_reference(int) -> std::type_identity<T&>;

template <typename T>
auto __add_lvalue_reference(...) -> std::type_identity<T>;

template <typename T>
auto __add_rvalue_reference(int) -> std::type_identity<T&&>;

template <typename T>
auto __add_rvalue_reference(...) -> std::type_identity<T>;

}; // namespace details

/**
 * @brief If `T` is an object type or a function type without const, volatile,
 * or reference qualifiers, member `type` has value `T&`. If `T` is an rvalue
 * reference to some type `U`, member `type` has value `U&`. Otherwise, member
 * `type` has value `T`.
 *
 * @tparam T type to add lvalue reference to
 * @param type reference to `T`, or `T` if not allowed
 */
template <typename T>
struct add_lvalue_reference : decltype(details::__add_lvalue_reference<T>(0)) {};

/**
 * @brief `T&` if `T` is an object type or a function type without const,
 * volatile, or reference qualifiers. `U&` if `T` is an rvalue reference to
 * some type `U`. Otherwise, `T`.
 *
 * @tparam T type to add lvalue reference to
 */
template <typename T>
using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;

/**
 * @brief If `T` is an object type or a function type without const, volatile,
 * or reference qualifiers, member `type` has value `T&&`; otherwise, member
 * `type` has value `T`.
 *
 * @tparam T type to add rvalue reference to
 * @param type reference to `T`, or `T` if not allowed
 */
template <typename T>
struct add_rvalue_reference : decltype(details::__add_rvalue_reference<T>(0)) {};

/**
 * @brief `T&&` if `T` is an object type or a function type without const,
 * volatile, or reference qualifiers; otherwise, `T`.
 *
 * @tparam T type to add rvalue reference to
 */
template <typename T>
using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

/**
 * @brief If the type `T` is a reference type, member `type` is equal to the
 * type referred to by T. If the type T is not a reference type, member `type`
 * is equal to `T`.
 *
 * @tparam T possible reference type
 * @param type the type referred to by `T`, or `T`
 */
template <typename T>
struct remove_reference { using type = T; };

/**
 * @brief If the type `T` is a reference type, member `type` is equal to the
 * type referred to by T. If the type T is not a reference type, member `type`
 * is equal to `T`.
 *
 * @tparam T possible reference type
 * @param type the type referred to by `T`, or `T`
 */
template <typename T>
struct remove_reference<T&> { using type = T; };

/**
 * @brief If the type `T` is a reference type, member `type` is equal to the
 * type referred to by T. If the type T is not a reference type, member `type`
 * is equal to `T`.
 *
 * @tparam T possible reference type
 * @param type the type referred to by `T`, or `T`
 */
template <typename T>
struct remove_reference<T&&> { using type = T; };

/**
 * @brief Underlying type that `T` is referring to, or `T` if `T` is not a
 * reference type.
 * 
 * @tparam T possible reference type
 */
template <typename T>
using remove_reference_t = typename remove_reference<T>::type;

}; // namespace std

#endif // _STD_TYPE_TRAITS_REFERENCE_MODIFIERS_HPP