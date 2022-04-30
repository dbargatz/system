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

#include <__type_traits/__is_reference.hpp>

#endif // _STD_TYPE_TRAITS_COMPOSITE_CATEGORIES_HPP