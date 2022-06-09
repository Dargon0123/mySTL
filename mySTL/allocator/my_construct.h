/*
 * 
 * @brief: This file include two global function that are going as the tools 
 * about construct and destroy.
 * 
 * @function: 
 * 
 * Note: This is an internal header file, included by other STL headers.
 *      You should not attempt to modify it directly.
 * 
 */
#ifndef __MY_CONSTRUCT_H
#define __MY_CONSTRUCT_H


#include <new>
#include "../iterator/my_type_traits.h"
#include "../iterator/my_iterator.h"


namespace mystl {

// construct 构造对象T

// 第一版本: 使用class T 的默认构造方式 T::T()
template <class T> 
inline void construct(T* p) {
    ::new ((void*) p) T();
}

// 第二版本: 使用class T作者自行设计的构造方式 T::T(value)
template <class T> 
inline void construct(T* p, const T& value) {
    ::new ((void*) p) T(value);
}

// 第三版本: "..." parameter pack 没有使用过

// destroy 析构对象T

// 01: 传递一个指针，直接将指针所指之物析构掉
template <class T> 
inline void destroy(T* pointer) {
    pointer->~T();  // 调用 T::~T()
}

// 02: 传递两个迭代器进来
// 使用 __type_traits<>, 找出最适当的析构方法，进阶操作
template <class ForwardIter>
inline void destroy(ForwardIter first, ForwardIter last) {
    typedef typename iterator_traits<ForwardIter>::value_type __value_type;
    typedef typename __type_traits<__value_type>::has_trivial_destructor trivial_destructor;
    __destroy_aux(first, last, trivial_destructor());
}

// 判断元素型别 trivial destructor 
template <class ForwardIter>
void __destroy_aux(ForwardIter first, ForwardIter last, __true_type) {}

// 判断元素型别 non-trivial destructor 
template <class ForwardIter>
void __destroy_aux(ForwardIter first, ForwardIter last, __false_type) {
    // 析构函数不是无关痛痒的，则每一个元素分别调用它去析构
    for ( ; first <last; ++first) {
        destroy(&*first); 
    }
}

} // end of namespace mystl

#endif