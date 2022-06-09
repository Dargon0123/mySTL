/**
 * @file my_help_function.h
 * @author your name (you@domain.com)
 * @brief 定义一些简单的辅助函数，一元谓词，二元谓词，lambda定制操作
 * 供外部调用
 * @version 0.1
 * @date 2022-05-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __MY_HELP_FUNCTION_H
#define __MY_HELP_FUNCTION_H

#include <cstddef>
#include <cstring>

namespace mystl {

/************************************************************************
 * 定义一元函数, 二元函数的参数型别和返回值型别
 ************************************************************************/
template <class Arg, class Result>
struct unary_function {
    typedef Arg     argument_type;
    typedef Result  retult_type;
};

template <class Arg1, class Arg2, class Result>
struct binary_function {
    typedef Arg1     first_argument_type;
    typedef Arg2     second_argument_type;
    typedef Result   retult_type;
};

/************************************************************************
 * 定义函数对象,重载调用运算符(.)
 * () 类似于函数的性质，可以直接进行调用
 ************************************************************************/
// 函数对象 = 操作
template <class T>
struct equal_to : public binary_function <T, T, bool>
{
    bool operator() (const T& x, const T& y) const { return x ==y; }
};

// 函数对象 != 操作
template <class T>
struct not_equal_to : public binary_function <T, T, bool>
{
    bool operator() (const T& x, const T& y) const { return x !=y; }
};

// 函数对象 > 操作
template <class T>
struct greater : public binary_function <T, T, bool>
{
    bool operator() (const T& x, const T& y) const { return x >y; }
};

// 函数对象 < 操作
template <class T>
struct less : public binary_function <T, T, bool>
{
    bool operator() (const T& x, const T& y) const { return x <y; }
};

// 函数对象 >= 操作
template <class T>
struct greater_equal_to : public binary_function <T, T, bool>
{
    bool operator() (const T& x, const T& y) const { return x >=y; }
};

// 函数对象 <= 操作
template <class T>
struct less_equal_to : public binary_function <T, T, bool>
{
    bool operator() (const T& x, const T& y) const { return x <=y; }
};


/************************************************************************
 * 针对pair，返回第一个元素的型别
 * 供 map 调用
 ************************************************************************/
template <class Pair> 
struct select_first 
    : public unary_function <Pair, typename Pair::first_type> {
    const typename Pair::first_type&
    operator() (const Pair& x) const {
        return x.first;
    }
};


/************************************************************************
 * 比较两个序列
 * f1序列的元素小，返回true，否则返回false
 * 前面全部相等，后面按照长度进行比较
 ************************************************************************/
template <class Iterator1, class Iterator2>
bool list_compare(Iterator1 f1, Iterator1 l1, Iterator2 f2, Iterator2 l2) {
    while (f1 !=l1 && f2 !=l2) {
        if (*f1 <*f2) return true;
        else if (*f2 <*f1) return false;
        ++f1;
        ++f2;
    }
    
    return f1 ==l1 && f2 !=l2;
}

// 重载使用函数comp比较
template <class Iterator1, class Iterator2, class Compared>
bool list_compare(Iterator1 f1, Iterator1 l1, Iterator2 f2, Iterator2 l2, Compared comp) {
    while (f1 !=l1 && f2 !=l2) {
        if (comp(*f1, *f2)) return true;
        else if (comp(*f2, *f1)) return false;
        ++f1;
        ++f2;
    }
    
    return f1 ==l1 && f2 !=l2;
}

// 针对const char* 的特化版本
template<>
bool list_compare(const unsigned char* f1, const unsigned char* l1, const unsigned char* f2, const unsigned char* l2) {
    auto len1 =l1 -f1;
    auto len2 =l2 -f2;
    // 利用memcmp函数比较相等长度部分
    const auto result =std::memcmp(f1, f2, std::min(len1, len2));
    // 相等长度较长比较大。
    // result <0. 则f1 比较小，返回true; >0 则f2比较小，返回false
    return result !=0 ? result <0 : len1 <len2;
}


};  // end of namespace mystl

#endif