/**
 * @file
 * @brief Implementations of the std::char_traits, std::basic_string, and
 * std::string classes.
 * 
 * @note Some comments in this file were based off of/taken directly from the
 * descriptions on en.cppreference.com/w/cpp/string and various subpages. All
 * errors are mine, all credit is theirs.
 */
#ifndef _STD_STRING_HPP
#define _STD_STRING_HPP

// TODO: how to angle-bracket include cstddef (specify self as stdlib?)
#include "cstddef.hpp"

namespace std {

// TODO: class, typedef comments
template <class CharT>
class char_traits {

    typedef CharT char_type;
    typedef int   int_type;
    // TODO: off_type
    // TODO: pos_type
    // TODO: state_type

    /**
     * @brief Assigns a character to a given value.
     * 
     * @param in_a character to assign to
     * @param in_b character value to assign
     * @note Space complexity: O(1)
     * @note Time complexity: O(1)
     */
    constexpr static inline void assign(char_type& in_a,
                                        const char_type& in_b) noexcept {
        in_a = in_b;
    }

    /**
     * @brief Assigns in_char value to in_count characters in the character
     * sequence pointed to by in_cseq.
     * 
     * @param in_cseq pointer to a character sequence to assign to
     * @param in_count length of the character sequence
     * @param in_char character value to assign
     * @return constexpr char_type* in_cseq
     * @note Space complexity: O(1)
     * @note Time complexity: O(in_count)
     */
    constexpr static inline char_type * assign(char_type * in_cseq,
                                               std::size_t in_count,
                                               char_type in_char) {
        auto seq = in_cseq;
        for(; in_count; --in_count, ++in_cseq) {
            assign(*in_cseq, in_char);
        }
        return seq;
    }

    /**
     * @brief Lexicographically compares the first in_count characters of in_a
     * and in_b. If in_count is zero, the character strings are considered
     * equal.
     * 
     * @param in_a pointer to the first character string to compare
     * @param in_b pointer to the second character string to compare
     * @param in_count number of characters to compare from each character string
     * @return constexpr int negative value if in_a is less than in_b. Zero if
     * in_a is equal to in_b. Positive value if in_a is greater than in_b.
     * @note Space complexity: O(1)
     * @note Time complexity: O(in_count)
     */
    constexpr static int compare(const CharT * in_a, const CharT * in_b,
                                 std::size_t in_count) {
        // Compare each character in the strings.
        for(; in_count > 0; --in_count, ++in_a, ++in_b) {
            if(lt(*in_a, *in_b)) {
                // "a" sorts lexicographically before "b".
                return -1;
            } else if(lt(*in_b, *in_a)) {
                // "a" sorts lexicographically after "b".
                return 1;
            }
        }

        // The two strings must be equal.
        return 0;
    }

    /**
     * @brief Compares in_a and in_b for equality.
     * 
     * @param in_a first character value to compare
     * @param in_b second character value to compare
     * @return true if in_a and in_b are equal
     * @return false if in_a and in_b are not equal
     * @note Space complexity: O(1)
     * @note Time complexity: O(1)
     */
    constexpr static inline bool eq(CharT in_a, CharT in_b) noexcept {
        return (in_a == in_b);
    }

    /**
     * @brief Compares in_a and in_b in such a way that they are totally ordered.
     * 
     * @param in_a first character value to compare
     * @param in_b second character value to compare
     * @return true if in_a is less than/lexicographically sorts before in_b
     * @return false if in_a is less than/lexicographically sorts after in_b
     * @note Space complexity: O(1)
     * @note Time complexity: O(1)
     */
    constexpr static inline bool lt(CharT in_a, CharT in_b) noexcept {
        return (in_a < in_b);
    }

    // TODO: move()
    // TODO: copy()
    // TODO: length()
    // TODO: find()
    // TODO: to_char_type()
    // TODO: to_int_type()
    // TODO: eq_int_type()
    // TODO: eof()
    // TODO: not_eof()
};

// TODO: comments
template <class CharT, class Traits = std::char_traits<CharT> /* TODO: , class Allocator = std::polymorphic_allocator<CharT> */>
class basic_string {
    typedef Traits traits_type;
    typedef CharT value_type;
    // TODO: allocator_type
    // TODO: size_type
    // TODO: difference_type
    typedef value_type& reference;
    typedef const value_type& const_reference;
    // TODO: iterator
    // TODO: const_iterator
    // TODO: reverse_iterator
    // TODO: const_reverse_iterator

    // TODO: all the ctor/dtor/member functions
};

// TODO: npos
// TODO: non-member basic_string operators

}; // namespace std

#endif // _STD_STRING_HPP
