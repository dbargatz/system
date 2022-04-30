/**
 * @file
 * @brief Templates to convert a given type to or from a pointer to the given
 * type, as defined by C++20 (N4849 draft) 20.15.7.5.
 * 
 * @note Some comments and implementations in this file were based off of or
 * taken directly from the descriptions on
 * https://en.cppreference.com/w/cpp/header/type_traits and various subpages.
 * All errors are mine, all credit is theirs.
 */
#ifndef _STD_TYPE_TRAITS_POINTER_MODIFICATIONS_HPP
#define _STD_TYPE_TRAITS_POINTER_MODIFICATIONS_HPP

#include <__type_traits/__misc_transformations.hpp>
#include <__type_traits/__reference_modifications.hpp>

namespace std {

namespace details {

template <typename T>
auto __add_pointer(int) -> std::type_identity<typename std::remove_reference<T>::type * >;

template <typename T>
auto __add_pointer(...) -> std::type_identity<T>;

}; // namespace details

/**
 * @brief If `T` is a reference type to `X`, member `type` has value `X*` (a
 * pointer to `X`). If `T` is an object type or a function type without const,
 * volatile, or reference qualifiers, member `type` has value `T*` (a pointer
 * to `T`). Otherwise, `T` must be a function type with const, volatile, and/or
 * reference qualifiers, so member `type` has value `T`.
 *
 * @tparam T type to add pointer to
 * @param type pointer to `X` where `T` is a reference type to `X`, or a
 * pointer to `T`, or `T`
 */
template <typename T>
struct add_pointer : decltype(details::__add_pointer<T>(0)) {};

}; // namespace std

#endif // _STD_TYPE_TRAITS_POINTER_MODIFICATIONS_HPP