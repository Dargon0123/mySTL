/**
 * @file my_set.h
 * @author your name (you@domain.com)
 * @brief set容器，以AVL树作为基础
 * @version 0.1
 * @date 2022-06-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#ifndef __MY_SET_H
#define __MY_SET_H


// 标准库文件
#include <cstdlib>
#include <cstddef>

#include "../allocator/my_allocator.h"
#include "../allocator/my_uninitialized.h"
#include "../algorithm/my_algorithm.h"
#include "../algorithm/my_help_function.h"
#include "../container/my_AVL_tree.h"

namespace mystl {

template <class Key, class Compare =std::less<Key>, template<class T> class Alloc =allocator>
class set {
public:
    typedef Key         key_type;
    typedef Key         value_type;
    typedef Compare     key_compare;
    typedef Compare     value_compare;

private:
    typedef mystl::AVLTree<key_type, value_type, std::_Identity<value_type>, key_compare, Alloc>  rep_type;
    // 采用 AVL树，来实现 set容器
    rep_type t;

public:
    typedef typename rep_type::const_pointer    pointer;
    typedef typename rep_type::const_pointer    const_pointer;
    typedef typename rep_type::const_reference  reference;
    typedef typename rep_type::const_reference  const_reference;
    // 定义为const_iterator，因为set的元素有一定的次序排列
    // 不允许用户在任意处进行写入操作
    typedef typename rep_type::const_iterator   iterator;
    typedef typename rep_type::const_iterator   const_iterator;
    typedef typename rep_type::size_type        size_type;
    typedef typename rep_type::difference_type  difference_type;


public:
    // ctor
    set() : t(Compare()) {};
    explicit set(const Compare& comp) : t(comp) {}

    // 以[first, last)进行set的构造
    template<class InputIterator>
    set(InputIterator first, InputIterator last): t(Compare()) { 
        t.insert_unique(first, last); 
    }
    template<class InputIterator>
    set(InputIterator first, InputIterator last, const Compare& comp): t(comp) { 
        t.insert_unique(first, last); 
    }

    // 拷贝构造
    set(const set& x): t(x.t) {}
    set& operator=(const set& x) {
        t =x.t;
        return *this;
    }

    // 以下所有的set的操作行为，AVL_tree 都已提供，所以 set 只要传递引用即可
    // accessors
    key_compare key_comp() const { return t.key_comp(); }
    value_compare value_comp() const { return t.key_comp(); }
    iterator begin() const { return t.begin(); }
    iterator end() const { return t.end(); }
    bool empty() const { return t.empty(); }
    size_type size() const { return t.size(); }
    size_type max_size() const { return t.max_size(); }

    // insert
    void insert(const value_type& x) { t.insert_unique(x); }
    template<class InputIterator>
    void insert(InputIterator first, InputIterator last) { 
        t.insert_unique(first, last); 
    }

    // erase
    void erase(const key_type& x) { t.erase(x); }
    void erase(iterator pos) { 
        using rep_iterator = typename rep_type::iterator;
        t.erase((rep_iterator&)pos);
    }
    void erase(iterator first, iterator last) {
        using rep_iterator = typename rep_type::iterator;
        t.erase((rep_iterator&)first, (rep_iterator&)last);
    }
    void clear() { t.clear(); }

    // find
    iterator find(const key_type& x) const { return t.find(x); }
    size_type count(const key_type& x) const { return t.count(x); }
    iterator lower_bound(const key_type& x) const { return t.lower_bound(x); }
    iterator upper_bound(const key_type& x) const { return t.upper_bound(x); }
    std::pair<iterator, iterator> equal_range(const key_type& x) const { return t.equal_range(x); }

    // log 调试使用
    void log_set() { t.log_tree(); }
    void log_node(iterator it) { t.log_node(it.cur); } // 待测试

    // friend
    template<class Ky, class Cp, template<class t> class Al>
    friend bool operator==(const set<Ky, Cp, Al>& x, const set<Ky, Cp, Al>& y);

private:


}; // end of class set

// operator ==
template<class Ky, class Cp, template<class t> class Al>
bool operator==(const set<Ky, Cp, Al>& x, const set<Ky, Cp, Al>& y) {
    return x.t ==y.t;
}
// operator !=
template<class Ky, class Cp, template<class t> class Al>
bool operator!=(const set<Ky, Cp, Al>& x, const set<Ky, Cp, Al>& y) {
    return !(x ==y);
}
// operator <
template<class Ky, class Cp, template<class t> class Al>
bool operator<(const set<Ky, Cp, Al>& x, const set<Ky, Cp, Al>& y) {
    return (x.t <y.t);
}

}  // end of namespace mystl



#endif