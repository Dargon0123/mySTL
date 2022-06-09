/**
 * @file my_uninitialized.h
 * @author your name (you@domain.com)
 * @brief 声明三个全局函数，用于对未初始化空间构造元素，fill or copy 方式
 * @version 0.1
 * @date 2022-03-12
 * 
 * @copyright Copyright (c) 2022
 * 
 * @note  This is an internal header file, included by other STL headers.
 *      You should not attempt to modify it directly.
 */
#ifndef __MY_UNINITIALIZED_H
#define __MY_UNINITIALIZED_H

#include <memory>
#include <iostream>
#include "my_construct.h"
#include "../iterator/my_iterator.h"
#include "../iterator/my_type_traits.h"
#include "../algorithm/my_algorithm.h"

namespace mystl {

/*******************************************************
 * uninitialized_fill_n() 的函数体
 * 从first 位置开始，插入n 个初始值为x 的对象内存
 * 返回的填充结束的位置
 ******************************************************/
// 对应的是所谓的trivial 类型的construct
template <class ForwardIterator, class Size, class T>
ForwardIterator __uninitialize_fill_n_aux(ForwardIterator first, Size n, const T& x, __true_type) {
    // 不使用默认的construct 去构建
    // 调用STL fill() 算法去填充， algorithm内容待填充
    return fill_n(first, n, x);
}

// 处理non-trivial 类型的construct
template <class ForwardIterator, class Size, class T>
ForwardIterator __uninitialize_fill_n_aux(ForwardIterator first, Size n, const T& x, __false_type) {
    ForwardIterator cur =first;
    // [&*cur] cur可能是原生指针，也可能是迭代器类型，先解引用在&地址
    while (n--) {
        construct(&*cur, x); // 调用全局的构建函数
        ++cur;
    }
    return cur;
}

template <class ForwardIterator, class Size, class T, class T1>
ForwardIterator __uninitialize_fill_n(ForwardIterator first, Size n, const T& x, T1*) {
    typedef typename __type_traits<T1>::is_POD_type is_POD;
    return __uninitialize_fill_n_aux(first, n, x, is_POD());
}

template <class ForwardIterator, class Size, class T>
ForwardIterator uninitialize_fill_n(ForwardIterator first, Size n, const T& x) {
    // 利用value_type() 获取迭代器类型
    return __uninitialize_fill_n(first, n, x, value_type(first));
}

/*******************************************************
 * uninitialized_fill(first, last, x) 的函数体
 * 在[first, last) 区间位置，插入初始值为x 的对象内存
 * 无返回值类型
 ******************************************************/
// 处理non-trivial 类型的construct
template <class ForwardIterator, class T>
void __uninitialize_fill_aux(ForwardIterator first, ForwardIterator last, T& x, __true_type) {
    // 不使用默认的construct 去构建
    // 调用STL fill() 算法去填充， algorithm内容待填充
    fill(first, last, x);
}

// 处理non-trivial 类型的construct
template <class ForwardIterator, class T>
void __uninitialize_fill_aux(ForwardIterator first, ForwardIterator last, T& x, __false_type) {
    ForwardIterator cur =first;
    // [&*cur] cur可能是原生指针，也可能是迭代器类型，先解引用在&地址
    while (cur !=last) {
        construct(&*cur, x);
        ++cur;
    }
}

template <class ForwardIterator, class T, class T1>
void uninitialize_fill(ForwardIterator first, ForwardIterator last, T& x, T1*) {
    typedef typename __type_traits<T1>::is_POD_type is_POD;
    return __uninitialize_fill_aux(first, last, x, is_POD());
}

template <class ForwardIterator, class T>
void uninitialize_fill(ForwardIterator first, ForwardIterator last, T& x) {
    __uninitialize_fill(first, last, x, value_type(first));
}

/*******************************************************
 * uninitialized_copy(first, last, result) 的函数体
 * 在[first, last) 区间位置的内存copy 到result 开始的地点
 * 返回copy 结束的位置
 ******************************************************/
// 处理non-trivial 类型的construct
template <class InputIterator, class ForwardIterator>
ForwardIterator __uninitialize_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __true_type) {
    // 不使用默认的construct 去构建
    // 调用STL copy() 算法去填充， algorithm内容待填充!!! :-)
    return copy(first, last, result);
}

// 处理non-trivial 类型的construct
template <class InputIterator, class ForwardIterator>
ForwardIterator __uninitialize_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __false_type) {
    ForwardIterator cur =result;
    // [&*cur] cur可能是原生指针，也可能是迭代器类型，先解引用在&地址
    while (first !=last) {
        construct(&*cur, *first);
        ++cur;
        ++first;
    }
    return cur;
}

template <class InputIterator, class ForwardIterator, class T>
ForwardIterator __uninitialize_copy(InputIterator first, InputIterator last, ForwardIterator result, T*) {
    typedef typename __type_traits<T>::is_POD_type is_POD;
    return __uninitialize_copy_aux(first, last, result, is_POD());
}

template <class InputIterator, class ForwardIterator>
ForwardIterator uninitialize_copy(InputIterator first, InputIterator last, ForwardIterator result) {
    return __uninitialize_copy(first, last, result, value_type(result));
}

} // end of namespace mystl

#endif