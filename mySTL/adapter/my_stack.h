/**
 * @file my_stack.h
 * @author your name (you@domain.com)
 * @brief adapter 适配器
 * @version 0.1
 * @date 2022-05-27
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __MY_STACK_H
#define __MY_STACK_H

#include "../container/my_list.h"

namespace mystl {

template <class T, class Container = mystl::list<T>>
class stack {
public:
    typedef Container                       container_type;
    typedef typename Container::value_type           value_type;
    typedef typename Container::size_type            size_type;
    typedef typename Container::reference            reference;
    typedef typename Container::const_reference      const_reference;

protected:
    container_type c_; // 底层容器实现stack

private:

public:
    // 构造，拷贝构造，析构
    stack() =default;
    explicit stack(size_type n) : c_(n) {}
    stack(size_type n, const value_type& value) : c_(n, value) {}
    // 拷贝构造
    stack(mystl::list<T> ilist) : c_(ilist.begin(), ilist.end()) {}
    stack(const stack& rhs) : c_(rhs.c_) {}

    ~stack() =default;

    // 重载操作符
    stack& operator =(const stack& rhs) {
        c_ =rhs.c_;
        return *this;
    }
    // 重载的时候，两个参数不应该定义在class里面，应该使用一个this指针处理，^-^
    // friend 友元声明表示可以访问类的内部成员
    friend bool operator== (const stack& lhs, const stack& rhs) { return lhs.c_ ==rhs.c_; }
    friend bool operator < (const stack& lhs, const stack& rhs) { return lhs.c_ < rhs.c_; } 

    
    
    // 访问元素相关操作
    reference top() { return c_.back(); }
    const_reference top() const { return c_.back(); }

    // 容量相关操作
    bool empty() const noexcept { return c_.empty(); }
    size_type size() const noexcept { return c_.size(); }

    // 修改容器相关操作
    void push(const value_type& value) { c_.push_back(value); }
    void pop() { c_.pop_back(); }

    void clear() {
        while (!empty()) {
            pop();
        }
    }

    // 打印stack元素,方便调试
    void log_stack() { c_.log_list(); }

}; // end of class stack

template <class T, class Container>
bool operator== (const stack<T, Container>& lhs, const stack<T, Container>& rhs) { 
    return lhs ==rhs; 
}

template <class T, class Container>
bool operator!= (const stack<T, Container>& lhs, const stack<T, Container>& rhs) { 
    return !(lhs ==rhs); 
}

template <class T, class Container>
bool operator < (const stack<T, Container>& lhs, const stack<T, Container>& rhs) { 
    return (lhs <rhs); 
}

template <class T, class Container>
bool operator > (const stack<T, Container>& lhs, const stack<T, Container>& rhs) { 
    return (rhs <lhs); 
}

}; // end of namespace mystl

#endif