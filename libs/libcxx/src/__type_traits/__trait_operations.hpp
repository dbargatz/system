/**
 * @file
 * @brief Implementations of the logical operator trait templates, as defined
 * by C++20 (N4849 draft) 20.15.8.
 * 
 * @note Some comments and implementations in this file were based off of or
 * taken directly from the descriptions on
 * https://en.cppreference.com/w/cpp/header/type_traits and various subpages.
 * All errors are mine, all credit is theirs.
 */
#ifndef _STD_TYPE_TRAITS_TRAIT_OPERATIONS_HPP
#define _STD_TYPE_TRAITS_TRAIT_OPERATIONS_HPP

#include <__type_traits/__helpers.hpp>
#include <__type_traits/__misc_transformations.hpp>

namespace std {

/**
 * @brief Performs the equivalent of a logical AND on the given sequence of
 * type traits, with the result stored in member `value`.
 * 
 * @tparam B... sequence of type traits for which each B must be usable as a
 * base class and defines a member `value` convertible to `bool`
 */
template <typename...>
struct conjunction : std::true_type {};

/**
 * @brief Performs the equivalent of a logical AND on the given sequence of
 * type traits, with the result stored in member `value`.
 * 
 * @tparam B... sequence of type traits for which each B must be usable as a
 * base class and defines a member `value` convertible to `bool`
 */
template <typename B1>
struct conjunction<B1> : B1 {};

/**
 * @brief Performs the equivalent of a logical AND on the given sequence of
 * type traits, with the result stored in member `value`.
 * 
 * @tparam B... sequence of type traits for which each B must be usable as a
 * base class and defines a member `value` convertible to `bool`
 */
template <typename B1, typename... Bn>
struct conjunction<B1, Bn...> : std::conditional_t<bool(B1::value), conjunction<Bn...>, B1> {};

/**
 * @brief `True` if the equivalent of a logical AND on the given sequence of
 * type traits is `true`; otherwise, `false`.
 * 
 * @tparam B... sequence of type traits for which each B must be usable as a
 * base class and defines a member `value` convertible to `bool`
 */
template <typename... B>
inline constexpr bool conjunction_v = conjunction<B...>::value;

/**
 * @brief Performs the equivalent of a logical OR on the given sequence of
 * type traits, with the result stored in member `value`. Will short-circuit,
 * meaning every Bi::value may not be instantiated.
 * 
 * @tparam B... sequence of type traits for which each B must be usable as a
 * base class and defines a member `value` convertible to `bool`
 */
template <typename...>
struct disjunction : std::false_type {};

/**
 * @brief Performs the equivalent of a logical OR on the given sequence of
 * type traits, with the result stored in member `value`. Will short-circuit,
 * meaning every Bi::value may not be instantiated.
 * 
 * @tparam B... sequence of type traits for which each B must be usable as a
 * base class and defines a member `value` convertible to `bool`
 */
template <typename B1>
struct disjunction<B1> : B1 {};

/**
 * @brief Performs the equivalent of a logical OR on the given sequence of
 * type traits, with the result stored in member `value`. Will short-circuit,
 * meaning every Bi::value may not be instantiated.
 * 
 * @tparam B... sequence of type traits for which each B must be usable as a
 * base class and defines a member `value` convertible to `bool`
 */
template <typename B1, typename... Bn>
struct disjunction<B1, Bn...> : std::conditional_t<bool(B1::value), B1, disjunction<Bn...>> {};

/**
 * @brief `True` if the equivalent of a logical OR on the given sequence of
 * type traits is `true`; otherwise, `false`.
 * 
 * @tparam B... sequence of type traits for which each B must be usable as a
 * base class and defines a member `value` convertible to `bool`
 */
template <typename... B>
inline constexpr bool disjunction_v = disjunction<B...>::value;

/**
 * @brief Performs the equivalent of a logical NOT on the given type trait,
 * storing the result in member `value`.
 * 
 * @tparam B type trait which defines a member `value` convertible to `bool`
 */
template <typename B>
struct negation : std::bool_constant<!bool(B::value)> {};

/**
 * @brief `True` if the equivalent of a logical NOT on the given type trait is
 * `true`; otherwise, `false`.
 * 
 * @tparam B type trait which defines a member `value` convertible to `bool`
 */
template <typename B>
inline constexpr bool negation_v = negation<B>::value;

}; // namespace std

#endif // _STD_TYPE_TRAITS_TRAIT_OPERATIONS_HPP