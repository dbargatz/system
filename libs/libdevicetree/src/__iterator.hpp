#ifndef _DEVICETREE_DETAILS_ITERATOR_HPP
#define _DEVICETREE_DETAILS_ITERATOR_HPP

namespace devicetree::details {

template <typename T, typename V>
class iterator {
private:
    V * _current;

public:
    iterator(V * in_ptr) { _current = in_ptr; }
    bool operator==(const iterator<T,V>& in_other) const { return (_current == in_other._current); }
    bool operator!=(const iterator<T,V>& in_other) const { return !(*this == in_other); }

    T operator*() { return T(_current); }
    iterator<T,V>& operator++();
};

}; // namespace devicetree::details

#endif // _DEVICETREE_DETAILS_ITERATOR_HPP