/**
 * @file mt_slist.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-05-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __MY_SLIST_H
#define __MY_SLIST_H

#include <cstdlib>
#include <cstddef>

#include "../allocator/my_allocator.h"
#include "../allocator/my_uninitialized.h"
#include "../algorithm/my_algorithm.h"
#include "../algorithm/my_help_function.h"
#include "../iterator/my_iterator.h"

namespace mystl {

// slist 的节点设计
struct slist_node_base {
    slist_node_base* next;
};

template <class T>
struct slist_node : public slist_node_base {
    T data; // 数据域

    slist_node() =default;
    slist_node(const T& val) : data(val) {}
};


// slist 的迭代器设计

struct slist_iterator_base {

    slist_node_base* node; // 指向节点基本结构
    slist_iterator_base(slist_node_base* x) : node(x) {}

    void incr() { node =node->next; }

    bool operator == (const slist_iterator_base& x) const { return node ==x.node; }
    bool operator != (const slist_iterator_base& x) const { return !(node ==x.node); }
};

template <class T>
struct slist_iterator : public slist_iterator_base {
    typedef slist_iterator<T>          self;

    typedef forward_iterator_tag       iterator_category;
    typedef T                          value_type;
    typedef T*                         pointer;
    typedef T&                         reference;
    typedef size_t                     size_type;
    typedef ptrdiff_t                  difference_type;

    typedef slist_iterator<T> iterator;
    typedef slist_node<T> list_node;

    slist_iterator(list_node* x) : slist_iterator_base(x) {}
    // 设计iterator_base的原因
    slist_iterator() : slist_iterator_base(nullptr) {}
    slist_iterator(const iterator& x) : slist_iterator_base(x.node) {}


    // 重载操作符
    
    reference operator*() const { return ((list_node*)node)->data; }
    pointer operator->() const { return &(operator*()); }

    self& operator++() {  
        incr();
        return *this;
    }
    self operator++(int) {  
        self tmp =*this;
        incr();
        return tmp;
    }
    // 不需要重载operator--，对应的是forward iterator

};

// slist const 版本设计
template <class T>
struct slist_const_iterator : public slist_iterator_base {
    typedef slist_const_iterator<T>           self;

    typedef forward_iterator_tag       iterator_category;
    typedef T                          value_type;
    typedef T*                         pointer;
    typedef T&                         reference;
    typedef size_t                     size_type;
    typedef ptrdiff_t                  difference_type;

    typedef slist_const_iterator<T> iterator;
    typedef slist_node<T> list_node;

    slist_const_iterator(list_node* x) : slist_iterator_base(x) {}
    // 设计iterator_base的原因
    slist_const_iterator() : slist_iterator_base(nullptr) {}
    slist_const_iterator(const iterator& x) : slist_iterator_base(x.node) {}

    // 重载操作符
    bool operator == (const slist_iterator_base& x) const { return node ==x.node; }
    bool operator != (const slist_iterator_base& x) const { return !(node ==x.node); }
    reference operator*() const { return ((list_node*)node)->data; }
    pointer operator->() const { return &(operator*()); }

    self& operator++() {  
        incr();
        return *this;
    }
    self operator++(int) {  
        self tmp =*this;
        incr();;
        return tmp;
    }
    // 不需要重载operator--，对应的是forward iterator
};


// slist class 的设计
template <class T, class Alloc =mystl::allocator<T>>
class slist {
public:
    // 指定迭代器类型
    typedef slist_node_base             list_node_base;
    typedef slist_node<T>               list_node; // 节点类型
    typedef slist_node<T>*              link_type; // 节点指针
    typedef slist_iterator<T>           iterator;  // 链表迭代器类型
    typedef slist_const_iterator<T>     const_iterator; // const迭代器类型

    // 内存分配
    typedef Alloc  data_allocator;
    // 必须有一个针对node的allocator
    typedef mystl::allocator<list_node> node_allocator; 

    typedef typename data_allocator::value_type         value_type;
    typedef typename data_allocator::pointer            pointer;
    typedef typename data_allocator::const_pointer      const_pointer;
    typedef typename data_allocator::reference          reference;
    typedef typename data_allocator::const_reference    const_reference;
    typedef typename data_allocator::size_type          size_type;
    typedef typename data_allocator::difference_type    difference_type;

protected:
    link_type       head_; // 头部，对应的是一个指针
    size_type       size_; // 链表大小

public:
    // 构造、复制、移动、析构函数
    // 构造函数
    slist() {
        fill_init(0, value_type());
    }
    explicit slist(size_type n) {
        fill_init(n, value_type());
    }
    slist(size_type n, const T& value) {
        fill_init(n, value);
    }

    // 复制构造
    slist(const slist& rhs) {
        copy_init(rhs.begin(), rhs.end());
    } 
    slist(slist&& rhs) : head_(rhs.head_), size_(rhs.size_) {
        rhs.head_ =nullptr;
        rhs.size_ =0;
    }

    // 析构函数
    ~slist() {
        clear();
    }
    

private:
    // 内部使用辅助函数
    // create/ destroy 节点
    link_type get_node();
    void put_node(link_type p);
    link_type creat_node(const T& value); 
    void destroy_node(link_type p);

    // initialize 相关
    void empty_init();
    void fill_init(size_type n, const value_type& value);
    template<class Iter>
    void copy_init(Iter first, Iter last);

    // list link 相关 前闭后闭区间[first, last]
    iterator link_iter_node(const_iterator pos, link_type new_node);
    void link_nodes(link_type pos, link_type first, link_type last);
    void link_nodes_at_front(link_type first, link_type last);
    void link_nodes_at_back(link_type first, link_type last);
    void unlink_nodes(link_type first, link_type last);


public:
    // 迭代器相关操作    
    iterator begin() noexcept { return (list_node*)head_->next; }
    iterator end() noexcept { return iterator(0); }
    const_iterator begin() const noexcept { return (list_node*)head_->next; }
    const_iterator end() const noexcept { return const_iterator(0); }
    
    const_iterator cbegin() const noexcept { return (list_node*)head_->next; }
    const_iterator cend() const noexcept { return const_iterator(0); }

    // 容量相关操作
    bool empty() const noexcept { return head_->next ==0; }
    size_type size() const noexcept { return size_; }
    size_type max_size() const noexcept { return static_cast<size_type>(-1); }

    // 访问元素相关操作
    reference front() {
        if (!empty()) 
            return *begin();
        else 
            throw "slist front error!";
    }
    void log_slist() {
        if (size_ ==0) {
            std::cout << "slist is empty" << std::endl;
            return;
        }
        iterator ite;
        std::cout << "[";
        for (ite =begin(); ite !=end(); ++ite) {
            std::cout << *ite << ' ';
        }
        std::cout << "]" << std::endl;
    }

    // 修改容器相关，增删改查操作
    // push_back/front, pop_back/front
    void push_front(const value_type& vlaue);
    void pop_front();

    // insert 操作, 在pos 处插入value值的元素
    iterator insert(const_iterator pos, const value_type& value);

    // reverse 操作
    void reverse();

    // merge 操作
    void merge(slist& x) {
        merge(x, mystl::less<T>() );
    }
    template<class Compare>
    void merge(slist& x, Compare commp);

    // sort 操作 
    void merge_sort() {
        slist_merge_sort(begin(), end(), size(), mystl::less<T>());
    }  
    template<class Compared>
    void merge_sort(Compared comp) {
        slist_merge_sort(begin(), end(), size(), comp);
    } 
    template<class Compared>
    iterator slist_merge_sort(iterator frist, iterator last, size_type n, Compared comp);


    // clear 操作
    void clear();


}; // end of class slist

/**********************************************************************
 * 修改容器相关，增删改查操作
 * insert 操作, 在pos 处后面一个位置插入value值的元素
 * 1-->3，在1 的位置插入2，结果1-->2-->3 
 *********************************************************************/
template<class T, class Alloc>
typename slist<T, Alloc>::iterator 
slist<T, Alloc>::insert(const_iterator pos, const value_type& value) {
    if (size_ > max_size() -1) {
        throw "List insert error!";
    } 
    link_type new_node =creat_node(value);
    ++size_;

    // 连接该节点
    new_node->next =pos.node->next;
    pos.node->next =new_node;

    return iterator(new_node);
}

/**********************************************************************
 * 修改容器相关，增删改查操作
 * merge 节点
 * 合并链表到 该链表上面
 *********************************************************************/
template<class T, class Alloc>
template<class Compare>
void slist<T, Alloc>::merge(slist& x, Compare comp) {
    if (this ==&x) return;
    if (size_ > max_size() -x.size_) {
        throw "List splice error!";
    }

    iterator f1 =begin();
    iterator l1 =end();
    iterator f2 =x.begin();
    iterator l2 =x.end();
    auto cur =(link_type)head_;

    while (f1 !=l1 && f2 !=l2) {
        if (comp(*f2, *f1)) {
            cur->next =(link_type)f2.node;
            ++f2;
        }
        else {
            cur->next =(link_type)f1.node;
            ++f1;
        }
        cur =(link_type)cur->next;
    }
    if (f2 !=l2) {
        cur->next =(link_type)f2.node;
    }
    size_ +=x.size_;
    x.head_ =nullptr;
    x.size_ =0;
    x.clear();
}

/**********************************************************************
 * 修改容器相关，增删改查操作
 * sort 节点 
 *********************************************************************/
template<class T, class Alloc>
template<class Compared>
typename slist<T, Alloc>::iterator 
slist<T, Alloc>::slist_merge_sort(iterator f1, iterator l2, size_type n, Compared comp) {
    // 暂定，归并排序，和list同理
}


/**********************************************************************
 * 修改容器相关，增删改查操作
 * reverse 节点 
 *********************************************************************/
template<class T, class Alloc>
void slist<T, Alloc>::reverse() {
    // 节点个数为 0 or 1，直接返回
    if (head_->next ==nullptr || head_->next->next ==nullptr) return;

    link_type cur =(link_type)begin().node;;
    link_type pre =nullptr;

    while (cur) {
        auto next =(link_type)cur->next;
        cur->next =pre;
        pre =cur;
        cur =next;
    }
    head_->next =pre;

}


/**********************************************************************
 * 修改容器相关，增删改查操作
 * push_back/front, pop_back/front 节点
 *********************************************************************/
template<class T, class Alloc>
void slist<T, Alloc>::push_front(const value_type& value) {
    if (size_ > max_size() -1) {
        throw "slist push_front error!";
    } 
    link_type new_node =creat_node(value);
    link_nodes_at_front(new_node, new_node);
    ++size_;
}

template<class T, class Alloc>
void slist<T, Alloc>::pop_front() {
    if (empty()) {
        throw "slist pop_front error!";
    }
    link_type n =(link_type)head_->next;
    head_->next =head_->next->next;
    destroy_node(n);
    --size_;
}

/**********************************************************************
 * 修改容器相关，增删改查操作
 * clear 节点
 *********************************************************************/
// clear 
template<class T, class Alloc>
void slist<T, Alloc>::clear() {
    if (size_ ==0) return ;
    auto cur =(link_type)head_->next;
    while (cur) {
        link_type next =(link_type)cur->next;
        destroy_node(cur);
        cur =next;
    }
    // 应该更新 node_为unlink的状态
    head_->next =nullptr;
    size_ =0;
}

/**********************************************************************
 * 内部使用辅助函数
 * initialize list 相关
 *********************************************************************/
// fill 初始化
template<class T, class Alloc>
void slist<T, Alloc>::fill_init(size_type n, const value_type& value) {
    head_ =get_node(); 
    head_->next =nullptr;
    size_ =n;

    while (n--) {
        auto node =creat_node(value);
        link_nodes_at_front(node, node);
    }
}

// copy 初始化 [first, last)
template<class T, class Alloc>
template<class Iter>
void slist<T, Alloc>::copy_init(Iter first, Iter last) {
    head_ =get_node();
    head_->next =nullptr;
    size_type n =mystl::distance(first, last);
    // size_type n =static_cast<size_type>(last -first);
    size_ =n;

    while (n--) {
        auto node =creat_node(*first++);
        link_nodes_at_front(node, node);
    }
}

/**********************************************************************
 * 内部使用辅助函数
 * list link 相关 前闭后闭区间[first, last]
 *********************************************************************/

// 在头部接入 [first, last] 的节点
template<class T, class Alloc>
void slist<T, Alloc>::link_nodes_at_front(link_type first, link_type last) {
    last->next =head_->next;
    head_->next =first;
}

// slist 不提供在尾部插入元素版本





/**********************************************************************
 * 内部使用辅助函数
 * create/ destroy 节点
 *********************************************************************/
// create/ destroy 节点
template<class T, class Alloc>
typename slist<T, Alloc>::link_type
slist<T, Alloc>::get_node() {
    // 返回类型加 强制转换
    return (link_type)node_allocator::allocate(1);
}

template<class T, class Alloc>
typename slist<T, Alloc>::link_type
slist<T, Alloc>::creat_node(const T& value) {
    link_type p =get_node();
    mystl::construct(&p->data, value);
    return p;
}

template<class T, class Alloc>
void slist<T, Alloc>::put_node(link_type p) {
    node_allocator::deallocate(p);
}

template<class T, class Alloc>
void slist<T, Alloc>::destroy_node(link_type p) {
    data_allocator::destroy(&(p->data));
    put_node(p);
}


/**********************************************************************
 * 操作符重载
 *********************************************************************/
template <class T>
bool operator==(const slist<T>& lhs, const slist<T>& rhs) {
    auto f1 =lhs.cbegin();
    auto l1 =lhs.cend();
    auto f2 =rhs.cbegin();
    auto l2 =rhs.cend();
    while (f1 !=l1 && f2 !=l2 && *f1 ==*f2) {
        ++f1;
        ++f2;
    }
    return (f1 ==l1 && f2 ==l2);
}

template <class T>
bool operator!=(const slist<T>& lhs, const slist<T>& rhs) {
    return !(lhs ==rhs);
}


template <class T>
bool operator<(const slist<T>& lhs, const slist<T>& rhs) {
    return mystl::list_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
}

// non-const 版本
template <class T>
bool operator<(slist<T>& lhs, slist<T>& rhs) {
    return mystl::list_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
}


} // end of namespace mystl

#endif