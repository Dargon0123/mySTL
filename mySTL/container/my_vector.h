/**
 * @file my_vector.h
 * @author your name (you@domain.com)
 * @brief container vector 向量
 * @version 0.1
 * @date 2022-03-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __MY_VECTOR_H
#define __MY_VECTOR_H

#include <cstdlib>
#include <cstddef>

#include "../allocator/my_allocator.h"
#include "../allocator/my_uninitialized.h"
#include "../algorithm/my_algorithm.h"

namespace mystl {

// 很明显这是使用的 自定义的allocator（效率不高）已处理
template <class T, class Alloc =allocator<T>>
class vector {

public:
    // vector 的嵌套型别定义
    typedef T               value_type;
    typedef value_type*     pointer;
    typedef value_type*     iterator; // vector 迭代器是原生指针，不是random Access iterator
    typedef const iterator  const_iterator;
    typedef value_type&     reference;
    typedef const reference const_reference;
    typedef size_t          size_type;
    typedef ptrdiff_t       difference_type;

// 受保护的类成员，其他的继承于此的类也可以有访问下面的
protected:
    iterator start;             // 目前使用空间的头
    iterator finish;            // 目前使用空间的尾
    iterator end_of_storage;    // 目前可用空间的尾


    // 分配内存allocator
    typedef Alloc  data_allocator;

    // 内存回收
    void deallocate() {
        if (start) {
            // 调用第二版本的destructor
            data_allocator::deallocate(start, end_of_storage -start);
        }
    }
    // 配置空间相关，填满内容以fill or copy 方式
    iterator allocate_and_fill(size_type n, const T& value);
    void fill_initialize(size_type n, const T& value);
    template<class InputIterator>
    iterator allocate_and_copy(InputIterator first, InputIterator last);
    template<class InputIterator>
    void copy_initialize(InputIterator first, InputIterator last);


    // 修改容器内部相关操作，供外部接口使用
    // push_back调用 insert(pos, val)
    void insert_aux(iterator position, const T& value);
    // insert(pos, n, val)调用
    void fill_insert(iterator position, size_type n, const T& value);
    // insert(pos, first, last)调用
    template <class InputIterator>
    void range_insert(iterator position, InputIterator first, InputIterator last, input_iterator_tag);
    // insert(pos, first, last)调用
    template <class ForwardIterator>
    void range_insert(iterator position, ForwardIterator first, ForwardIterator last, forward_iterator_tag);


public:
    // 构造函数，析构函数，赋值运算符
    // fill 初始化
    vector() : start(nullptr), finish(nullptr), end_of_storage(nullptr) {};
    vector(size_type n, const T& val) { fill_initialize(n, val); }
    vector(int n, const T& val) { fill_initialize(n, val); }
    vector(long n, const T& val) { fill_initialize(n, val); }
    // copy 初始化
    template <class InputIterator>
    vector(InputIterator first, InputIterator last) { copy_initialize(first, last); }
    // explicit 防止其他类型对该类类型的隐式转换，仍然可以显示调用
    explicit vector(size_type n) { fill_initialize(n, T()); } 
    // 拷贝构造
    vector(const vector & rhs) { copy_initialize(rhs.start, rhs.finish); } // 左值引用
    vector(vector && rhs); // 右值引用
    // 重载赋值"="运算符
    vector& operator = (const vector& rhs);
    vector& operator = (vector&& rhs);
    // 析构函数
    ~vector();


    // 迭代器相关
    iterator begin() { return start; }
    const_iterator begin() const { return start; }
    const_iterator cbegin() const { return start; }
    iterator end() { return finish; }
    const_iterator end() const { return finish; }
    const_iterator cend() const { return finish; }


    // 访问元素相关
    reference operator[] (const size_type i) { return *(begin() +i); }
    const_reference operator[] (const size_type i) const { return *(begin() +i); }
    reference at(size_type n) { range_check(n); return (*this)[n]; }
    const_reference at(size_type n) const { range_check(n); return (*this)[n]; }
    reference front() { return *begin(); }
    const_reference front() const { return *begin(); }
    reference back() { return *end(); }
    const_reference back() const { return *end(); }
    // 检查越界抛出异常
    void range_check(size_type n) const {
        if (n >=this->size()) throw "vector range error!";
    } 


    // 修改容器相关，增删改查操作
    void push_back(const value_type& value);
    void pop_back();
    iterator erase(iterator position);
    iterator erase(iterator first, iterator last);
    // 未释放空间，内存还是被占用着的
    void clear() { erase(begin(), end()); }
    void swap(vector& rhs);
    iterator insert(iterator position, const value_type& value);
    void insert(iterator position, size_type n, const value_type& value);
    template<class InputIterator>
    void insert(iterator position, InputIterator first, InputIterator last);


    // 与容量相关
    size_type max_size() const { return size_type(-1) /sizeof(T);  }
    difference_type size() const { return size_type (finish -start); }
    difference_type capacity() const { return (end_of_storage -start); }
    bool empty() const { return (start ==finish); }
    void reserve(size_type n);
    // value_type()对应一个默认初始值，类似int()
    void resize(size_type new_size, const value_type & value =value_type());
    // 放弃capacity的容量，end_of_storage -finish的空间
    // 缩小空间，直接破坏destroy掉超出空间部分的内容
    void shrink_to_fit();

};


/**********************************************************************
 * 配置空间相关,具体函数实现
 * 填满内容以 fill or copy 的方式，内部函数，供构造，拷贝使用
 *********************************************************************/
// fill 填充的实现
template<class T, class Alloc>
typename vector<T, Alloc>::iterator 
vector<T, Alloc>::allocate_and_fill(size_type n, const T& value) {
    iterator result =data_allocator::allocate(n);
    // 调用全局函数进行fill
    mystl::uninitialize_fill_n(result, n, value);
    return result;
}
template<class T, class Alloc>
void vector<T, Alloc>::fill_initialize(size_type n, const T& value) {
        start =allocate_and_fill(n ,value);
        finish =start +n;
        end_of_storage =finish;
}
// copy 填充的实现
template<class T, class Alloc>
template<class InputIterator>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::allocate_and_copy(InputIterator first, InputIterator last) {
    size_type n =static_cast<size_type>(last -first);
    iterator result =data_allocator::allocate(n);
    // 调用全局函数进行 copy
    mystl::uninitialize_copy(first, last, result);
    return result;
}
template<class T, class Alloc>
template<class InputIterator>
void vector<T, Alloc>::copy_initialize(InputIterator first, InputIterator last) {
    size_type n =static_cast<size_type>(last -first);
    start =allocate_and_copy(first, last);
    finish =end_of_storage =start +n;
}
// 拷贝构造，右值引用方式
template<class T, class Alloc>
vector<T, Alloc>::vector(vector && rhs)
    : start(rhs.start),
      finish(rhs.finish),
      end_of_storage(rhs.end_of_storage) {
    rhs.start =rhs.finish =rhs.end_of_storage =nullptr;
}
// 运算符重载“=”，rhs后面该怎么处理
template<class T, class Alloc>
vector<T, Alloc>& vector<T, Alloc>::operator = (const vector& rhs) {
    if (&rhs != this) {
        const size_type rhs_len =rhs.size();
        // > this->capacity() 重新申请一块足够大的内存
        if (rhs_len > this->capacity()) {
            iterator new_start =allocate_and_copy(rhs.begin(), rhs.end());
            // 析构原对象
            mystl::destroy(start, finish);
            // 释放内存
            data_allocator::deallocate(start, capacity());
            // 更新新对象
            start =new_start;
            end_of_storage =start +rhs_len;
        }
        // 有空间足够容纳 rhs
        // >size, <=capacity
        else if (rhs_len >size()) {
            // <= size 部分，直接将rhs copy过来
            mystl::copy(rhs.begin(), rhs.begin() +size(), start);
            // > size 部分， 还未构造成对象
            mystl::uninitialize_copy(rhs.begin() +size(), rhs.end(), finish);
        }
        // <= size, 足够容纳rhs，还得删除自己的一部分
        else {
            iterator it =mystl::copy(rhs.begin(), rhs.end(), start);
            // 析构掉 [it, finish）
            mystl::destroy(it, finish);
        }
        finish =start +rhs_len;
    }
    return *this;
}
template<class T, class Alloc>
vector<T, Alloc>& vector<T, Alloc>::operator = (vector&& rhs) {
    // 这个方式比较直接,这一块得好好学学，如此神技
    if (this != &rhs) {
        // 析构原对象
        if (size()) {
            mystl::destroy(start, finish);
        }            
        // 释放内存
        if (capacity()) {
            data_allocator::deallocate(start, capacity());
        }
        start =rhs.start;
        finish =rhs.finish;
        end_of_storage =rhs.end_of_storage;
        rhs.start =rhs.finish =rhs.end_of_storage =nullptr;
    }
    return *this;
}
// 析构函数
template<class T, class Alloc>
vector<T, Alloc>::~vector() {
    // 调用T的析构函数
    if (size() !=0) mystl::destroy(begin(), end());
}


/**********************************************************************
 * 容器内部相关接口，具体函数实现
 * 修改容器内部相关操作，供外部接口使用，增删改查
 *********************************************************************/
// push_back和 insert(pos, val)调用
template<class T, class Alloc>
void vector<T, Alloc>::insert_aux(iterator position, const T& value) {
    // 检查还有备用空间
    if (finish != end_of_storage) {
        // 现在备用空间起始处 构建一个该类型元素，以最后一个元素初始化它
        construct(finish, *(finish -1));
        ++finish;
        T x_copy =value;
        mystl::copy_backward(position, finish -2, finish -1);
        *position =x_copy; // 相当于将原来的position 的值覆盖掉
    }
    // 没有备用空间，得先allocate 内存空间啦，在insert操作
    else {
        const size_type old_size =size();
        const size_type len =(old_size !=0 ? 2 *old_size : 1);
        // 以上配置原则；如果原内存为0，则配置一个元素
        // 如果原大小不为0，则配置为原大小的两倍，
        // 前半段用来防止原始数据，后半段用来放置新数据

        // 开始alloctae 内存
        iterator new_start =data_allocator::allocate(len);
        iterator new_finish =new_start;
        try {
            // 将原vector 拷贝到新内存这里
            new_finish =mystl::uninitialize_copy(start, position, new_start);
            // 为position 位置上的新元素设定初值 x
            construct(new_finish, value);
            ++new_finish;
            // 不一定都是push_back()操作，还有position在中间的case
            // 将position及后面的部分拷贝过来（也可能被insert(p, x）调用)
            new_finish =mystl::uninitialize_copy(position, finish, new_finish);
        }
        catch(...) {
            // "commit or rollback"
            mystl::destroy(new_start, new_finish); //  进行析构
            data_allocator::deallocate(new_start, len); // 内存回收
            throw;
        }

        // 析构并释放原 vector
        mystl::destroy(begin(), end());
        data_allocator::deallocate(begin(), capacity());

        // 调整迭代器，指向新 vector
        start =new_start;
        finish =new_finish;
        end_of_storage =new_start +len;
    }
}

// fill_insert(position, n, value) 插入n 个value
// insert(position, n, value) 调用
template<class T, class Alloc>
void vector<T, Alloc>::fill_insert(iterator position, size_type n, const T& value) {
    if (n ==0) return;
    // 备用空间 >= n（新增元素个数)，不需要扩展内存空间
    if (size_type(end_of_storage -finish) >=n) {
        T value_copy =value;
        // 计算插入点之后的现有元素个数
        const size_type elems_after =finish -position;
        iterator old_finish =finish;
        // “插入点之后的现有元素个数” > “新增元素个数”
        if (elems_after >n) {
            mystl::uninitialize_copy(finish -n, finish, finish);
            finish +=n; // vector 尾端标记后移
            mystl::copy_backward(position, old_finish -n, old_finish);
            mystl::fill(position, position +n, value_copy);
        }
        // “插入点之后的现有元素个数” <= “新增元素个数”
        else {
            mystl::uninitialize_fill_n(finish, n -elems_after, value_copy);
            finish += (n -elems_after);
            mystl::uninitialize_copy(position, old_finish, finish);
            finish +=elems_after; // vector 尾端标记后移
            mystl::fill(position, old_finish, value_copy);
        }
    }
    // 备用空间 >= n（新增元素个数)，需要扩展内存空间
    // 首先确定新长度: 旧长度的两倍，或旧长度+ 新插入长度
    else {
        const size_type old_size =size();
        const size_type len =old_size +mystl::max(old_size, n);
        // 配置新的 vector 空间
        iterator new_start =data_allocator::allocate(len);
        iterator new_finish =new_start;
        try {
            // 首先将旧vector 的插入点之前的元素复制到新空间
            new_finish =mystl::uninitialize_copy(start, position, new_start);
            // 再将新增元素插入
            new_finish =mystl::uninitialize_fill_n(new_finish, n, value);
            // 最后将旧vector 的插入点之后的元素复制到新空间
            new_finish =mystl::uninitialize_copy(position, finish, new_finish);
        }
        catch(...) {
            // 销毁对象，释放内存
            mystl::destroy(new_start, new_finish);
            data_allocator::deallocate(new_start, len);
            throw;
        }
        // 对旧的vector 销毁对象并释放空间
        mystl::destroy(start, finish);
        data_allocator::deallocate(start, capacity());

        // 更新迭代器
        start =new_start;
        finish =new_finish;
        end_of_storage =new_start +len;
    }
}

// InputIterator 版本，在一个范围内插入
// 供insert(pos, first, last)调用
template<class T, class Alloc>
template <class InputIterator>
void vector<T, Alloc>::range_insert(iterator position, InputIterator first, InputIterator last, input_iterator_tag) {
    // 迭代器判断循环继续的条件，速度较慢
    for ( ; first !=last; ++first) {
        // 循环逐个在该位置进行单个值为 *first++的插入
        position =insert(position, *first);
        ++position;
    }
}

// ForwardIterator 版本，在一个范围内插入
// 供insert(pos, first, last)调用
template<class T, class Alloc>
template <class ForwardIterator>
void vector<T, Alloc>::range_insert(iterator position, ForwardIterator first, ForwardIterator last, forward_iterator_tag) {
    if (first ==last) return;
    size_type n =mystl::distance(first, last);
    // 备用空间 >= n（新增元素个数)，不需要扩展内存空间
    if (size_type(end_of_storage -finish) >=n) {
        // 计算插入点之后的现有元素个数
        const size_type elems_after =finish -position;
        iterator old_finish =finish;
        // 考虑到出现重叠现象
        // “插入点之后的现有元素个数” > “新增元素个数”
        if (elems_after >n) {
            mystl::uninitialize_copy(finish -n, finish, finish);
            finish +=n; // vector 尾端标记后移 
            mystl::copy_backward(position, old_finish -n, old_finish); //逆向复制
            mystl::copy(first, last, position);
        } 
        // “插入点之后的现有元素个数” <= “新增元素个数n”
        else {
            // 前移elems_after个位置
            ForwardIterator mid =first;
            mystl::advance(mid, elems_after);
            mystl::uninitialize_copy(mid, last, finish);
            finish +=(n -elems_after);
            mystl::uninitialize_copy(position, old_finish, finish);
            finish +=elems_after;
            mystl::copy(first, mid, position);
        }
    }
    // 备用空间 >= n（新增元素个数)，需要扩展内存空间
    // 首先确定新长度: 旧长度的两倍，或旧长度+ 新插入长度
    else {
        const size_type old_size =size();
        const size_type len =old_size +mystl::max(old_size, n);
        // 配置新的 vector 空间
        iterator new_start =data_allocator::allocate(len);
        iterator new_finish =new_start;
        try {
            // 首先将旧vector 的插入点之前的元素复制到新空间
            new_finish =mystl::uninitialize_copy(start, position, new_start);
            // 再将新增元素插入
            new_finish =mystl::uninitialize_copy(first, last, new_finish);
            // 最后将旧vector 的插入点之后的元素复制到新空间
            new_finish =mystl::uninitialize_copy(position, finish, new_finish);
        }
        catch (...) {
            // 销毁对象，释放内存
            mystl::destroy(new_start, new_finish);
            data_allocator::deallocate(new_start, len); 
            throw;
        }
        // 对旧的vector 销毁对象并释放空间
        mystl::destroy(start, finish);
        data_allocator::deallocate(start, capacity());

        // 更新迭代器
        start =new_start;
        finish =new_finish;
        end_of_storage =new_start +len;
    }
}

/**********************************************************************
 * 修改容器相关，增删改查操作，具体函数实现
 * 修改容器内部相关操作
 *********************************************************************/
// push_back() 函数实现
template<class T, class Alloc>
void vector<T, Alloc>::push_back(const value_type& value) {
    // 还有备用空间
    if (finish !=end_of_storage) {
        construct(finish, value);
        ++finish;
    }
    // 空间不够，构建新空间
    else {
        insert_aux(end(), value);
    }
}

// pop_back() 函数的实现
template<class T, class Alloc>
void vector<T, Alloc>::pop_back() {
    --finish; // 将尾端标记前移一格，表示将放弃尾端元素
    mystl::destroy(finish); // 将尾端元素析构掉
}

// erase(position) 清除固定位置的元素
template<class T, class Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator position) {
    if (position +1 !=end()) {        
        copy(position +1, finish, position); // 直接利用copy 将其覆盖掉        
    }
    --finish;
    mystl::destroy(finish); // 将尾端元素析构掉
    return position;
}

// erase(first, last) 清除区域上的元素
template<class T, class Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator first, iterator last) {
    iterator i =copy(last, finish, first);
    mystl::destroy(i, finish);
    finish =finish -(last -first);
    return first;
}

// insert(position, value) 固定位置插入一个元素
template<class T, class Alloc>
typename vector<T, Alloc>::iterator 
vector<T, Alloc>::insert(iterator position, const value_type& value) {
    size_type n =position -begin();
    // 相当于直接在vector尾部构造 value
    if (finish !=end_of_storage && position ==finish) {
        mystl::construct(finish, value);
        ++finish;
    }
    // 在中间某个位置插入
    else {
        insert_aux(position, value);
    }
    // 没有直接返回 position的原因是
    // 有可能经过一轮insert_aux，原来的vector被删除，新建一个
    // 导致迭代器失效
    return begin() +n;
}


// insert(position, n, value) 范围性插入
template<class T, class Alloc>
void  vector<T, Alloc>::insert(iterator position, size_type n, const value_type& value) {
    fill_insert(position, n, value);
}

// insert(position, first, last) 区域性插入
template<class T, class Alloc>
template<class InputIterator>
void vector<T, Alloc>::insert(iterator position, InputIterator first, InputIterator last) {
    range_insert(position, first, last, iterator_category(first));
}


/**********************************************************************
 * 修改vector容量相关
 * 修改尺寸大小
 *********************************************************************/
// 由于第二个参数已经确定，可以被缺省调用
// resize(size_type new_size) 同样适用
template<class T, class Alloc>
void vector<T, Alloc>::resize(size_type new_size, const value_type & value) {
    // 当前空间较大，后面多的erase 掉
    if (new_size <size()) {
        erase(begin +new_size, end());
    }
    // 当前空间较小，调用fill_insert()操作，扩大内存
    else {
        insert(end(), new_size -size(), value); // 插入新空间，扩大内存
    }
}

// 预留存储空间， 若n > capacity(),则重新分配更大一块内存。否则不做任何事
template<class T, class Alloc>
void vector<T, Alloc>::reserve(size_type n) {
    if (capacity() >=n) return;
    // 重新分配内存空间
    iterator new_start =data_allocator::allocate(n);
    iterator new_finish =new_start;
    try {
        // 将原vector copy过来
        new_finish =mystl::uninitialize_copy(start, finish, new_start);
    }
    catch (...) {
        // "commit or rollback"
        mystl::destroy(new_start, new_finish);
        data_allocator::deallocate(new_start, n);
        throw;
    }
    // 销毁原始对象vector，释放内存
    mystl::destroy(start, finish);
    data_allocator::deallocate(start, capacity());
    // 更新迭代器
    start =new_start;
    finish =new_finish;
    end_of_storage =new_start +n;
}

// 放弃capacity的容量，end_of_storage -finish的空间
// 缩小空间，直接破坏destroy掉超出空间部分的内容
template<class T, class Alloc>
void vector<T, Alloc>::shrink_to_fit() {
    if (finish ==end_of_storage) {
        return;
    }
    const size_type old_size =size();
    // 重新分配内存空间方式破坏找出size()部分的元素
    iterator new_start =data_allocator::allocate(old_size);
    iterator new_finish =new_start;
    try {
        // copy 原vector 到新内存这里
        new_finish =mystl::uninitialize_copy(start, finish, new_start);
    }
    catch (...) {
        // "commit or rollback"
        mystl::destroy(new_start, new_finish);
        data_allocator::deallocate(new_start, old_size);
        throw;
    }
    // 销毁原始对象vector，释放内存
    mystl::destroy(start, finish);
    data_allocator::deallocate(start, old_size);
    // 更新迭代器
    start =new_start;
    finish =new_finish;
    end_of_storage =new_finish;
}

} // end of namespace mystl


#endif