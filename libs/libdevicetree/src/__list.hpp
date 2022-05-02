#ifndef _DEVICETREE_DETAILS_LIST_HPP
#define _DEVICETREE_DETAILS_LIST_HPP

#include "__iterator.hpp"

namespace devicetree::details {

template <typename T, typename V>
class list {
private:
    V * _first;

public:
    list() { _first = nullptr; }
    list(V * in_first) { _first = in_first; }

    iterator<T,V> begin() noexcept { return iterator<T,V>(_first); }
    iterator<T,V> end() noexcept { return iterator<T,V>(nullptr); }

    T first() noexcept { return T(_first); }
};

}; // namespace devicetree::details

#endif // _DEVICETREE_DETAILS_LIST_HPP