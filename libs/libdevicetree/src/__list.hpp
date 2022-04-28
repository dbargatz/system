#ifndef _DEVICETREE_DETAILS_LIST_HPP
#define _DEVICETREE_DETAILS_LIST_HPP

#include "__iterator.hpp"

namespace devicetree::details {

template <typename T>
class list {
private:
    typename T::internal_struct * _first;

public:
    list() { _first = nullptr; }
    list(typename T::internal_struct * in_first) { _first = in_first; }

    iterator<T> begin() noexcept { return iterator<T>(_first); }
    iterator<T> end() noexcept { return iterator<T>(nullptr); }

    T first() noexcept { return T(_first); }
};

}; // namespace devicetree::details

#endif // _DEVICETREE_DETAILS_LIST_HPP