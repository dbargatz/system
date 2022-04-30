/**
 * @file
 * @brief Templates which add or remove const and/or volatile qualifiers for a
 * given type, as defined by C++20 (N4849 draft) 20.15.7.1.
 * 
 * @note Some comments and implementations in this file were based off of or
 * taken directly from the descriptions on
 * https://en.cppreference.com/w/cpp/header/type_traits and various subpages.
 * All errors are mine, all credit is theirs.
 */
#ifndef _STD_TYPE_TRAITS_CV_MODIFIERS_HPP
#define _STD_TYPE_TRAITS_CV_MODIFIERS_HPP

#include <__type_traits/__helpers.hpp>

namespace std {

/**
 * @brief Adds a const qualifier to the given type `T`, unless `T` is a
 * function, a reference, or already has a const qualifier.
 * 
 * @tparam T type to add const qualifier to
 * @param type type with const qualifier added, if not already present
 */
template <typename T>
struct add_const { using type = const T; };

/**
 * @brief The given type `T` with a const qualifier added, unless `T` is a
 * function, a reference, or already has a const qualifier.
 * 
 * @tparam T type to add const qualifier to
 */
template <typename T>
using add_const_t = typename add_const<T>::type;

/**
 * @brief Adds both const and volatile qualifiers to the given type `T`,
 * unless `T` is a function, a reference, or already has const/volatile
 * qualifiers.
 * 
 * @tparam T type to add const and volatile qualifiers to
 * @param type type with const and volatile qualifiers added, if not already
 * present
 */
template <typename T>
struct add_cv { using type = const volatile T; };

/**
 * @brief The given type `T` with both const and volatile qualifiers added,
 * unless `T` is a function, a reference, or already has const/volatile
 * qualifiers.
 * 
 * @tparam T type to add const and volatile qualifiers to
 */
template <typename T>
using add_cv_t = typename add_cv<T>::type;

/**
 * @brief Adds a volatile qualifier to the given type `T`, unless `T` is a
 * function, a reference, or already has a volatile qualifier.
 * 
 * @tparam T type to add volatile qualifier to
 * @param type type with volatile qualifier added, if not already present
 */
template <typename T>
struct add_volatile { using type = volatile T; };

/**
 * @brief The given type `T` with a volatile qualifier added, unless `T` is a
 * function, a reference, or already has a volatile qualifier.
 * 
 * @tparam T type to add volatile qualifier to
 */
template <typename T>
using add_volatile_t = typename add_volatile<T>::type;
 
/**
 * @brief Removes the topmost const qualifier (if any) from the given type.
 * 
 * @tparam T type with possible const qualifier
 * @param type type with topmost const qualifier removed (if any)
 */
template <typename T>
struct remove_const { typedef T type; };

 /**
 * @brief Removes the topmost const qualifier (if any) from the given type.
 * 
 * @tparam T type with possible const qualifier
 * @param type type with topmost const qualifier removed (if any)
 */
template <typename T>
struct remove_const<const T> { typedef T type; };

/**
 * @brief Type with the topmost const qualifier removed (if any).
 * 
 * @tparam T type with possible const qualifier
 */
template <typename T>
using remove_const_t = typename remove_const<T>::type;

/**
 * @brief Removes the topmost const qualifier and/or the topmost volatile
 * qualifier (if any) from the given type.
 * 
 * @tparam T type with possible const/volatile qualifiers
 * @param type type with topmost const/volatile qualifiers removed (if any) 
 */
template <typename T>
struct remove_cv { typedef T type; };

/**
 * @brief Removes the topmost const qualifier and/or the topmost volatile
 * qualifier (if any) from the given type.
 * 
 * @tparam T type with possible const/volatile qualifiers
 * @param type type with topmost const/volatile qualifiers removed (if any) 
 */
template <typename T>
struct remove_cv<const T> { typedef T type; };

/**
 * @brief Removes the topmost const qualifier and/or the topmost volatile
 * qualifier (if any) from the given type.
 * 
 * @tparam T type with possible const/volatile qualifiers
 * @param type type with topmost const/volatile qualifiers removed (if any) 
 */
template <typename T>
struct remove_cv<volatile T> { typedef T type; };

/**
 * @brief Removes the topmost const qualifier and/or the topmost volatile
 * qualifier (if any) from the given type.
 * 
 * @tparam T type with possible const/volatile qualifiers
 * @param type type with topmost const/volatile qualifiers removed (if any) 
 */
template <typename T>
struct remove_cv<const volatile T> { typedef T type; };

/**
 * @brief Type with the topmost const qualifier and/or the topmost volatile
 * qualifier removed (if any).
 * 
 * @tparam T type with possible const/volatile qualifiers
 */
template <typename T>
using remove_cv_t = typename remove_cv<T>::type;

/**
 * @brief Removes the topmost volatile qualifier (if any) from the given type.
 * 
 * @tparam T type with possible volatile qualifier
 * @param type type with topmost volatile qualifier removed (if any)
 */
template <typename T>
struct remove_volatile { typedef T type; };

/**
 * @brief Removes the topmost volatile qualifier (if any) from the given type.
 * 
 * @tparam T type with possible volatile qualifier
 * @param type type with topmost volatile qualifier removed (if any)
 */
template <typename T>
struct remove_volatile<volatile T> { typedef T type; };

/**
 * @brief Type `T` with the topmost volatile qualifier removed (if any).
 * 
 * @tparam T type with possible volatile qualifier
 */
template <typename T>
using remove_volatile_t = typename remove_volatile<T>::type;

}; // namespace std

#endif // _STD_TYPE_TRAITS_CV_MODIFIERS_HPP