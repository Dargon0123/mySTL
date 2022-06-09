/**
 * @file my_map.h
 * @author your name (you@domain.com)
 * @brief 实现map 容器
 * map: 映射，节点为pair <key : value>,会根据key进行自动排序，key不允许重复
 * @version 0.1
 * @date 2022-06-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __MY_MAP_H
#define __MY_MAP_H

// 标准库文件
#include <cstdlib>
#include <cstddef>


#include "../allocator/my_allocator.h"
#include "../allocator/my_uninitialized.h"
#include "../algorithm/my_algorithm.h"
#include "../algorithm/my_help_function.h"
#include "../container/my_AVL_tree.h"


namespace mystl {

// 后面更替为自己的 mystl::less, 进行试试
template <class Key, class T, class Compare =mystl::less<Key>, template<class T1> class Alloc =mystl::allocator>
class map {

public:
    typedef Key                         key_type; // 键值型别
    typedef T                           data_type; // 实值型别
    typedef T                           mapped_type;
    typedef std::pair<const Key, T>     value_type; // 元素型别（键值/实值）
    typedef Compare                     key_compare;

    // 定义functor， 进行调用 “元素比较函数”
    class value_compare 
        : public binary_function <value_type, value_type, bool> {
    friend class map<Key, T, Compare, Alloc>;
    protected:
        Compare comp;
        value_compare(Compare c) : comp(c) {}
    public:
        // 重载函数调用符
        bool operator()(const value_type x, const value_type y) const {
            return comp(x.first, y.first);
        }
    };


private:
    typedef mystl::AVLTree<key_type, value_type, mystl::select_first<value_type>, key_compare, Alloc> rep_type;
    rep_type t; // 以AVL树，来实现map


public:
    typedef typename rep_type::pointer          pointer;
    typedef typename rep_type::const_pointer    const_pointer;
    typedef typename rep_type::reference        reference;
    typedef typename rep_type::const_reference  const_reference;
    // 注意此处迭代器并不想 set 一样设置为const_iterator，
    // 因为它允许用户通过迭代器修改元素的value值，key是不能修改的。
    typedef typename rep_type::iterator         iterator;
    typedef typename rep_type::const_iterator   const_iterator;
    typedef typename rep_type::size_type        size_type;
    typedef typename rep_type::difference_type  difference_type;


public:
    // ctor
    map() : t(Compare()) {}
    explicit map(const Compare& comp) : t(comp) {}

    // 以[first, last)进行set的构造
    template<class InputIterator>
    map(InputIterator first, InputIterator last): t(Compare()) { 
        t.insert_unique(first, last); 
    }
    template<class InputIterator>
    map(InputIterator first, InputIterator last, const Compare& comp): t(comp) { 
        t.insert_unique(first, last); 
    }

    // 拷贝构造
    map(const map& x) : t(x.t) {}
    map& operator=(const map& x) {
        t =x.t;
        return *this;
    }

    // 以下所有的set的操作行为，AVL_tree 都已提供，所以 set 只要传递引用即可

    // accessors
    key_compare key_comp() const { return t.key_comp(); }
    value_compare value_comp() const { return value_compare(t.key_comp()); }
    iterator begin() { return t.begin(); }
    const_iterator begin() const { return t.begin(); }
    iterator end() { return t.end(); }
    const_iterator end() const { return t.end(); }
    bool empty() const { return t.empty(); }
    size_type size() const { return t.size(); }
    size_type max_size() const { return t.max_size(); }
    // []重载下标操作符,稍后进行
    T& operator[](const key_type& k) {
        insert(value_type(k, T()));
        iterator ite =find(k);
        return (*ite).second;

        // return ( * (insert(value_type(k, T())).first) ).second;
    }

    // insert
    void insert(const value_type& x) { t.insert_unique(x); }
    template<class InputIterator>
    void insert(InputIterator first, InputIterator last) { 
        t.insert_unique(first, last); 
    }

    // erase
    void erase(const key_type& x) { t.erase_map(x); }
    void erase(iterator pos) { t.erase_map(pos); }    
    void erase(iterator first, iterator last) { t.erase_map(first, last); }    
    void clear() { t.clear(); }

    // find
    iterator find(const key_type& x) { return t.find(x); }
    const_iterator find(const key_type& x) const { return t.find(x); }
    size_type count(const key_type& x) const { return t.count(x); }
    iterator lower_bound(const key_type& x) { return t.lower_bound(x); }
    iterator upper_bound(const key_type& x) { return t.upper_bound(x); }
    const_iterator lower_bound(const key_type& x) const { return t.lower_bound(x); }
    const_iterator upper_bound(const key_type& x) const { return t.upper_bound(x); }
    std::pair<iterator, iterator> equal_range(const key_type& x)  { return t.equal_range(x); }
    std::pair<const_iterator, const_iterator> equal_range(const key_type& x) const { return t.equal_range(x); }

    // log 调试使用
    void log_map() { t.log_map_tree(); }
    void log_node(iterator it) { t.log_map_node(it.cur); } // 不好，暴露底层容器

    // friend
    // operator ==
    template<class Ky, class Va, class Cp, template<class t> class Al>
    friend bool operator==(const map<Ky, Va, Cp, Al>& x, const map<Ky, Va, Cp, Al>& y);
    // operator <
    template<class Ky, class Va, class Cp, template<class t> class Al>
    friend bool operator<(const  map<Ky, Va, Cp, Al>& x, const  map<Ky, Va, Cp, Al>& y);


}; // end of class map


template<class Ky, class Va, class Cp, template<class t> class Al>
bool operator==(const map<Ky, Va, Cp, Al>& x, const map<Ky, Va, Cp, Al>& y) {
    return x.t ==y.t;
}
template<class Ky, class Va, class Cp, template<class t> class Al>
bool operator<(const map<Ky, Va, Cp, Al>& x, const map<Ky, Va, Cp, Al>& y) {
    return x.t <y.t;
}


} // end of namespace mystl


#endif