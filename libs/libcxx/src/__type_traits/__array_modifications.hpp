/**
 * @file
 * @brief Templates to remove array extents from array types, as defined by
 * C++20 (N4849 draft) 20.15.7.4.
 * 
 * @note Some comments and implementations in this file were based off of or
 * taken directly from the descriptions on
 * https://en.cppreference.com/w/cpp/header/type_traits and various subpages.
 * All errors are mine, all credit is theirs.
 */
#ifndef _STD_TYPE_TRAITS_ARRAY_MODIFICATIONS_HPP
#define _STD_TYPE_TRAITS_ARRAY_MODIFICATIONS_HPP

#include <cstddef>

namespace std {

/**
 * @brief If `T` is an array of some type `X`, member `type` is equal to `X`;
 * otherwise, member `type` is `T`.
 * 
 * @note If `T` is a multidimensional array, only the first dimension is
 * removed.
 * 
 * @tparam T possible array type
 */
template <typename T>
struct remove_extent { using type = T; };

/**
 * @brief If `T` is an array of some type `X`, member `type` is equal to `X`;
 * otherwise, member `type` is `T`.
 * 
 * @note If `T` is a multidimensional array, only the first dimension is
 * removed.
 * 
 * @tparam T possible array type
 */
template <typename T>
struct remove_extent<T[]> { using type = T; };

/**
 * @brief If `T` is an array of some type `X`, member `type` is equal to `X`;
 * otherwise, member `type` is `T`.
 * 
 * @note If `T` is a multidimensional array, only the first dimension is
 * removed.
 * 
 * @tparam T possible array type
 */
template <typename T, std::size_t N>
struct remove_extent<T[N]> { using type = T; };

/**
 * @brief `X` if `T` is an array of some type `X`; otherwise, `T`.
 * 
 * @note If `T` is a multidimensional array, only the first dimension is
 * removed.
 * 
 * @tparam T possible array type
 */
template <typename T>
using remove_extent_t = typename remove_extent<T>::type;

}; // namespace std

#endif // _STD_TYPE_TRAITS_ARRAY_MODIFICATIONS_HPP