#ifndef _STD_ARRAY
#define _STD_ARRAY

#include "assert.h"
#include "stdint.h"

template<typename T, size_t N>
class array {
public:
    class iterator {
    public:
        iterator(const T* in_ptr) : _ptr(in_ptr) {}
        iterator operator++() {
            _ptr++;
            return *this;
        }

        bool operator!=(const iterator& in_other) const {
            return _ptr != in_other._ptr;
        }

        const T& operator*() {
            return *_ptr;
        }

    private:
        const T* _ptr;
    };

    constexpr array() {}

    constexpr T& operator[](const size_t in_idx) {
        //static_assert(in_idx < N, "array index out-of-bounds");
        return _buf[in_idx];
    }

    iterator begin() {
        return iterator(_buf);
    }

    iterator end() {
        return iterator(_buf + N);
    }

private:
    T _buf[N];
};

#endif // _STD_ARRAY
