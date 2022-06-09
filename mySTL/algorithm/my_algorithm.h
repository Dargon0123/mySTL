/**
 * @file my_algorithm.h
 * @author your name (you@domain.com)
 * @brief STL 基本算法
 * @version 0.1
 * @date 2022-03-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __MY_ALGORITHM_H
#define __MY_ALGORITHM_H

#include "../iterator/my_iterator.h"
#include "../iterator/my_type_traits.h"

namespace mystl {

/**********************************************************************
 * fill_n() 的函数体
 * 从first 位置开始，填充n 个值到内存里面
 * 返回的填充结束的位置
 *********************************************************************/
template <class OutputIterator, class Size, class T>
OutputIterator fill_n(OutputIterator first, Size n, const T& val) {
    while (n--) {
        *first =val;
        ++first;
    }
    return first;
}

/************************************************************************
 * fill(first, last, val) 的函数体
 * 在[first, last) 区间内，填充val值到内存里面
 * 无返回值
 ***********************************************************************/
// ForwardIterator 版本的 fill()
template <class ForwardIterator, class T> 
void __fill(ForwardIterator first, ForwardIterator last, const T& val, forward_iterator_tag) {
    for ( ; first !=last; ++first) {
        *first =val;
    }
}
// RandomAccessIterator 版本的 fill()
template <class RandomAccessIterator, class T> 
void __fill(RandomAccessIterator first, RandomAccessIterator last, const T& val, random_access_iterator_tag) {
    // 调用fill_n()
    mystl::fill_n(first, last -first, val);
}
template <class ForwardIterator, class T> 
void fill(ForwardIterator first, ForwardIterator last, const T& val) {
    __fill(first, last, val, iterator_category(first));
}

/************************************************************************
 * copy() 的函数体
 * 把 [first, last) 上的内容复制到以result为起始的空间，返回赋值结束的位置
 ***********************************************************************/
// InputIterator 版本
// 完全泛化的__copy_dispatch 使用
template<class InputIterator, class OutputIterator>
OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag) {
    // 以迭代器等同与否，决定循环是否相等，速度慢
    for ( ; first !=last; ++result, ++first) {
        *result =*first;
    }
    return result;
}
// RandomAccessIterator 版本
// 完全泛化的__copy_dispatch 使用
template<class RandomAccessIterator, class OutputIterator>
OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last, 
                        OutputIterator result, random_access_iterator_tag) {
    // 划分出的一个函数，为的是其它地方也能用到                        
    return  __copy_d(first, last, result, difference_type(first));                       
}
// __copy_d
template <class RandomAccessIterator, class OutputIterator, class Distance>
OutputIterator __copy_d(RandomAccessIterator first, RandomAccessIterator last, 
                        OutputIterator result, Distance*) {
    // 以n 来决定循环的执行次数，明显速度要快（上面的情况判断需要调用迭代器的 == 操作符） 
    for (Distance n =last -first; n >0; --n, ++result, ++first) {
        *result =*first;
    } 
    return result;                       
}

// copy_t trivial 版本
template<class T>
T* __copy_t(const T* first, const T* last, T* result, __true_type) {
    // 调用库函数 memmove(),相对mencopy() 函数做的更好
    memmove(result, first, (last -first)* sizeof(T));
    return result +(last -first);
}
// copy_t non-trivial 版本
template<class T>
T* __copy_t(const T* first, const T* last, T* result, __false_type) {
    // 原生指针也是一种随机访问指针
    return __copy_d(first, last, result, difference_type(first));
} 

// __copy_dispatch() 的实现，重载函数调用运算符
// 完全泛化版本
template <class InputIterator, class OutputIterator> 
struct __copy_dispatch {
    OutputIterator operator() (InputIterator first, InputIterator last, OutputIterator result) {
        return __copy(first, last, result, iterator_category(first));
    }
};
// __copy_dispatch() 的偏特化 T*, T* 版本1
// 针对原型指针的 partial specialization
template <class T>
struct __copy_dispatch<T*, T*> {
    T* operator() (T* first, T* last, T* result) {
        typedef typename __type_traits<T>::has_trivial_assignment_operator t;
        return __copy_t(first, last, result, t());
    }   
};
// __copy_dispatch() 的偏特化const T*,T* 版本2
// 针对常量指针的 partial specialization
template <class T>
struct __copy_dispatch<const T*, T*> {
    T* operator() (const T* first, const T* last, T* result) {
        typedef typename __type_traits<T>::has_trivial_assignment_operator t;
        return __copy_t(first, last, result, t());
    }
};

// copy的对外接口
template <class InputIterator, class OutputIterator> 
OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result) {
    return __copy_dispatch<InputIterator, OutputIterator>()
                (first, last, result);
}

/************************************************************************
 * copy_backward() 的函数体,整体结构与copy() 相似
 * 把 [first, last) 上的内容逆向复制到以result -1为终点的空间，返回赋值结束的位置
 * [first, last)  --> [result -(last -first), result)
 ***********************************************************************/
// BidirectionalIterator 版本
// 完全泛化的__copy_backward_dispatch() 使用
template <class BidirectionalIterator1, class BidirectionalIterator2>
BidirectionalIterator2 __copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result, bidirection_iterator_tag) {
    // 只能以迭代器等同与否，判断循环是否继续，速度慢
    while (first !=last) {
        // 逆向逐个复制元素 copy operator assignment
        *(--result) =*(--last);
    }
}
// BidirectionalIterator 版本
// 完全泛化的__copy_backward_dispatch() 使用
template <class RandomAccessIterator, class BidirectionalIterator>
BidirectionalIterator __copy_backward(RandomAccessIterator first, RandomAccessIterator last, BidirectionalIterator result, random_access_iterator_tag) {
    // 划分出的一个函数，为的是其它地方也能用到
    return __copy_backward_d(first, last, result, difference_type(first));
}
// __copy_backward_d
template <class RandomAccessIterator, class BidirectionalIterator, class Distance>
BidirectionalIterator __copy_backward_d(RandomAccessIterator first, RandomAccessIterator last, BidirectionalIterator result, Distance* ) {
    // 以n 决定循环次数，速度快
    for (Distance n =last -first; n >0; --n) {
        *(--result) =*(--last);
    }
    return result;
}

// __copy_backward_t trivial 版本
template <class T>
T* __copy_backward_t(const T* first, const T* last, T* result, __true_type) {
    // 直接调用 memmove,以字节为单位copy，在内存覆盖比memcpy函数做得更好
    const ptrdiff_t num =last -first;
    memmove(result -num, first, num *sizeof(T));
    return result -num;
}
// __copy_backward_t non-trivial 版本
template <class T>
T* __copy_backward_t(const T* first, const T* last, T* result, __false_type) {
    // 原生指针也是一种RandomAccessIterator
    return __copy_backward_d(first, last, result, difference_type(first));
}

// __copy_backward_dispatch() 的实现，重载函数调用运算符
// 完全泛化版本
template <class BidirectionalIterator1, class BidirectionalIterator2>
struct __copy_backward_dispatch {
    BidirectionalIterator2 operator() (BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result) {
        return __copy_backward(first, last, result, iterator_category(first));
    }
};
// __copy_backward_dispatch() 的偏特化 T*, T* 版本1
// 针对原型指针的 partial specialization
template <class T>
struct __copy_backward_dispatch<T*, T*> {
    T* operator() (T* first, T* last, T* result) {
        typedef typename __type_traits<T>::has_trivial_assignment_operator t;
        return __copy_backward_t(first, last, result, t());
    }
};
// __copy_backward_dispatch() 的偏特化 const T*, T* 版本1
// 针对原型指针的 partial specialization
template <class T>
struct __copy_backward_dispatch<const T*, T*> {
    T* operator() (const T* first, const T* last, T* result) {
        typedef typename __type_traits<T>::has_trivial_assignment_operator t;
        return __copy_backward_t(first, last, result, t());
    }
};


// copy_backward() 的对外接口
template <class BidirectionalIterator1, class BidirectionalIterator2>
BidirectionalIterator2 
copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result) {
    // 调用的同样是函数操作符
    return __copy_backward_dispatch<BidirectionalIterator1, BidirectionalIterator2>() (first, last, result);
}

/************************************************************************
 * swap 的函数体
 * 运用异或运算符进行交换
 ************************************************************************/
template <class T>
void swap(T& a, T& b) {
    a ^= b;
    b ^= a;
    a ^= b;
}

/************************************************************************
 * max 的函数体
 * 取两者较大值
 ************************************************************************/
template <class T>
const T& max(const T& lhs, const T& rhs) {
    return lhs <rhs ? rhs : lhs;
}

//重载版本 暂时未实现

} // end of namespace mystl

#endif