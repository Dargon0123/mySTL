/**
 * @file my_allocator.h
 * @author your name (you@domain.com)
 * @brief This is an internal header file, included by other STL headers.
 * You should not attempt to modify it directly.
 * @version 0.1
 * @date 2022-03-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __MY_ALLOCATOR_H
#define __MY_ALLOCATOR_H

#include "my_construct.h"
#include "my_alloc.h"

namespace mystl {

// 提供外部使用
// 包含内存allocate 和 deallocate
// 包含construct 和destruct
template <class T>
class allocator{
public:
    typedef T           value_type;
    typedef T*          pointer;
    typedef const T*    const_pointer;
    typedef T&          reference;
    typedef const T&    const_reference;
    typedef size_t      size_type;
    typedef ptrdiff_t   difference_type;

    // 提供的外部接口

    // 分配内存函数接口
    static T* allocate();
    static T* allocate(size_type n);
    static T* allocate(size_type n, const void* hint);

    // 收回内存函数接口
    static void deallocate(T* p);
    static void deallocate(T* p, size_type );


    // 调用构造函数
    static void construct(T* p);
    static void construct(T* p, const T& value);
    static void construct(T* p, T&& value);

    // 调用析构函数
    static void destroy(T* p);
    static void destroy(T* first, T* last);

public:
    // 默认的allocator 的构造和析构
    allocator() {};
    ~allocator() {};

private:

}; // end of class allocator

// 分配内存函数接口
// 如果定义__USE_MALLOC就使用new和delete操作即可
// 否则就直接使用，设计的内存配置器
#ifdef __USE_MALLOC

template <class T>
T* allocator<T>::allocate() {
    return static_cast<T*>(::operator new(sizeof(T)));
}

template <class T>
T* allocator<T>::allocate(size_type n) {
    if (n ==0) {
        return nullptr;
    }
    return static_cast<T*>(::operator new(n *sizeof(T)));
}

template <class T>
T* allocator<T>::allocate(size_type n, const void* hint) {
    if (n ==0) {
        return nullptr;
    }   
    return static_cast<T*>(::operator new(n *sizeof(T)));  
}

// 收回内存函数接口
template <class T>
void allocator<T>::deallocate(T* p) {
    if (p == nullptr) {
        return;
    }
    ::operator delete(p);
}

template <class T>
void allocator<T>::deallocate(T* p, size_type ) {
    /* 同上 */
    if (p ==nullptr) {
        return;
    }
    ::operator delete(p); // 对应的new 找对应delete操作
}

#else
template <class T>
T* allocator<T>::allocate() {
    return static_cast<T*>(mystl::alloc::allocate(sizeof(T)));
}

template <class T>
T* allocator<T>::allocate(size_type n) {
    if (n ==0) {
        return nullptr;
    }
    return static_cast<T*>(mystl::alloc::allocate(n *sizeof(T)));
}

template <class T>
T* allocator<T>::allocate(size_type n, const void* hint) {
    if (n ==0) {
        return nullptr;
    }   
    return static_cast<T*>(mystl::alloc::allocate(n *sizeof(T)));  
}

// 收回内存函数接口
template <class T>
void allocator<T>::deallocate(T* p) {
    if (p == nullptr) {
        return;
    }
    mystl::alloc::deallocate(p, sizeof(T));
}

template <class T>
void allocator<T>::deallocate(T* p, size_type n) {
    /* 同上 */
    if (p ==nullptr) {
        return;
    }
    mystl::alloc::deallocate(p, n *sizeof(T)); // 对应的new 找对应delete操作
}
#endif

// 调用构造函数
template <class T>
void allocator<T>::construct(T* p) {
    mystl::construct(p);
}

template <class T>
void allocator<T>::construct(T* p, const T& value) {
    mystl::construct(p, value);
}

template <class T>
void allocator<T>::construct(T* p, T&& value) {
    // value 以右值的方式传递进来
    mystl::construct(p, value);
}

// 调用析构函数
template <class T>
void allocator<T>::destroy(T* p) {
    mystl::destroy(p);
}

template <class T>
void allocator<T>::destroy(T* first, T* last) {
    // 暂时未实现
    mystl::destroy(first, last);
}

} // end of namespace mystl


#endif