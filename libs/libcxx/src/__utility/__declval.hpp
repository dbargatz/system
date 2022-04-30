/**
 * @file
 * @brief The `std::declval()` implementation and internal helpers, as defined
 * by C++20 (N4849 draft) sections 20.2.1 and 20.2.6.
 * 
 * @details
 * This is pulled into its own header because:
 * - Some templates in <type_traits> are best implemented with `std::declval`
 * - Some templates in <utility> including `std::declval` are best implemented
 *   with various reference-related templates in <type_traits>
 * - `std::declval` must be declared in <utility>, per C++20 (N4849 draft)
 *   sections 20.2.1 and 20.2.6.
 * 
 * To avoid the resulting circular references, `std::declval` is defined in
 * this header, and included by various <type_traits> implementation headers
 * and <utility>.
 * 
 * @note Some comments and implementations in this file were based off of or
 * taken directly from the descriptions on
 * https://en.cppreference.com/w/cpp/header/utility and various subpages.
 * All errors are mine, all credit is theirs.
 */
#ifndef _STD_UTILITY_DECLVAL_HPP
#define _STD_UTILITY_DECLVAL_HPP

#include <__type_traits/__reference_modifiers.hpp>

namespace std {

namespace details {

/**
 * @brief Internal helper template. Always `false` regardless of `T`'s type.
 * 
 * @tparam T any type
 */
template <typename T>
constexpr bool __always_false = false;

}; // namespace details

/**
 * @brief Converts any type `T` to a reference type, which allows member
 * functions to be used in `decltype()` expressions easily.
 * 
 * @details
 * Commonly used in templates where acceptable template parameters may not
 * share a common constructor, but have the same member function whose return
 * type is needed. This type cannot be evaluated and can only be used in
 * unevaluated contexts, as it only queries the properties of the provided type
 * `T`.
 * 
 * @tparam T type to convert to a reference type
 * @return `T&&` unless `T` is void (or void with optional const/volatile
 * qualifiers), in which case `T`.
 */
template <typename T>
typename std::add_rvalue_reference<T>::type declval() noexcept {
    static_assert(details::__always_false<T>, "declval not allowed in an evaluated context");
}

}; // namespace std

#endif // _STD_UTILITY_DECLVAL_HPP