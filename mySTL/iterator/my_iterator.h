/**
 * @file my_iterator.h
 * @author dargon0123@163.com
 * @brief iterator 迭代器，针对迭代器推导其具体属性
 * @version 0.1
 * @date 2022-03-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __MY_ITERATOR_H
#define __MY_ITERATOR_H

#include <cstddef>

#include "my_type_traits.h"

namespace mystl {

// 五种iterator_category 基本属性
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirection_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirection_iterator_tag {};

// iterator 模板
// 后三个参数有默认值，新的迭代器只需要提供前两个参数即可
template <class Category, class T, class Distance =ptrdiff_t, 
            class Pointer =T*, class Reference =T& >
struct iterator {
    typedef Category    iterator_category;
    typedef T           value_type;
    typedef Distance    difference_type;
    typedef Pointer     pointer;
    typedef Reference   reference;
};

// 真正能用到的
// iterator_traits 萃取迭代器所指向的类型
template <class Iterator>
struct iterator_traits {
    typedef typename Iterator::iterator_category  iterator_category;
    typedef typename Iterator::value_type         value_type;
    typedef typename Iterator::difference_type    difference_type;
    typedef typename Iterator::pointer            pointer;
    typedef typename Iterator::reference          reference;
};

// 原生指针的偏特化 partial specialization
// 设计iterator_traits 特别版
template <class T>
struct iterator_traits<T*> {
    typedef random_access_iterator_tag   iterator_category;
    typedef T                            value_type;
    typedef ptrdiff_t                    difference_type;
    typedef T*                           pointer;
    typedef T&                           reference;
};

// 原生指向const的指针 pointer to const的偏特化
// 设计iterator_traits 特别版
template <class T>
struct iterator_traits<const T*> {
    typedef random_access_iterator_tag   iterator_category;
    typedef T                            value_type;
    typedef ptrdiff_t                    difference_type;
    typedef const T*                     pointer;
    typedef const T&                     reference;
};

// 该函数获取迭代器的类型 category
template <class Iter>
inline typename iterator_traits<Iter>::iterator_category
iterator_category(const Iter&) {
    typedef typename iterator_traits<Iter>::iterator_category category;
    return category(); // 行为类似int() 的形式
}

// 该函数获取迭代器的 distance type
// static_cast 将一个较大的算术类型赋值给较小的类型时，
// 1，告诉编译器不在乎精度损失 2，避免警告信息
template <class Iter>
inline typename iterator_traits<Iter>::difference_type*
difference_type(const Iter&) {
    typedef typename iterator_traits<Iter>::difference_type difference_type;
    return static_cast<difference_type*>(nullptr);
}

// 该函数获取迭代器的类型 value_type
// static_cast 将一个较大的算术类型赋值给较小的类型时，
// 1，告诉编译器不在乎精度损失 2，避免警告信息
template <class Iter>
inline typename iterator_traits<Iter>::value_type*
value_type(const Iter&) {
    typedef typename iterator_traits<Iter>::value_type value_type;
    return static_cast<value_type*>(nullptr);
}

/**********************************************************************
 * 整组distance 函数计算迭代器first 和 last 之间的距离
 * 
 *********************************************************************/
template <class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
__distance(InputIterator first, InputIterator last, input_iterator_tag) {
    typename iterator_traits<InputIterator>::difference_type n =0;
    while (first !=last) {
        ++n;
        ++first;
    }
    return n;
}


template <class random_access_iterator>
inline typename iterator_traits<random_access_iterator>::difference_type
__distance(random_access_iterator first, random_access_iterator last,random_access_iterator_tag) {
    return last -first;
} 

// 算法接受型别参数 InputIterator，遵循STL命名guize
// 以算法所能接受的最低阶迭代器类型，来为其迭代器型别参数命名
template <class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last) {
    return __distance(first, last, iterator_category(first));
}

/**********************************************************************
 * 以下时整组advance 函数操作使2迭代器移动 distance 距离
 * 可以看出不同的迭代器categoty，具体的效率是不一样的
 *********************************************************************/
template <class InputIterator, class Distance>
inline void __advance(InputIterator& i, Distance n, input_iterator_tag) {
    while (n--) {
        ++i;
    }
}

template <class BidirectionalIterator, class Distance>
inline void __advance(BidirectionalIterator& i, Distance n, bidirection_iterator_tag) {
    if (n >=0) {
        while (n--) ++i;
    }
    else {
        while (n++) --i;
    }
}

template <class RandomAccessIterator, class Distance>
inline void __advance(RandomAccessIterator& i, Distance n, random_access_iterator_tag) {
    i +=n;
}

// STL 算法的一个命名规则：
// 以算法所能接受的最低阶迭代器类型，来为其迭代器型别参数命名
template <class InputIterator, class Distance>
inline void advance(InputIterator& i, Distance n) {
    __advance(i, n, iterator_category(i));
}


} // end of namespace mystl
#endif