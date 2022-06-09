/**
 * @file my_queue.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-05-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __MY_QUEUE_H
#define __MY_QUEUE_H

#include "../container/my_list.h"

namespace mystl {

template <class T, class Container = mystl::list<T>>
class queue {
public:
    typedef Container                       container_type;
    typedef typename Container::value_type           value_type;
    typedef typename Container::size_type            size_type;
    typedef typename Container::reference            reference;
    typedef typename Container::const_reference      const_reference;

protected:
    container_type c_; // 底层容器实现stack

public:
    // 构造，拷贝构造，析构
    queue() =default;
    explicit queue(size_type n) : c_(n) {}
    queue(size_type n, const value_type& value) : c_(n, value) {}
    // 拷贝构造
    queue(mystl::list<T> ilist) : c_(ilist.begin(), ilist.end()) {}
    queue(const queue& rhs) : c_(rhs.c_) {}

    ~queue() =default;

    // 重载操作符
    queue& operator =(const queue& rhs) {
        c_ =rhs.c_;
        return *this;
    }
    // 重载的时候，两个参数不应该定义在class里面，应该使用一个this指针处理，^-^
    // friend 友元声明表示可以访问类的内部成员
    friend bool operator== (const queue& lhs, const queue& rhs) { return lhs.c_ ==rhs.c_; }
    friend bool operator < (const queue& lhs, const queue& rhs) { return lhs.c_ < rhs.c_; } 

    
    
    // 访问元素相关操作
    reference front() { return c_.front(); }
    const_reference front() const { return c_.front(); }
    reference back() { return c_.back(); }
    const_reference back() const { return c_.back(); }

    // 容量相关操作
    bool empty() const noexcept { return c_.empty(); }
    size_type size() const noexcept { return c_.size(); }

    // 修改容器相关操作
    void push(const value_type& value) { c_.push_back(value); }
    void pop() { c_.pop_front(); }

    void clear() {
        while (!empty()) {
            pop();
        }
    }

    // 打印stack元素,方便调试
    void log_queue() { c_.log_list(); }

};

template <class T, class Container>
bool operator== (const queue<T, Container>& lhs, const queue<T, Container>& rhs) { 
    return lhs ==rhs; 
}

template <class T, class Container>
bool operator!= (const queue<T, Container>& lhs, const queue<T, Container>& rhs) { 
    return !(lhs ==rhs); 
}

template <class T, class Container>
bool operator < (const queue<T, Container>& lhs, const queue<T, Container>& rhs) { 
    return (lhs <rhs); 
}

template <class T, class Container>
bool operator > (const queue<T, Container>& lhs, const queue<T, Container>& rhs) { 
    return (rhs <lhs); 
}

} // end of namespace mystl


#endif