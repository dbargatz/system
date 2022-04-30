/**
 * @file
 * @brief Implementation of the `std::decay`, as defined by C++20 (N4849 draft)
 * 20.15.7.6.
 * 
 * @details
 * This is pulled into its own header because `std::decay` relies on a large
 * number of other type traits, and would cause a large number of circular
 * includes if defined in its conceptual home in
 * <__type_traits/__misc_transformations.hpp>.
 * 
 * @note Some comments and implementations in this file were based off of or
 * taken directly from the descriptions on
 * https://en.cppreference.com/w/cpp/header/type_traits and various subpages.
 * All errors are mine, all credit is theirs.
 */
#ifndef _STD_TYPE_TRAITS_DECAY_HPP
#define _STD_TYPE_TRAITS_DECAY_HPP

#include <__type_traits/__array_modifications.hpp>
#include <__type_traits/__misc_transformations.hpp>
#include <__type_traits/__primary_categories.hpp>
#include <__type_traits/__reference_modifications.hpp>

namespace std {

/**
 * @brief Applies lvalue-to-rvalue, array-to-pointer, and function-to-pointer
 * implicit conversions to type `T`, removes const/volatile qualifiers, and
 * sets the member `type` to the resulting type.
 * 
 * @tparam T type to decay
 */
template <typename T>
struct decay {
private:
    using U = typename std::remove_reference<T>::type;

public:
    using type = typename std::conditional_t<std::is_array_v<U>,
        typename std::remove_extent_t<U>*,
        typename std::conditional_t<std::is_function_v<U>,
            typename std::add_pointer_t<U>,
            typename std::remove_cv_t<U>
        >
    >;
};

/**
 * @brief The resulting type after applying lvalue-to-rvalue, array-to-pointer,
 * and function-to-pointer implicit conversions, and removing const/volatile
 * qualifiers, from the given type `T`.
 * 
 * @tparam T type to decay
 */
template <typename T>
using decay_t = typename decay<T>::type;

}; // namespace std

#endif // _STD_TYPE_TRAITS_DECAY_HPP