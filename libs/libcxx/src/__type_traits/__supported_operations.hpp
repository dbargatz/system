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

namespace details {

template <typename T>
using __add_const_lvalue_reference = typename std::add_lvalue_reference<typename std::add_const<T>::type>::type;

}; // namespace details

/**
 * @brief If `T` is a type with a virtual destructor, then the member `value`
 * is equal to `true`; otherwise, member `value` is equal to `false`.
 * 
 * @note The `__has_virtual_destructor()` function used in the definition of
 * this template is a compiler intrinsic. Compiler support is required for a
 * correct and conforming implementation of `std::has_virtual_destructor`.
 * 
 * @tparam T type with possible virtual destructor
 */
template <typename T>
struct has_virtual_destructor : std::bool_constant<__has_virtual_destructor(T)> {};

/**
 * @brief `True` if `T` is a type with a virtual destructor; otherwise,
 * `false`.
 * 
 * @tparam T type with possible virtual destructor
 */
template <typename T>
inline constexpr bool has_virtual_destructor_v = has_virtual_destructor<T>::value;

/**
 * @brief If the expression `std::declval<T>() == std::declval<U>()` is well-
 * formed in an unevaluated context, then the member `value` is equal to
 * `true`; otherwise, member `value` is equal to `false`.
 * 
 * @note The `__is_assignable()` function used in the definition of this
 * template is a compiler intrinsic. Compiler support isn't necessary for a
 * correct and conforming implementation of `std::is_assignable`, but it does
 * make the implementation significantly simpler, as seen in this block from
 * clang's libcxx:
 * https://github.com/llvm/llvm-project/blob/a9d68a5524dea113cace5983697786599cbdce9a/libcxx/include/type_traits#L2510-L2553
 * 
 * @tparam T possible assignable type
 * @tparam U possible assignable type
 */
template <typename T, typename U>
struct is_assignable : std::bool_constant<__is_assignable(T, U)> {};

/**
 * @brief `True` if the expression `std::declval<T>() == std::declval<U>()` is
 * well-formed in an unevaluated context; otherwise, `false`.
 * 
 * @tparam T possible assignable type
 * @tparam U possible assignable type
 */
template <typename T, typename U>
inline constexpr bool is_assignable_v = is_assignable<T, U>::value;

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
 * @brief If `T` is a referenceable type and is assignable from a `T&` lvalue
 * reference or `const T&` lvalue reference (aka a copy assignment), then the
 * member `value` is equal to `true`; otherwise, member `value` is equal to
 * `false`.
 * 
 * @tparam T possible copy-assignable type
 */
template <typename T>
struct is_copy_assignable : is_assignable<typename std::add_lvalue_reference<T>::type, typename std::add_lvalue_reference<const T>::type> {};

/**
 * @brief `True` if `T` is a referenceable type and is assignable from a `T&`
 * lvalue reference or `const T&` lvalue reference (aka a copy assignment);
 * otherwise, `false`.
 * 
 * @tparam T possible copy-assignable type
 */
template <typename T>
inline constexpr bool is_copy_assignable_v = is_copy_assignable<T>::value;

/**
 * @brief If `T` is a referenceable type and is constructible via a constructor
 * accepting a single `const T&` argument (aka a copy constructor), then the
 * member `value` is equal to `true`; otherwise, member `value` is equal to
 * `false`.
 * 
 * @tparam T possible copy-constructible type
 */
template <typename T>
struct is_copy_constructible : is_constructible<T, details::__add_const_lvalue_reference<T>> {};

/**
 * @brief `True` if `T` is a referenceable type and is constructible via a
 * constructor accepting a single `const T&` argument (aka a copy constructor);
 * otherwise, `false`.
 * 
 * @tparam T possible copy-constructible type
 */
template <typename T>
inline constexpr bool is_copy_constructible_v = is_copy_constructible<T>::value;

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
 * @brief If `T` is a reference type, or `T` is an object type such that the
 * expression `std::declval<U&>().~U()` (where `U` is
 * `std::remove_all_extents<T>::type`) is well-formed in an unevaluated
 * context, then the member `value` is equal to `true`; otherwise, member
 * `value` is equal to `false`.
 * 
 * @note The `__is_destructible()` function used in the definition of this
 * template is a compiler intrinsic, and can only be enabled via the
 * `-fms-extensions` flag provided to clang. Compiler support isn't necessary
 * for a correct and conforming implementation of `std::is_destructible`, but
 * it does make the implementation significantly simpler, as seen in this block
 * from clang's libcxx:
 * https://github.com/llvm/llvm-project/blob/a9d68a5524dea113cace5983697786599cbdce9a/libcxx/include/type_traits#L2577-L2652
 * @note For more information on the required `-fms-extensions` flag and the
 * `__is_destructible` intrinsic, see the LLVM docs at:
 * https://clang.llvm.org/docs/LanguageExtensions.html#type-trait-primitives
 * 
 * @tparam T possible destructible type
 */
template <typename T>
struct is_destructible : std::bool_constant<__is_destructible(T)> {};

/**
 * @brief `True` if `T` is a reference type, or `T` is an object type such that
 * the expression `std::declval<U&>().~U()` (where `U` is
 * `std::remove_all_extents<T>::type`) is well-formed in an unevaluated
 * context; otherwise, `false`.
 * 
 * @tparam T possible destructible type
 */
template <typename T>
inline constexpr bool is_destructible_v = is_destructible<T>::value;

/**
 * @brief If `T` is a referenceable type and is assignable from a `T&` lvalue
 * reference or `T&&` rvalue reference (aka a move assignment), then the member
 * `value` is equal to `true`; otherwise, member `value` is equal to `false`.
 * 
 * @tparam T possible move-assignable type
 */
template <typename T>
struct is_move_assignable : is_assignable<typename std::add_lvalue_reference<T>::type, typename std::add_rvalue_reference<T>::type> {};

/**
 * @brief `True` if `T` is a referenceable type and is assignable from a `T&`
 * lvalue reference or `T&&` rvalue reference (aka a move assignment);
 * otherwise, `false`.
 * 
 * @tparam T possible move-assignable type
 */
template <typename T>
inline constexpr bool is_move_assignable_v = is_move_assignable<T>::value;

/**
 * @brief If `T` is a referenceable type and is constructible via a constructor
 * accepting a single `T&&` argument (aka a move constructor), then the member
 * `value` is equal to `true`; otherwise, member `value` is equal to `false`.
 * 
 * @tparam T possible move-constructible type
 */
template <typename T>
struct is_move_constructible : is_constructible<T, typename std::add_rvalue_reference<T>::type> {};

/**
 * @brief `True` if `T` is a referenceable type and is constructible via a
 * constructor accepting a single `T&&` argument (aka a move constructor);
 * otherwise, `false`.
 * 
 * @tparam T possible move-constructible type
 */
template <typename T>
inline constexpr bool is_move_constructible_v = is_move_constructible<T>::value;

/**
 * @brief If the expression `std::declval<T>() == std::declval<U>()` is well-
 * formed and nothrow in an unevaluated context, then the member `value` is
 * equal to `true`; otherwise, member `value` is equal to `false`.
 * 
 * @note The `__is_nothrow_assignable()` function used in the definition of
 * this template is a compiler intrinsic. Compiler support isn't necessary for
 * a correct and conforming implementation of `std::is_nothrow_assignable`, but
 * it does make the implementation significantly simpler, as seen in this block
 * from clang's libcxx:
 * https://github.com/llvm/llvm-project/blob/a9d68a5524dea113cace5983697786599cbdce9a/libcxx/include/type_traits#L3153-L3183
 * 
 * @tparam T possible nothrow-assignable type
 * @tparam U possible nothrow-assignable type
 */
template <typename T, typename U>
struct is_nothrow_assignable : std::bool_constant<__is_nothrow_assignable(T, U)> {};

/**
 * @brief `True` if the expression `std::declval<T>() == std::declval<U>()` is
 * well-formed and noexcept in an unevaluated context; otherwise, `false`.
 * 
 * @tparam T possible noexcept-assignable type
 * @tparam U possible noexcept-assignable type
 */
template <typename T, typename U>
inline constexpr bool is_nothrow_assignable_v = is_nothrow_assignable<T, U>::value;

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
 * @brief If `T` is a referenceable type and is nothrow-assignable from a `T&`
 * lvalue reference or `const T&` lvalue reference (aka a nothrow copy
 * assignment), then the member `value` is equal to `true`; otherwise, member
 * `value` is equal to `false`.
 * 
 * @tparam T possible nothrow-copy-assignable type
 */
template <typename T>
struct is_nothrow_copy_assignable : is_nothrow_assignable<typename std::add_lvalue_reference<T>::type, typename std::add_lvalue_reference<const T>::type> {};

/**
 * @brief `True` if `T` is a referenceable type and is nothrow-assignable from
 * a `T&` lvalue reference or `const T&` lvalue reference (aka a nothrow copy
 * assignment); otherwise, `false`.
 * 
 * @tparam T possible nothrow-copy-assignable type
 */
template <typename T>
inline constexpr bool is_nothrow_copy_assignable_v = is_nothrow_copy_assignable<T>::value;

/**
 * @brief If `T` is a referenceable type and is constructible via a nothrow-
 * marked constructor accepting a single `const T&` argument (aka a copy
 * constructor), then the member `value` is equal to `true`; otherwise, member
 * `value` is equal to `false`.
 * 
 * @tparam T possible nothrow-copy-constructible type
 */
template <typename T>
struct is_nothrow_copy_constructible : is_nothrow_constructible<T, details::__add_const_lvalue_reference<T>> {};

/**
 * @brief `True` if `T` is a referenceable type and is constructible via a
 * nothrow-marked constructor accepting a single `const T&` argument (aka a
 * copy constructor); otherwise, `false`.
 * 
 * @tparam T possible nothrow-copy-constructible type
 */
template <typename T>
inline constexpr bool is_nothrow_copy_constructible_v = is_nothrow_copy_constructible<T>::value;

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
 * @brief If `T` is a reference type, or `T` is an object type such that the
 * expression `std::declval<U&>().~U() nothrow` (where `U` is
 * `std::remove_all_extents<T>::type`) is well-formed in an unevaluated
 * context, then the member `value` is equal to `true`; otherwise, member
 * `value` is equal to `false`.
 * 
 * @note The `__is_nothrow_destructible()` function used in the definition of
 * this template is a compiler intrinsic, and can only be enabled via the
 * `-fms-extensions` flag provided to clang. Compiler support isn't necessary
 * for a correct and conforming implementation of
 * `std::is_nothrow_destructible`, but it does make the implementation
 * significantly simpler, as seen in this block from clang's libcxx:
 * https://github.com/llvm/llvm-project/blob/a9d68a5524dea113cace5983697786599cbdce9a/libcxx/include/type_traits#L3213-L3268
 * @note For more information on the required `-fms-extensions` flag and the
 * `__is_nothrow_destructible` intrinsic, see the LLVM docs at:
 * https://clang.llvm.org/docs/LanguageExtensions.html#type-trait-primitives
 * 
 * @tparam T possible nothrow-destructible type
 */
template <typename T>
struct is_nothrow_destructible : std::bool_constant<__is_nothrow_destructible(T)> {};

/**
 * @brief `True` if `T` is a reference type, or `T` is an object type such that
 * the expression `std::declval<U&>().~U() nothrow` (where `U` is
 * `std::remove_all_extents<T>::type`) is well-formed in an unevaluated
 * context; otherwise, `false`.
 * 
 * @tparam T possible nothrow-destructible type
 */
template <typename T>
inline constexpr bool is_nothrow_destructible_v = is_nothrow_destructible<T>::value;

/**
 * @brief If `T` is a referenceable type and is nothrow-assignable from a `T&`
 * lvalue reference or `T&&` rvalue reference (aka a nothrow move assignment),
 * then the member `value` is equal to `true`; otherwise, member `value` is
 * equal to `false`.
 * 
 * @tparam T possible nothrow-move-assignable type
 */
template <typename T>
struct is_nothrow_move_assignable : is_nothrow_assignable<typename std::add_lvalue_reference<T>::type, typename std::add_rvalue_reference<T>::type> {};

/**
 * @brief `True` if `T` is a referenceable type and is nothrow-assignable from
 * a `T&` lvalue reference or `T&&` rvalue reference (aka a nothrow move
 * assignment); otherwise, `false`.
 * 
 * @tparam T possible nothrow-move-assignable type
 */
template <typename T>
inline constexpr bool is_nothrow_move_assignable_v = is_nothrow_move_assignable<T>::value;

/**
 * @brief If `T` is a referenceable type and is constructible via a nothrow-
 * marked constructor accepting a single `T&&` argument (aka a move
 * constructor), then the member `value` is equal to `true`; otherwise, member
 * `value` is equal to `false`.
 * 
 * @tparam T possible nothrow-move-constructible type
 */
template <typename T>
struct is_nothrow_move_constructible : is_nothrow_constructible<T, typename std::add_rvalue_reference<T>::type> {};

/**
 * @brief `True` if `T` is a referenceable type and is constructible via a
 * nothrow-marked constructor accepting a single `T&&` argument (aka a move
 * constructor); otherwise, `false`.
 * 
 * @tparam T possible nothrow-move-constructible type
 */
template <typename T>
inline constexpr bool is_nothrow_move_constructible_v = is_nothrow_move_constructible<T>::value;

/**
 * @brief If the expression `std::declval<T>() == std::declval<U>()` is well-
 * formed in an unevaluated context and the evaluation of the expression only
 * calls `std::declval` and trivial operations, then the member `value` is
 * equal to `true`; otherwise, member `value` is equal to `false`.
 * 
 * @note The `__is_trivially_assignable()` function used in the definition
 * of this template is a compiler intrinsic. Compiler support is required for a
 * correct and conforming implementation of `std::is_trivially_assignable`.
 * For details, see https://stackoverflow.com/a/65761122, which applies to
 * `std::is_trivially_assignable` as well.
 * 
 * @tparam T possible trivially-assignable type
 * @tparam U possible trivially-assignable type
 */
template <typename T, typename U>
struct is_trivially_assignable : std::bool_constant<__is_trivially_assignable(T, U)> {};

/**
 * @brief `True` if the expression `std::declval<T>() == std::declval<U>()` is
 * well-formed in an unevaluated context and the evaluation of the expression
 * only calls `std::declval` and trivial operations; otherwise, `false`.
 * 
 * @tparam T possible trivially-assignable type
 * @tparam U possible trivially-assignable type
 */
template <typename T, typename U>
inline constexpr bool is_trivially_assignable_v = is_trivially_assignable<T, U>::value;

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
 * @brief If `T` is a referenceable type and is trivially-assignable from a
 * `T&` lvalue reference or `const T&` lvalue reference (aka a trivial copy
 * assignment), then the member `value` is equal to `true`; otherwise, member
 * `value` is equal to `false`.
 * 
 * @tparam T possible trivially-copy-assignable type
 */
template <typename T>
struct is_trivially_copy_assignable : is_trivially_assignable<typename std::add_lvalue_reference<T>::type, typename std::add_lvalue_reference<const T>::type> {};

/**
 * @brief `True` if `T` is a referenceable type and is trivially-assignable
 * from a `T&` lvalue reference or `const T&` lvalue reference (aka a trivial
 * copy assignment); otherwise, `false`.
 * 
 * @tparam T possible trivially-copy-assignable type
 */
template <typename T>
inline constexpr bool is_trivially_copy_assignable_v = is_trivially_copy_assignable<T>::value;

/**
 * @brief If `T` is a referenceable type and is constructible via a constructor
 * accepting a single `const T&` argument (aka a copy constructor) which only
 * calls trivial operations, then the member `value` is equal to `true`;
 * otherwise, member `value` is equal to `false`.
 * 
 * @tparam T possible trivially-copy-constructible type
 */
template <typename T>
struct is_trivially_copy_constructible : is_trivially_constructible<T, details::__add_const_lvalue_reference<T>> {};

/**
 * @brief `True` if `T` is a referenceable type and is constructible via a
 * constructor accepting a single `const T&` argument (aka a copy constructor)
 * which only calls trivial operations; otherwise, `false`.
 * 
 * @tparam T possible trivially-copy-constructible type
 */
template <typename T>
inline constexpr bool is_trivially_copy_constructible_v = is_trivially_copy_constructible<T>::value;

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

/**
 * @brief If `T` is a reference type, or `T` is an object type such that the
 * expression `std::declval<U&>().~U()` (where `U` is
 * `std::remove_all_extents<T>::type`) is well-formed in an unevaluated
 * context, and the destructor only calls trivial operations, then the member
 * `value` is equal to `true`; otherwise, member `value` is equal to `false`.
 * 
 * @note The `__is_trivially_destructible()` function used in the definition of
 * this template is a compiler intrinsic. Compiler support isn't necessary for
 * a correct and conforming implementation of `std::is_trivially_destructible`,
 * but it does make the implementation significantly simpler, as seen in this
 * block from clang's libcxx:
 * https://github.com/llvm/llvm-project/blob/a9d68a5524dea113cace5983697786599cbdce9a/libcxx/include/type_traits#L3036-L3065
 * 
 * @tparam T possible trivially-destructible type
 */
template <typename T>
struct is_trivially_destructible : std::bool_constant<__is_trivially_destructible(T)> {};

/**
 * @brief `True` if `T` is a reference type, or `T` is an object type such that
 * the expression `std::declval<U&>().~U()` (where `U` is
 * `std::remove_all_extents<T>::type`) is well-formed in an unevaluated
 * context, and the destructor only calls trivial operations; otherwise,
 * `false`.
 * 
 * @tparam T possible trivially-destructible type
 */
template <typename T>
inline constexpr bool is_trivially_destructible_v = is_trivially_destructible<T>::value;

/**
 * @brief If `T` is a referenceable type and is trivially-assignable from a
 * `T&` lvalue reference or `T&&` rvalue reference (aka a trivial move
 * assignment), then the member `value` is equal to `true`; otherwise, member
 * `value` is equal to `false`.
 * 
 * @tparam T possible trivially-move-assignable type
 */
template <typename T>
struct is_trivially_move_assignable : is_trivially_assignable<typename std::add_lvalue_reference<T>::type, typename std::add_rvalue_reference<T>::type> {};

/**
 * @brief `True` if `T` is a referenceable type and is trivially-assignable
 * from a `T&` lvalue reference or `T&&` rvalue reference (aka a trivial move
 * assignment); otherwise, `false`.
 * 
 * @tparam T possible trivially-move-assignable type
 */
template <typename T>
inline constexpr bool is_trivially_move_assignable_v = is_trivially_move_assignable<T>::value;

/**
 * @brief If `T` is a referenceable type and is constructible via a constructor
 * accepting a single `T&&` argument (aka a move constructor) which only calls
 * trivial operations, then the member `value` is equal to `true`; otherwise,
 * member `value` is equal to `false`.
 * 
 * @tparam T possible trivially-move-constructible type
 */
template <typename T>
struct is_trivially_move_constructible : is_trivially_constructible<T, typename std::add_rvalue_reference<T>::type> {};

/**
 * @brief `True` if `T` is a referenceable type and is constructible via a
 * constructor accepting a single `T&&` argument (aka a move constructor) which
 * only calls trivial operations; otherwise, `false`.
 * 
 * @tparam T possible trivially-move-constructible type
 */
template <typename T>
inline constexpr bool is_trivially_move_constructible_v = is_trivially_move_constructible<T>::value;

}; // namespace std

#endif // _STD_TYPE_TRAITS_SUPPORTED_OPERATIONS_HPP