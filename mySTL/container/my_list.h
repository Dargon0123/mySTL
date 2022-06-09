/**
 * @file my_list.h
 * @author your name (you@domain.com)
 * @brief container list 双向链表
 * @version 0.1
 * @date 2022-05-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __MY_LIST_H
#define __MY_LIST_H

// 标准库文件
#include <cstdlib>
#include <cstddef>

#include "../allocator/my_allocator.h"
#include "../allocator/my_uninitialized.h"
#include "../algorithm/my_algorithm.h"
#include "../algorithm/my_help_function.h"
#include "../iterator/my_iterator.h"

namespace mystl {

/**********************************************************************
 * list 的节点设计
 *********************************************************************/
template <class T>
struct __list_node {
    __list_node<T>* prev; // 前一节点
    __list_node<T>* next; // 后一节点

    T data; // 节点值
};

/**********************************************************************
 * list 的迭代器
 *********************************************************************/
template <class T>
struct __list_iterator : public
mystl::iterator<mystl::bidirection_iterator_tag, T> {
    typedef __list_iterator<T>           self;
    // typedef __list_iterator<T>           iterator;


    typedef bidirection_iterator_tag   iterator_category;
    typedef T                          value_type;
    typedef T*                         pointer;
    typedef T&                         reference;
    typedef __list_node<T>*            link_type;
    typedef size_t                     size_type;
    typedef ptrdiff_t                  difference_type;

    link_type node; // 迭代器内部一个普通指针，指向list的节点

    // ctor
    __list_iterator() =default;
    // __list_iterator() {}
    __list_iterator(link_type x) : node(x) {}
    __list_iterator(const __list_iterator& x) : node(x.node) {}

    // 重载操作符

    // 重载比较操作符
    bool operator ==(const self& x) const { return node ==x.node; }
    bool operator !=(const self& x) const { return node !=x.node; }
    // 重载指针操作符
    reference operator*() const { return (*node).data; }
    pointer operator->() const { return &(operator*()); }
    // 重载累加操作符
    // ++i
    self& operator++() {
        node =(link_type)(node->next);
        return *this;
    }
    // i++
    self operator++(int) {
        self tmp =*this;
        ++*this;
        return tmp;
    }
    // --i
    self& operator--() {
        node =(link_type)(node->prev);
        return *this;
    }
    // i--
    self operator--(int) {
        self tmp =*this;
        --*this;
        return tmp;
    }
     
};
// const 类型的iterator
template <class T>
struct __list_const_iterator : public
mystl::iterator<mystl::bidirection_iterator_tag, T> {
    typedef __list_const_iterator<T>           self;
    // typedef __list_iterator<T>           iterator;


    // typedef bidirection_iterator_tag   iterator_category;
    typedef T                          value_type;
    typedef T*                         pointer;
    typedef T&                         reference;
    typedef __list_node<T>*            link_type;
    typedef size_t                     size_type;
    typedef ptrdiff_t                  difference_type;

    link_type node; // 迭代器内部一个普通指针，指向list的节点

    // ctor
    __list_const_iterator() =default;
    // __list_iterator() {}
    __list_const_iterator(link_type x) : node(x) {}
    __list_const_iterator(const __list_const_iterator& x) : node(x.node) {}

    // 重载操作符
    
    // 重载比较操作符
    bool operator ==(const self& x) const { return node ==x.node; }
    bool operator !=(const self& x) const { return node !=x.node; }
    // 重载指针操作符
    reference operator*() const { return (*node).data; }
    pointer operator->() const { return &(operator*()); }
    // 重载累加操作符
    // ++i
    self& operator++() {
        node =(link_type)(node->next);
        return *this;
    }
    // i++
    self operator++(int) {
        self tmp =*this;
        ++*this;
        return tmp;
    }
    // --i
    self& operator--() {
        node =(link_type)(node->prev);
        return *this;
    }
    // i--
    self operator--(int) {
        self tmp =*this;
        --*this;
        return tmp;
    }
     
};

template <class T, class Alloc =allocator<T>>
class list {
public:

    // 指定迭代器类型
    typedef __list_node<T>           list_node; // 节点类型
    typedef __list_node<T>*          link_type; // 节点指针
    typedef __list_iterator<T>       iterator;  // 链表迭代器类型
    typedef __list_const_iterator<T> const_iterator; // const迭代器类型

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
    

private:
    link_type node_; // 指向末尾节点
    size_type size_; // 链表大小

public:
    // 构造、复制、移动、析构函数
    // 构造函数
    list() {
        fill_init(0, value_type());
    }
    explicit list(size_type n) {
        fill_init(n, value_type());
    }
    list(size_type n, const T& value) {
        fill_init(n, value);
    }

    // 复制构造 
    // 这个构造出错，是和上面的初始化参数冲突了      
    // template<class Iter>
    // list(Iter first, Iter last) {
    //     copy_init(first, last);
    // }
    list(std::initializer_list<T> ilist) {
        copy_init(ilist.begin(), ilist.end());
    }
    list(const list& rhs) {
        copy_init(rhs.begin(), rhs.end());
    } 
    list(list&& rhs) : node_(rhs.node_), size_(rhs.size_) {
        rhs.node_ =nullptr;
        rhs.size_ =0;
    }

    // 析构函数
    ~list() {
        if (node_) {
            clear();
            destroy_node(node_);
            node_ =nullptr;
            size_ =0;
        }
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
    iterator begin() noexcept { return node_->next; }
    iterator end() noexcept { return node_; }
    const_iterator begin() const noexcept { return node_->next; }
    const_iterator end() const noexcept { return node_; }
    
    const_iterator cbegin() const noexcept { return node_->next; }
    const_iterator cend() const noexcept { return node_; }

    // 容量相关操作
    bool empty() const noexcept { return node_->next ==node_; }
    size_type size() const noexcept { return size_; }
    size_type max_size() const noexcept { return static_cast<size_type>(-1); }


    // 访问元素相关操作
    reference front() {
        if (!empty()) 
            return *begin();
        else 
            throw "List front error!";
    }
    reference back() {
        if (!empty()) 
            return *(--end());
        else 
            throw "List back error!";
    }
    void log_list() {
        if (size_ ==0) {
            std::cout << "list is empty" << std::endl;
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
    // insert 操作, 在pos 处插入value值的元素
    iterator insert(const_iterator pos, const value_type& value);

    // 针对list的splice/结合操作
    void splice(const_iterator pos, list& x);
    void splice(const_iterator pos, list& x, const_iterator ite);
    void splice(const_iterator pos, list& x, const_iterator first, const_iterator last); 

    // remove 节点操作
    void remove(const value_type& value) {
        // 使用lambda，隐式的引用捕获方式
        remove_if( [&](const value_type& v) {return v ==value;} );
    }
    template <class UnaryPredicate>
    void remove_if(UnaryPredicate pred);

    // unique 操作
    void unique();
    void unique_v2() {
        unique_if(mystl::equal_to<T>() );
    }
    template <class BinaryPredicate>
    void unique_if(BinaryPredicate pred);

    // merge 操作
    void merge(list& x) {
        merge(x, mystl::less<T>() );
    }
    template<class Compare>
    void merge(list& x, Compare commp);

    // sort 操作 
    void sort() {
        merge_sort();
    }
    void merge_sort() {
        list_merge_sort(begin(), end(), size(), mystl::less<T>());
    }  
    template<class Compared>
    void merge_sort(Compared comp) {
        list_merge_sort(begin(), end(), size(), comp);
    } 
    template<class Compared>
    iterator list_merge_sort(iterator frist, iterator last, size_type n, Compared comp);

    // reverse 操作
    void reverse();

    // push_back/front, pop_back/front
    void push_front(const value_type& vlaue);
    void push_back(const value_type& vlaue);
    void pop_front();
    void pop_back();

    // clear and erase操作
    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);
    iterator erase(iterator pos);
    iterator erase(iterator first, iterator last);
    void clear();


};



/**********************************************************************
 * 修改容器相关，增删改查操作
 * insert 操作, 在pos 处插入value值的元素
 *********************************************************************/
template<class T, class Alloc>
typename list<T, Alloc>::iterator 
list<T, Alloc>::insert(const_iterator pos, const value_type& value) {
    if (size_ > max_size() -1) {
        throw "List insert error!";
    } 
    link_type new_node =creat_node(value);
    ++size_;
    return link_iter_node(pos, new_node);
}


/**********************************************************************
 * 修改容器相关，增删改查操作
 * splice 操作, 在pos 处插入value值的元素
 *********************************************************************/
// 将list x 结合于所指位置pos 之前
template<class T, class Alloc>
void list<T, Alloc>::splice(const_iterator pos, list& x) {
    if (x.empty()) return;
    if (this ==&x) {
        return;
    }
    if (size_ > max_size() -1) {
        throw "List splice error!";
    }
    link_type new_head =x.node_->next;
    link_type new_tail =x.node_->prev;

    x.unlink_nodes(new_head, new_tail);
    link_nodes(pos.node, new_head, new_tail);

    size_ +=x.size_;
    x.size_ =0;
}
// 将list x 中的节点ite，结合于所指位置pos 之前
template<class T, class Alloc>
void list<T, Alloc>::splice(const_iterator pos, list& x, const_iterator ite) {
    if (pos.node != ite.node) {
        if (size_ > max_size() -1) {
            throw "List splice error!";
        }
        link_type new_node =ite.node;
        x.unlink_nodes(new_node, new_node);
        link_nodes(pos.node, new_node, new_node);

        ++size_;
        --x.size_;
    }
}
// 将list x 中的节点[first, last)范围内节点，结合于所指位置pos 之前
template<class T, class Alloc>
void list<T, Alloc>::splice(const_iterator pos, list& x, const_iterator first, const_iterator last) {
    if (first !=last) {
        size_type len =mystl::distance(first, last);
        if (size_ > max_size() -len) {
            throw "List splice error!";
        }
        link_type new_head =first.node;
        link_type new_tail =last.node->prev;

        x.unlink_nodes(new_head, new_tail);
        link_nodes(pos.node, new_head, new_tail);

        size_ +=len;
        x.size_ -=len;
    }
}

/**********************************************************************
 * 修改容器相关，增删改查操作
 * remove 节点, 将令一元操作为 true的节点删除掉，lambda表达式
 *********************************************************************/
template<class T, class Alloc>
template <class UnaryPredicate>
void list<T, Alloc>::remove_if(UnaryPredicate pred) {
    const_iterator first =cbegin();
    const_iterator last =cend();
    const_iterator cur;
    while (first !=last) {
        cur =first;
        ++first;
        if (pred(*cur)) {
            erase(cur);
        }
    }
}

/**********************************************************************
 * 修改容器相关，增删改查操作
 * unique 节点, 移除数值相同的连续元素，只剩一个元素
 * 例如 [1 1 0 2 2]， unique之后 [1 0 2]
 *********************************************************************/
template<class T, class Alloc>
void list<T, Alloc>::unique() {
    auto first =begin();
    auto last =end();
    if (first ==last) return;
    auto cur =first;
    while (++cur !=last)
    {
        if (*first ==*cur) {
            erase(cur);
        }
        else {
            first =cur;
        }
        cur =first;
    }
    
}
// 使用二元操作符处理
template<class T, class Alloc>
template <class BinaryPredicate>
void list<T, Alloc>::unique_if(BinaryPredicate pred) {
    auto first =begin();
    auto last =end();
    if (first ==last) return;
    auto cur =first;
    while (++cur !=last)
    {
        if (pred(*cur, *first)) {
            erase(cur);
        }
        else {
            first =cur;
        }
        cur =first;
    }
}

/**********************************************************************
 * 修改容器相关，增删改查操作
 * merge 节点
 * 合并链表到 该链表上面
 *********************************************************************/
template<class T, class Alloc>
template<class Compare>
void list<T, Alloc>::merge(list& x, Compare comp) {
    if (this ==&x) return;
    if (size_ > max_size() -x.size_) {
        throw "List splice error!";
    }
    iterator f1 =begin();
    iterator l1 =end();
    iterator f2 =x.begin();
    iterator l2 =x.end();

    while (f1 !=l1 && f2 !=l2) {
        if (comp(*f2, *f1)) {
            // f2 <f1 的一段区间
            iterator next =f2;
            ++next;
            while (next !=l2 && comp(*next, *f1) ) {
                ++next; // next 后移直到不满足条件
            }
            auto f =f2.node;
            auto l =next.node->prev;
            f2 =next;

            // 将x 的一段区间 合并起来
            x.unlink_nodes(f, l);
            link_nodes(f1.node, f, l);
            ++f1;
        }
        else {
            ++f1;
        }        
    }
    // 剩余部分
    if (f2 !=l2) {
        auto f =f2.node;
        auto l =l2.node->prev;
        x.unlink_nodes(f, l);
        link_nodes(l1.node, f, l);
    }
    // 对应处理尺寸问题
    size_ +=x.size_;
    x.size_ =0;
}
/**********************************************************************
 * 修改容器相关，增删改查操作
 * sort 节点 
 *********************************************************************/
// merge sort 
// 对应前闭后开区间[f1 l1), [l1/f2, l2)
template<class T, class Alloc>
template<class Compared>
typename list<T, Alloc>::iterator 
list<T, Alloc>::list_merge_sort(iterator f1, iterator l2, size_type n, Compared comp) {
    // 节点数 <= 1
    if (n <2) return f1;

    // 仅有两个节点
    if (n ==2) {
        if (comp(*--l2, *f1)) {
            auto last =l2.node;
            unlink_nodes(last, last);
            link_nodes(f1.node, last, last);
            return l2;
        }
        return f1;
    }
    size_type n2 =n /2; // 中间分隔位置
    auto l1 =f1;
    mystl::advance(l1, n2); // 移动list1 到中间位置

    auto result = f1 =list_merge_sort(f1, l1, n2, comp); // 前半段排序
    auto f2 =l1 =list_merge_sort(l1, l2, n -n2, comp); // 后半段排序

    // 先开始移动一小段,
    // 每次迭代 将result 返回值更新为最小的节点
    // [1 4 5 2 3] --> [1 4 2 3 5]
    if (comp(*f2, *f1)) {
        auto next =f2;
        ++next;
        while (next !=l2 && comp(*next, *f1) ) {
            ++next; // next 后移直到不满足条件
        }
        auto f =f2.node;
        auto l =next.node->prev;
        result =f2;
        l1 =f2 =next;
        unlink_nodes(f, l);
        next =f1;
        ++next;
        link_nodes(f1.node, f, l);
        f1 =next;
    }
    else {
        ++f1;
    }

    // 合并两段有序区间
    while (f1 !=l1 && f2 !=l2) {
        if (comp(*f2, *f1)) {
            auto next =f2;
            ++next;
            while (next !=l2 && comp(*next, *f1)) {
                ++next;
            }
            auto f =f2.node;
            auto l =next.node->prev;
            if (l1 ==f2) {
                l1 =next;
            }
            f2 =next;
            unlink_nodes(f, l);
            next =f1;
            ++next;
            link_nodes(f1.node, f, l);
            f1 =next;
        }
        else {
            ++f1;
        }
    }

    return result;

}

/**********************************************************************
 * 修改容器相关，增删改查操作
 * reverse 节点 
 *********************************************************************/
template<class T, class Alloc>
void list<T, Alloc>::reverse() {
    // 节点个数为 0 or 1，直接返回
    if (node_->next ==node_ || node_->next->next ==node_) { // if 条件错误一次
        return;
    }
    iterator first =begin();
    while (first !=end()) {
        iterator cur =first;
        ++first;
        // 将[cur, first)节点插入到最新的begin()位置前面
        iterator pos =begin();
        first.node->prev->next =pos.node;
        cur.node->prev->next =first.node;
        pos.node->prev->next =cur.node;
        link_type tmp =pos.node->prev;
        pos.node->prev =first.node->prev;
        first.node->prev =cur.node->prev;
        cur.node->prev =tmp;
    }
}


/**********************************************************************
 * 修改容器相关，增删改查操作
 * push_back/front, pop_back/front 节点
 *********************************************************************/
template<class T, class Alloc>
void list<T, Alloc>::push_front(const value_type& value) {
    if (size_ > max_size() -1) {
        throw "List push_front error!";
    } 
    link_type new_node =creat_node(value);
    link_nodes_at_front(new_node, new_node);
    ++size_;
}
template<class T, class Alloc>
void list<T, Alloc>::push_back(const value_type& value) {
    if (size_ > max_size() -1) {
        throw "List push_back error!";
    } 
    link_type new_node =creat_node(value);
    link_nodes_at_back(new_node, new_node);
    ++size_;
}

template<class T, class Alloc>
void list<T, Alloc>::pop_front() {
    if (empty()) {
        throw "List pop_front error!";
    }
    link_type n =node_->next;
    unlink_nodes(n, n);
    destroy_node(n);
    --size_;
}
template<class T, class Alloc>
void list<T, Alloc>::pop_back() {
    if (empty()) {
        throw "List pop_back error!";
    }
    link_type n =node_->prev;
    unlink_nodes(n, n);
    destroy_node(n);
    --size_;
}

/**********************************************************************
 * 修改容器相关，增删改查操作
 * create/ destroy 节点
 *********************************************************************/
// const 版本
// erase操作
template<class T, class Alloc>
typename list<T, Alloc>::iterator 
list<T, Alloc>::erase(const_iterator pos) {
    if (pos ==cend()) {
        throw "List erase error!";
    }
    link_type n =pos.node; // 提取iterator里的节点指针
    link_type next =n->next;
    unlink_nodes(n, n);
    destroy_node(n);
    --size_;
    // 返回一个迭代器类型
    return iterator(next);
}
// erase [first, last)节点元素
template<class T, class Alloc>
typename list<T, Alloc>::iterator 
list<T, Alloc>::erase(const_iterator first, const_iterator last) {
    if (first !=last) {
        unlink_nodes(first.node, first.node);
        while (first !=last) {
            link_type cur =first.node;
            ++first;
            destroy_node(cur);            
            --size_;
        }
    }
    return iterator(last.node);
}
// non-const 版本
// erase操作
template<class T, class Alloc>
typename list<T, Alloc>::iterator 
list<T, Alloc>::erase(iterator pos) {
    if (pos ==end()) {
        throw "List erase error!";
    }
    link_type n =pos.node; // 提取iterator里的节点指针
    link_type next =n->next;
    unlink_nodes(n, n);
    destroy_node(n);
    --size_;
    // 返回一个迭代器类型
    return iterator(next);
}
// erase [first, last)节点元素
template<class T, class Alloc>
typename list<T, Alloc>::iterator 
list<T, Alloc>::erase(iterator first, iterator last) {
    if (first !=last) {
        unlink_nodes(first.node, first.node);
        while (first !=last) {
            link_type cur =first.node;
            ++first;
            destroy_node(cur);            
            --size_;
        }
    }
    return iterator(last.node);
}


// clear 
template<class T, class Alloc>
void list<T, Alloc>::clear() {
    if (size_ ==0) return ;
    auto cur =node_->next;
    while (cur !=node_) {
        link_type next =cur->next;
        destroy_node(cur);
        cur =next;
    }
    // 应该更新 node_为unlink的状态
    node_->prev =node_->next =nullptr;
    size_ =0;
}


/**********************************************************************
 * 内部使用辅助函数
 * create/ destroy 节点
 *********************************************************************/
// create/ destroy 节点
template<class T, class Alloc>
typename list<T, Alloc>::link_type
list<T, Alloc>::get_node() {
    // 返回类型加 强制转换
    return (link_type)node_allocator::allocate(1);
}
template<class T, class Alloc>
void list<T, Alloc>::put_node(link_type p) {
    node_allocator::deallocate(p);
}

template<class T, class Alloc>
typename list<T, Alloc>::link_type
list<T, Alloc>::creat_node(const T& value) {
    link_type p =get_node();
    mystl::construct(&p->data, value);
    return p;
}

template<class T, class Alloc>
void list<T, Alloc>::destroy_node(link_type p) {
    data_allocator::destroy(&(p->data));
    put_node(p);
}

/**********************************************************************
 * 内部使用辅助函数
 * initialize list 相关
 *********************************************************************/
// 配置一个空链表
template<class T, class Alloc>
void list<T, Alloc>::empty_init() {
    node_ =get_node(); // 配置一个节点node，让其头尾都指向自己
    node_->next =node_->prev =node_;
    size_ =0;
}
// fill 初始化
template<class T, class Alloc>
void list<T, Alloc>::fill_init(size_type n, const value_type& value) {
    node_ =get_node(); 
    node_->next =node_->prev =node_;
    size_ =n;

    while (n--) {
        auto node =creat_node(value);
        link_nodes_at_back(node, node);
    }
}
// copy 初始化 [first, last)
template<class T, class Alloc>
template<class Iter>
void list<T, Alloc>::copy_init(Iter first, Iter last) {
    node_ =get_node();
    node_->next =node_->prev =node_;
    size_type n =mystl::distance(first, last);
    // size_type n =static_cast<size_type>(last -first);
    size_ =n;

    while (n--) {
        auto node =creat_node(*(first));
        link_nodes_at_back(node, node);
    }
}

/**********************************************************************
 * 内部使用辅助函数
 * list link 相关 前闭后闭区间[first, last]
 *********************************************************************/
template<class T, class Alloc>
typename list<T, Alloc>::iterator
list<T, Alloc>::link_iter_node(const_iterator pos, link_type new_node) {
    if (pos ==node_->next) {
        // 在头部位置插入
        link_nodes_at_front(new_node, new_node);
    }
    else if (pos ==node_) {
        // 在尾部位置插入
        link_nodes_at_back(new_node, new_node);
    }
    else {
        // 正常位置插入
        link_nodes(pos.node, new_node, new_node);
    }
    return iterator(new_node);
}

// 在pos 处插入[first, last] 的节点
template<class T, class Alloc>
void list<T, Alloc>::link_nodes(link_type pos, link_type first, link_type last) {
    pos->prev ->next =first;
    first->prev =pos->prev;
    pos->prev =last;
    last->next =pos;
}

// 在头部接入 [first, last] 的节点
template<class T, class Alloc>
void list<T, Alloc>::link_nodes_at_front(link_type first, link_type last) {
    first->prev =node_;
    last->next =node_->next;
    last->next->prev =last;
    node_->next =first;
}

// 在尾部接入 [first, last] 的节点
template<class T, class Alloc>
void list<T, Alloc>::link_nodes_at_back(link_type first, link_type last) {
    last->next =node_;
    first->prev =node_->prev;
    first->prev->next =first;
    node_->prev =last;
}

// 容器与 [first, last] 的节点断开连接
template<class T, class Alloc>
void list<T, Alloc>::unlink_nodes(link_type first, link_type last) {
    first->prev->next =last->next;
    last->next->prev =first->prev;
}


/**********************************************************************
 * 操作符重载
 *********************************************************************/
template <class T>
bool operator==(const list<T>& lhs, const list<T>& rhs) {
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
bool operator!=(const list<T>& lhs, const list<T>& rhs) {
    return !(lhs ==rhs);
}

// const 版本
template <class T>
bool operator<(const list<T>& lhs, const list<T>& rhs) {
    return mystl::list_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
    // auto f1 =lhs.cbegin();
    // auto l1 =lhs.cend();
    // auto f2 =rhs.cbegin();
    // auto l2 =rhs.cend();

    // while (f1 !=l1 && f2 !=l2) {
    //     if (*f1 <*f2) return true;
    //     else if (*f2 <*f1) return false;
    //     ++f1;
    //     ++f2;
    // }
    
    // return f1 ==l1 && f2 !=l2;
}

// non-const 版本
template <class T>
bool operator<(list<T>& lhs, list<T>& rhs) {
    return mystl::list_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
    // auto f1 =lhs.begin();
    // auto l1 =lhs.end();
    // auto f2 =rhs.begin();
    // auto l2 =rhs.end();

    // while (f1 !=l1 && f2 !=l2) {
    //     if (*f1 <*f2) return true;
    //     else if (*f2 <*f1) return false;
    //     ++f1;
    //     ++f2;
    // }
    
    // return f1 ==l1 && f2 !=l2;
}



template <class T>
bool operator>(const list<T>& lhs, const list<T>& rhs) {
    return (rhs <lhs);
}

template <class T>
bool operator<=(const list<T>& lhs, const list<T>& rhs) {
    return !(rhs <lhs);
}

template <class T>
bool operator>=(const list<T>& lhs, const list<T>& rhs) {
    return !(lhs <rhs);
}


} // end of namespace mystl

#endif