#ifndef _STD_TYPE_TRAITS_HPP
#define _STD_TYPE_TRAITS_HPP

namespace std {

template <typename T, T v>
struct integral_constant {
    constexpr static T              value = v;
    using value_type = T;
    using type = integral_constant;

    constexpr operator value_type() const noexcept { return value; }
    constexpr value_type operator ()() const noexcept { return value; }
};

template <bool v>
using bool_constant = integral_constant<bool, v>;

using true_type  = bool_constant<true>;
using false_type = bool_constant<false>;

template <typename T>
struct remove_reference { typedef T type; };

template <typename T>
struct remove_reference<T&> { typedef T type; };

template <typename T>
struct remove_reference<T&&> { typedef T type; };

template <typename T>
using remove_reference_t = typename remove_reference<T>::type;

template <typename T>
struct is_reference : false_type {};

template <typename T>
struct is_reference<T&> : true_type {};

template <typename T>
struct is_reference<T&&> : true_type {};

template <typename T>
struct is_lvalue_reference : false_type {};

template <typename T>
struct is_lvalue_reference<T&> : true_type {};

template <typename T>
struct is_rvalue_reference : false_type {};

template <typename T>
struct is_rvalue_reference<T&&> : true_type {};

}; // namespace std

#endif // _STD_TYPE_TRAITS_HPP