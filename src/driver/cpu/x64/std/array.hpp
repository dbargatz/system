#ifndef _STD_ARRAY_HPP
#define _STD_ARRAY_HPP

#include "assert.h"
#include "stdint.h"

namespace std {

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

    T& operator[](size_t in_idx) {
        ASSERT(in_idx < N, "array index out-of-bounds");
        return _buf[in_idx];
    }

    constexpr const T& operator[](size_t in_idx) const noexcept {
        return _buf[in_idx];
    }

    T& at(size_t in_idx) {
        return _buf[in_idx];
    }

    constexpr const T& at(size_t in_idx) const noexcept {
        return _buf[in_idx];
    }

    iterator begin() noexcept {
        return iterator((T*)_buf);
    }

    iterator end() noexcept {
        return iterator((T*)_buf + N);
    }

    constexpr T& front() const noexcept {
        return *begin();
    }

    constexpr T& back() const noexcept {
        return N ? *(end() - 1) : *end();
    }

    constexpr size_t size() const noexcept {
        return N;
    }

    constexpr size_t max_size() const noexcept {
        return N;
    }

    constexpr bool empty() const noexcept {
        return size() == 0;
    }

    T* data() noexcept {
        return __addressof(_buf[0]);
    }

    const T* data() const noexcept {
        return __addressof(_buf[0]);
    }

private:
    T _buf[N];
};

}; // namespace std
#endif // _STD_ARRAY_HPP
