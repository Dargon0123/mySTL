/**
 * @file my_AVL_tree.h
 * @author your name (you@domain.com)
 * @brief 设计AVL平衡树，作为set和map的底部容器
 * @version 0.1
 * @date 2022-06-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __MY_AVL_TREE_H
#define __MY_AVL_TREE_H

// 标准库文件
#include <cstdlib>
#include <cstddef>
#include <utility>
#include <functional>


#include "../allocator/my_allocator.h"
#include "../allocator/my_uninitialized.h"
#include "../algorithm/my_algorithm.h"

namespace mystl {

/**********************************************************************
 * AVLtree 的节点设计
 *********************************************************************/
template <class Value>
struct __AVLTree_node {
    // typedef __AVLTree_node<Value>*  base_ptr;

    Value val; // 节点值
    long long height; // 节点高度
    __AVLTree_node* left; // 左节点
    __AVLTree_node* right; // 右节点
    __AVLTree_node* parent; // 父节点

    // 求该子树上的最小值
    static __AVLTree_node* minimum(__AVLTree_node* x) {
        // 一直搜索左子树
        while (x->left) {
            x =x->left;
        }
        return x;
    }

    // 求该子树上的最大值
    static __AVLTree_node* maximum(__AVLTree_node* x) {
        // 一直搜索右子树
        while (x->right) {
            x =x->right;
        }
        return x;
    }
};

/**********************************************************************
 * AVLtree 的迭代器设计
 * __AVLTree_iterator
 * __AVLTree_const_iterator
 *********************************************************************/
template <class T>
struct __AVLTree_iterator {
    typedef __AVLTree_iterator<T>           iterator;
    typedef __AVLTree_node<T>               node;
    typedef __AVLTree_node<T>*              link_type;

    typedef bidirection_iterator_tag   iterator_category;
    typedef T                          value_type;
    typedef T*                         pointer;
    typedef T&                         reference;
    typedef size_t                     size_type;
    typedef ptrdiff_t                  difference_type;

    // 成员变量
    link_type cur; // 与容器之间产生的一个连接关系

    // ctor
    __AVLTree_iterator() {}
    __AVLTree_iterator(link_type n) : cur(n) {};
    __AVLTree_iterator(const iterator& it) { cur =it.cur; }

    // 操作符重载
    reference operator*() const { return cur->val; }
    pointer operator->() const { return &(operator*()); } // 使用取地址操作符& 传递指针
    iterator& operator++();
    iterator operator++(int);
    iterator& operator--();
    iterator operator--(int);
    bool operator==(const iterator& it) const { return cur ==it.cur; }
    bool operator!=(const iterator& it) const { return cur !=it.cur; }

};

// ++ 的实际实现
template<class T>
__AVLTree_iterator<T>&
__AVLTree_iterator<T>::operator++() {
    // 如果有右子节点,case 1
    if (cur->right) {
        // 寻找右子树的最左节点，即为前进一个
        cur =cur->right;
        while (cur->left) {
            cur =cur->left;
        }
    }
    else {
        // cur 节点本身是右节点，向上回溯
        node* y =cur->parent; 
        while (y->right ==cur) {
            cur =y;
            y =cur->parent;
        }
        // 正常情况执行下面的语句
        // 特殊情况，求root 节点的下一个节点，
        // while执行之后，header->right =root, header->parent =root, root->parent =header
        if (cur->right !=y) {
            cur =y;
        }
    }
    return *this;
}
template<class T>
__AVLTree_iterator<T>
__AVLTree_iterator<T>::operator++(int) {
    iterator tmp =*this;
    ++*this;
    return tmp;
}
// -- 的实际实现
template<class T>
__AVLTree_iterator<T>&
__AVLTree_iterator<T>::operator--() {
    // cur为header，返回其右节点，mostright()
    if (cur->height ==-1) {
        cur =cur->right;
    }
    // 若有左子节点
    else if (cur->left) {
        // 寻找左子树的最右节点
        cur =cur->left;
        while (cur->right) {
            cur =cur->right;
        }
    }
    else {
        // 向上回溯
        node* y =cur->parent;
        while (y->left ==cur) {
            cur =y;
            y =cur->parent;
        }
        cur =y;
    }
    return *this;
}
template<class T>
__AVLTree_iterator<T>
__AVLTree_iterator<T>::operator--(int) {
    iterator tmp =*this;
    --*this;
    return tmp;
}

// __AVLTree_const_iterator
template <class Value>
struct __AVLTree_const_iterator {
    typedef __AVLTree_const_iterator<Value>     const_iterator;
    typedef __AVLTree_node<Value>               node;
    typedef __AVLTree_node<Value>*              link_type;

    typedef bidirection_iterator_tag   iterator_category;
    typedef Value                      value_type;
    typedef const Value*               pointer;
    typedef const Value&               reference;
    typedef size_t                     size_type;
    typedef ptrdiff_t                  difference_type;

    // 成员变量
    const node* cur; // 与容器之间产生的一个连接关系

    // ctor
    __AVLTree_const_iterator() {}
    __AVLTree_const_iterator(link_type n) : cur(n) {};
    __AVLTree_const_iterator(const const_iterator& it) { cur =(const node*)it.cur; }

    // 操作符重载
    reference operator*() const { return cur->val; }
    pointer operator->() const { return &(operator*()); } // 使用取地址操作符& 传递指针
    const_iterator& operator++();
    const_iterator operator++(int);
    const_iterator& operator--();
    const_iterator operator--(int);
    bool operator==(const const_iterator& it) const { return cur ==it.cur; }
    bool operator!=(const const_iterator& it) const { return cur !=it.cur; }
};

// ++ 的实际实现
template<class T>
__AVLTree_const_iterator<T>&
__AVLTree_const_iterator<T>::operator++() {
    // 如果有右子节点,case 1
    if (cur->right) {
        // 寻找右子树的最左节点，即为前进一个
        cur =cur->right;
        while (cur->left) {
            cur =cur->left;
        }
    }
    else {
        // cur 节点本身是右节点，向上回溯
        const node* y =cur->parent; 
        while (y->right ==cur) {
            cur =y;
            y =cur->parent;
        }
        // 正常情况执行下面的语句
        // 特殊情况，求root 节点的下一个节点，
        // while执行之后，header->right =root, header->parent =root, root->parent =header
        if (cur->right !=y) {
            cur =y;
        }
    }
    return *this;
}
template<class T>
__AVLTree_const_iterator<T>
__AVLTree_const_iterator<T>::operator++(int) {
    const_iterator tmp =*this;
    ++*this;
    return tmp;
}
// -- 的实际实现
template<class T>
__AVLTree_const_iterator<T>&
__AVLTree_const_iterator<T>::operator--() {
    // cur为header，返回其右节点，mostright()
    if (cur->height ==-1) {
        cur =cur->right;
    }
    // 若有左子节点
    else if (cur->left) {
        // 寻找左子树的最右节点
        cur =cur->left;
        while (cur->right) {
            cur =cur->right;
        }
    }
    else {
        // 向上回溯
        const node* y =cur->parent;
        while (y->left ==cur) {
            cur =y;
            y =cur->parent;
        }
        cur =y;
    }
    return *this;
}
template<class T>
__AVLTree_const_iterator<T>
__AVLTree_const_iterator<T>::operator--(int) {
    const_iterator tmp =*this;
    --*this;
    return tmp;
}


/**********************************************************************
 * AVLtree 
 * Key：键值型别， Valule：实值型别， KeyOfValue：从节点中取出键值的方法
 * Compare:比较键值的方法， Alloc:空间配置器
 *********************************************************************/
template <class Key, class Value, class KeyOfValue, 
            class Compare, template<class T> class Alloc =mystl::allocator>
class AVLTree {

public:
    typedef Key                     key_type;
    typedef Value                   value_type;
    typedef value_type*             pointer;
    typedef const value_type*       const_pointer;
    typedef value_type&             reference;
    typedef const value_type*       const_reference;
    typedef size_t                  size_type;
    typedef ptrdiff_t               difference_type;


protected:
    typedef  __AVLTree_node<Value>  node;
    typedef  __AVLTree_node<Value>* link_type;
    typedef  mystl::allocator<node> node_allocator;


public:
    typedef __AVLTree_iterator<Value>          iterator;
    typedef __AVLTree_const_iterator<Value>    const_iterator;


private:
    // 一个AVL Tree的占用字节的大小 （4 +4 +1）bytes，
    // 由于内存对齐，总体占用12bytes。

    //关于AVL树中的节点说明：
    //header是AVL树中辅助的头节点，其实值无意义
    //保证其父节点指向根节点root，同时root的父节点也指向header
    //左子节点指向AVL树中最小节点，右子节点指向AVL树中最大节点
    //另：header的height设为-1，此值无意义，但可以用来标识header节点（iterator::operator--()中有用到） 
    size_type node_count;
    node* header;
    Compare key_compare;

public:
    // ctor
    AVLTree(const Compare& comp=Compare()): node_count(0), key_compare(comp) { init(); }
    // 拷贝构造
    AVLTree(const AVLTree& at) : node_count(0), key_compare(at.key_compare) {
        if (at.root() ==nullptr) init();
        else {
            init();
            root() =copy(at.root(), header);
            leftmost() =minmum(root());
            rightmost() =maximum(root());
        }
        node_count =at.node_count;
    }
    // 重载赋值操作符
    AVLTree& operator=(const AVLTree& at);
    ~AVLTree() { clear(); destroy_node(header); }

public:
    //几个size相关函数
    size_type size() const { return node_count; }
    size_type max_size() const { return size_type(-1); }
    bool empty() const { return node_count==0; }
          

    //begin、end、key_comp
    Compare key_comp() const { return key_compare; }
    iterator begin() { return iterator(leftmost()); }
    const_iterator begin() const { return const_iterator(leftmost()); }
    const_iterator cbegin() const { return const_iterator(leftmost()); }
    iterator end() { return iterator(header); }
    const_iterator end() const { return const_iterator(header); }
    const_iterator cend() const { return const_iterator(header); }


    //插入相关
    //插入单个元素，不允许重复
    void insert_unique(const value_type& obj);
    //插入一段序列
    template<class InputIterator>
    void insert_unique(InputIterator first, InputIterator last) { 
        insert_unique(first, last, iterator_category(first)); 
        }
    //InputIterator类型
    template<class InputIterator>
    void insert_unique(InputIterator first, InputIterator last, input_iterator_tag);
    //ForwardIterator类型及以上
    template<class ForwardIterator>
    void insert_unique(ForwardIterator first, ForwardIterator last, forward_iterator_tag);


    //查找
    iterator find(const key_type& k);
    const_iterator find(const key_type& k) const;
    size_type count(const key_type& k) const;
    std::pair<iterator, iterator> equal_range(const key_type& k);
    std::pair<const_iterator, const_iterator> equal_range(const key_type& k) const;
    iterator lower_bound(const key_type& k);
    const_iterator lower_bound(const key_type& k) const;
    iterator upper_bound(const key_type& k);
    const_iterator upper_bound(const key_type& k) const;

    //erase and clear操作
    void clear();
    void erase(const key_type& k);
    void erase(iterator pos);
    void erase(iterator first, iterator last);
    // 针对map特定的擦除操作
    void erase_map(const key_type& k); 
    void erase_map(iterator pos);
    void erase_map(iterator first, iterator last);

    // 遍历AVL
    void log_tree();
    void log_node(node* cur);
    // 针对map的形式
    void log_map_tree();
    void log_map_node(node* cur);

    //友元声明
    template<class Ky, class Vl, class KV,
                class Cp, template<class t> class Al>
    friend bool operator== (const AVLTree<Ky, Vl, KV, Cp, Al>& at1,
                            const AVLTree<Ky, Vl, KV, Cp, Al>& at2);

protected:
    // 节点构造和初始化
    node* get_node();
    void put_node(node* p);
    node* creat_node(const value_type& x);
    void destroy_node(node* n);
    node* clone_node(node* n); 

    // 以下函数取header的成员
    node* & root() const { return header->parent; }
    node* & leftmost() const { return header->left; }
    node* & rightmost() const { return header->right; }

    // 以下函数用来方便取得节点x的成员
    static node* & left(node* x) { return x->left; }
    static node* & right(node* x) { return x->right; }
    static node* & parent(node* x) { return x->parent; }
    static reference value(node* x) { return x->val; }
    static const Key& key(const node* x) { return KeyOfValue()(value(const_cast<node*>(x))); }
    static long long height(node* x) { return x ? x->height : -1; }

    static node* minimum(node* x) { return node::minimum(x); }
    static node* maximum(node* x) { return node::maximum(x); }

private:
    // 辅助函数
    node* copy(node* x, node* p);
    void init();

    
    void __erase(node* x); // 供 clear()调用
    node* __erase(node* &tree, node* x);
    node* __erase_map(node* &tree, node* x); // 供erase_map 调用

    node* __insert(node* &tree, node* parent, const value_type& obj);
    //四种情况对应的旋转操作
    node* leftLeftRotation(node* x);
    node* rightRightRotation(node* x);
    node* leftRightRotation(node* x);
    node* rightLeftRotation(node* x);
};

/**********************************************************************
 * 重载赋值操作符
 *********************************************************************/
template <class Key, class Value, class KeyOfValue, 
            class Compare, template<class T> class Alloc>
AVLTree<Key, Value, KeyOfValue, Compare, Alloc>& 
AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::operator=(const AVLTree& at) {
    // 查看是否重复，直接返回
    if (this ==&at) return *this;
    clear();
    node_count =0;
    key_compare =at.key_compare;
    if (nullptr ==at.root()) {
        root() =nullptr;
        leftmost() =header;
        rightmost() =header;
    }
    else {
        root() =copy(at.root(), header);
        leftmost() =minmum(root());
        rightmost() =maximum(root());
        node_count =at.node_count;
    }
    return *this;
}

/**********************************************************************
 * 插入相关
 *********************************************************************/
template<class Key, class Value, class KeyOfValue,
            class Compare, template <class T> class Alloc>
void AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const value_type& obj) {
    size_type old = node_count;
    __insert(root(), header, obj);
    //更新
    if(old < node_count) {
        leftmost() = minimum(root());
        rightmost() = maximum(root());
    }
}
//插入序列
//InputIterator类型
template<class Key, class Value, class KeyOfValue,
            class Compare, template <class T> class Alloc>
template<class InputIterator>
void AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(InputIterator first, InputIterator last, input_iterator_tag) {
    for( ; first!=last; first++)
        insert_unique(*first);
}
//ForwardIterator类型及以上
template<class Key, class Value, class KeyOfValue,
            class Compare, template <class T> class Alloc>
template<class ForwardIterator>
void AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(ForwardIterator first, ForwardIterator last, forward_iterator_tag) {
    size_type n = distance(first, last);
    for( ; n>0; n--,first++)
        insert_unique(*first);
}

/**********************************************************************
 * 查找相关
 * AVL平衡树最拿手的操作，lon(n)的时间
 *********************************************************************/
template<class Key, class Value, class KeyOfValue,
            class Compare, template <class T> class Alloc>
typename AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::iterator 
AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::find(const key_type& k) {
    node* y = header;
    node* x = root();
    
    while(nullptr != x) {
        //k<=key(x) 则向左 并用y记录x
        if(!key_compare(key(x), k))
            y = x, x = left(x);
        else x = right(x);
    }

    iterator j = iterator(y);
    //满足j!=end()并且k>=key(x) 结合之前的k<=key(x) 因此k=key(x)
    return (j==end() || key_compare(k, key(j.cur))) ? end() : j;
}
template<class Key, class Value, class KeyOfValue,
            class Compare, template <class T> class Alloc>
typename AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator
AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::find(const key_type& k) const {
    node* y = header;
    node* x = root();
    
    while(nullptr != x) {
        //k<=key(x) 则向左 并用y记录x
        if(!key_compare(key(x), k))
            y = x, x = left(x);
        else x = right(x);
    }

    const_iterator j = const_iterator(y);
    //满足j!=end()并且k>=key(x) 结合之前的k<=key(x) 因此k=key(x)
    return (j==end() || key_compare(k, key(j.cur))) ? end() : j;
}
template<class Key, class Value, class KeyOfValue,
            class Compare, template <class T> class Alloc>
typename AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::size_type 
AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::count(const key_type& k) const {
    std::pair<const_iterator, const_iterator> p = equal_range(k);
    // 这里应使用mystl::distance,以免发生调用冲突
    size_type n = mystl::distance(p.first, p.second); 
    return n;
}
template<class Key, class Value, class KeyOfValue,
            class Compare, template <class T> class Alloc>
std::pair<typename AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::iterator,
typename AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::iterator>
AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::equal_range(const key_type& k) {
    return std::pair<iterator, iterator>(lower_bound(k), upper_bound(k));
}
template<class Key, class Value, class KeyOfValue,
            class Compare, template <class T> class Alloc>
std::pair<typename AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator,
typename AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator>
AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::equal_range(const key_type& k) const {
    return std::pair<const_iterator, const_iterator>(lower_bound(k), upper_bound(k));
}
template<class Key, class Value, class KeyOfValue,
            class Compare, template <class T> class Alloc>
typename AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::iterator 
AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::lower_bound(const key_type& k) {
    node* y = header;
    node* x = root();
    while(nullptr != x) {
        //k<=key(x)
        if(!key_compare(key(x), k))
            y = x, x = left(x);
        else x = right(x);
    }
    return iterator(y);
}
template<class Key, class Value, class KeyOfValue,
            class Compare, template <class T> class Alloc>
typename AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator 
AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::lower_bound(const key_type& k) const {
    node* y = header;
    node* x = root();
    while(nullptr != x) {
        //k<=key(x)
        if(!key_compare(key(x), k))
            y = x, x = left(x);
        else x = right(x);
    }
    return const_iterator(y);
}
template<class Key, class Value, class KeyOfValue,
            class Compare, template <class T> class Alloc>
typename AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::iterator 
AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::upper_bound(const key_type& k) {
    node* y = header;
    node* x = root();
    while(nullptr != x) {
        //k<key(x)
        if(key_compare(k, key(x)))
            y = x, x = left(x);
        else x = right(x);
    }
    return iterator(y);
}
template<class Key, class Value, class KeyOfValue,
            class Compare, template <class T> class Alloc>
typename AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator 
AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::upper_bound(const key_type& k) const {
    node* y = header;
    node* x = root();
    while(nullptr != x) {
        //k<key(x)
        if(key_compare(k, key(x)))
            y = x, x = left(x);
        else x = right(x);
    }
    return const_iterator(y);
}

/**********************************************************************
 * erase 节点
 *********************************************************************/
template<class Key, class Value, class KeyOfValue,
            class Compare, template <class T> class Alloc>
void AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::erase(const key_type& k) {
    node* x;
    if(nullptr != (x=find(k).cur))
        root() = __erase(root(), x);
}
template<class Key, class Value, class KeyOfValue,
            class Compare, template <class T> class Alloc>
void AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::erase(iterator pos) {
    root() = __erase(root(), pos.cur);
}
template<class Key, class Value, class KeyOfValue,
            class Compare, template <class T> class Alloc>
void AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::erase(iterator first, iterator last) {
    if(first==begin() && last==end())
        clear();
    else while(first != last) erase(first++);
}

template<class Key, class Value, class KeyOfValue,
            class Compare, template <class T> class Alloc>
void AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::erase_map(const key_type& k) {
    node* x;
    if(nullptr != (x=find(k).cur))
        root() = __erase_map(root(), x);
}
template<class Key, class Value, class KeyOfValue,
            class Compare, template <class T> class Alloc>
void AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::erase_map(iterator pos) {
    root() = __erase_map(root(), pos.cur);
}
template<class Key, class Value, class KeyOfValue,
            class Compare, template <class T> class Alloc>
void AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::erase_map(iterator first, iterator last) {
    if(first==begin() && last==end())
        clear();
    else while(first != last) erase_map(first++);
}

/**********************************************************************
 * 遍历AVL树
 *********************************************************************/
template <class Key, class Value, class KeyOfValue, 
            class Compare, template<class T> class Alloc>
void AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::log_tree() {
    std::cout << "tree.size()----" << size() << std::endl;
    auto first =begin();
    auto last =end();
    std::cout << "[" ;
    for (; first !=last; ++first) {
        std::cout << *first << " ";
    } 
    std::cout << "]" << std::endl;
}
template <class Key, class Value, class KeyOfValue, 
            class Compare, template<class T> class Alloc>
void AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::log_map_tree() {
    std::cout << "tree.size()----" << size() << std::endl;
    auto first =begin();
    auto last =end();
    std::cout << "[" ;
    for (; first !=last; ++first) {
        std::cout << "[" << (*first).first << ": " << (*first).second << "]" << ", ";
    } 
    std::cout << "]" << std::endl;
}
// 打印节点信息
template <class Key, class Value, class KeyOfValue, 
            class Compare, template<class T> class Alloc>
void AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::log_node(node* cur) {
    std::cout << "cur->height----" << cur->height << std::endl;
    std::cout << "cur->val----" << cur->val << std::endl;
}
// 打印节点信息
template <class Key, class Value, class KeyOfValue, 
            class Compare, template<class T> class Alloc>
void AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::log_map_node(node* cur) {
    std::cout << "cur->height----" << cur->height << std::endl;
    std::cout << "cur->val----[" << (cur->val).first << ": " << (cur->val).second << "]" <<std::endl;
}

/**********************************************************************
 * AVLtree 节点的初始化
 *********************************************************************/
// get_node
template <class Key, class Value, class KeyOfValue, 
            class Compare, template<class T> class Alloc>
typename AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::node*
AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::get_node() {
    // 配置一个node的空间
    return node_allocator::allocate(1);
}

// put_node
template <class Key, class Value, class KeyOfValue, 
            class Compare, template<class T> class Alloc>
void AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::put_node(node* p) {
    // 收回一个node的空间
    node_allocator::deallocate(p);
}

// creat_node
template <class Key, class Value, class KeyOfValue, 
            class Compare, template<class T> class Alloc>
typename AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::node*
AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::creat_node(const value_type& x) {
    // 分配内存
    node* tmp =get_node();
    tmp->left =tmp->right =tmp->parent =nullptr;
    tmp->height =0;
    // 进行构造
    try {
        mystl::construct(&(tmp->val), x); // 调用全局进行构建
        return tmp;
    }
    catch(...) {
        put_node(tmp);
        throw;
    }
}

// delete_node
template <class Key, class Value, class KeyOfValue, 
            class Compare, template<class T> class Alloc>
void AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::destroy_node(node* n) {
    // 先进行析构，再回收内存
    destroy(&n->val);
    put_node(n);
}

// clone_node
template <class Key, class Value, class KeyOfValue, 
            class Compare, template<class T> class Alloc>
typename AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::node*
AVLTree<Key, Value, KeyOfValue, Compare, Alloc>:: clone_node(node* n) {
    node* tmp =creat_node(n->val);
    tmp->height =n->height;
    tmp->left =tmp->right =tmp->parent =nullptr;
    return tmp;
}

/**********************************************************************
 * 辅助函数
 * AVLtree init
 *********************************************************************/
template <class Key, class Value, class KeyOfValue, 
            class Compare, template<class T> class Alloc>
void AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::init() {
    header =get_node();
    header->height =-1;

    root() =nullptr;
    leftmost() =header;
    rightmost() =header;
}

/**********************************************************************
 * 辅助函数
 * AVLtree copy 初始化
 *********************************************************************/
template<class Key, class Value, class KeyOfValue,
             class Compare, template <class T> class Alloc>
typename AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::node* 
AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::copy(node* x, node* p) {
    //递归地copy
    node * top = clone_node(x);
    top->parent = p;
    try{
        if(x->right)
            top->right = copy(right(x), top);
        p = top;
        x = left(x);
        
        while(x) {
            node *y = clone_node(x);
            p->left = y;
            y->parent = p;
            if(x->right)
                y->right = copy(right(x), y);
            p = y;
            x = left(x);
        }
    }
    catch(...) {
        // "commit or rollback"
        __erase(top);
    }
    return top;
}

/**********************************************************************
 * 辅助函数
 * AVLtree clear 初始化
 *********************************************************************/
template<class Key, class Value, class KeyOfValue,
            class Compare, template <class T> class Alloc>
void AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::clear() {
    if(0 != node_count) {
        __erase(root());
        root() = nullptr;
        leftmost() = header;
        rightmost() = header;
        node_count = 0;
    }
}
/**********************************************************************
 * 辅助函数
 * __erase() ,供 clear()调用
 *********************************************************************/
template<class Key, class Value, class KeyOfValue,
            class Compare, template <class T> class Alloc>
void AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::__erase(node* x) {
    // 不考虑重新平衡的问题
    // 按照右中左，顺序删除，
    while(nullptr != x) {
        __erase(right(x));
        node* y = left(x);
        destroy_node(x);
        x = y;
    }
}
/**********************************************************************
 * 辅助函数
 * __erase() ,供 erase()调用
 *********************************************************************/
template<class Key, class Value, class KeyOfValue,
             class Compare, template <class T> class Alloc>
typename AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::node* 
AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::__erase(node* &tree, node* x) {
    if(nullptr==tree || nullptr==x)
        return nullptr;  
    //删除节点在左子树
    if(key_compare(key(x), key(tree))) {
        tree->left = __erase(tree->left, x);
        //失衡
        if(height(tree->right)-height(tree->left) >= 2) {
            node* r = tree->right;
            if(height(r->left) > height(r->right))
                tree = rightLeftRotation(tree);
            else 
                tree = rightRightRotation(tree);
        }
    }
    //删除节点在右子树
    else if(key_compare(key(tree), key(x))) {
        tree->right = __erase(tree->right, x);
        if(height(tree->left)-height(tree->right) >= 2) {
            node* l = tree->left;
            if(height(l->right) > height(l->left))
                tree = leftRightRotation(tree);
            else 
                tree = leftLeftRotation(tree);
        }
    }
    //tree是需要删除的节点
    else {
        //左右子节点均非空
        if((nullptr!=tree->left) && (nullptr!=tree->right)) {
            //若tree的左子树比右子树高 找出左子树中最大节点 将该最大节点的值赋值给tree 删除该最大节点
            if(height(tree->left) > height(tree->right)) {
                node* max = maximum(tree->left);
                tree->val = max->val; // map节点的val域不能直接等于赋值
                tree->left = __erase(tree->left, max);
            }
            else {
                node* min = minimum(tree->right);
                tree->val = min->val;
                tree->right = __erase(tree->right, min);
            }
        }
        else {
            node* tmp = tree;
            tree = (nullptr !=tree->left) ? tree->left : tree->right;
            if(nullptr != tree) {
                tree->parent = tmp->parent;
            }
            destroy_node(tmp);
            node_count--;
            //更新
            if(nullptr == root()) {
                leftmost() = header;
                rightmost() = header;
            }
            else {
                leftmost() = minimum(root());
                rightmost() = maximum(root());
            }
        }
    }
    return tree;
} 

template<class Key, class Value, class KeyOfValue,
             class Compare, template <class T> class Alloc>
typename AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::node* 
AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::__erase_map(node* &tree, node* x) {
    if(nullptr==tree || nullptr==x)
        return nullptr;  
    //删除节点在左子树
    if(key_compare(key(x), key(tree))) {
        tree->left = __erase_map(tree->left, x);
        //失衡
        if(height(tree->right)-height(tree->left) >= 2) {
            node* r = tree->right;
            if(height(r->left) > height(r->right))
                tree = rightLeftRotation(tree);
            else 
                tree = rightRightRotation(tree);
        }
    }
    //删除节点在右子树
    else if(key_compare(key(tree), key(x))) {
        tree->right = __erase_map(tree->right, x);
        if(height(tree->left)-height(tree->right) >= 2) {
            node* l = tree->left;
            if(height(l->right) > height(l->left))
                tree = leftRightRotation(tree);
            else 
                tree = leftLeftRotation(tree);
        }
    }
    //tree是需要删除的节点
    else {
        //左右子节点均非空
        if((nullptr!=tree->left) && (nullptr!=tree->right)) {
            //若tree的左子树比右子树高 找出左子树中最大节点 将该最大节点的值赋值给tree 删除该最大节点
            if(height(tree->left) > height(tree->right)) {
                node* max = maximum(tree->left);
                // tree->val = max->val; // map节点的val域不能直接等于赋值
                // test
                node* tmp =get_node();
                tmp =max;
                tmp->parent =tree->parent;
                tmp->left =tree->left;
                tmp->right =tree->right;
                destroy_node(tree);
                tmp->left = __erase_map(tmp->left, max);
                // test
                // tree->left = __erase(tree->left, max);
            }
            else {
                node* min = minimum(tree->right);
                // tree->val = min->val;
                // test
                node* tmp =get_node();
                tmp =min;
                tmp->parent =tree->parent;
                tmp->left =tree->left;
                tmp->right =tree->right;
                destroy_node(tree);
                tmp->right = __erase_map(tmp->right, min);
                // test
                // tree->right = __erase(tree->right, min);
            }
        }
        else {
            node* tmp = tree;
            tree = (nullptr !=tree->left) ? tree->left : tree->right;
            if(nullptr != tree) {
                tree->parent = tmp->parent;
            }
            destroy_node(tmp);
            node_count--;
            //更新
            if(nullptr == root()) {
                leftmost() = header;
                rightmost() = header;
            }
            else {
                leftmost() = minimum(root());
                rightmost() = maximum(root());
            }
        }
    }
    return tree;
}

/**********************************************************************
 * 辅助函数
 * __insert() ,供 insert()调用
 *********************************************************************/
template<class Key, class Value, class KeyOfValue,
            class Compare, template <class T> class Alloc>
typename AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::node*
AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::__insert(node* &tree, node* parent, const value_type& obj) {
    if(nullptr == tree) {
        tree = creat_node(obj);
        tree->parent = parent;
        node_count++;
    }
    //插入到左子树
    else if(key_compare(KeyOfValue()(obj), key(tree))) {
        tree->left = __insert(tree->left, tree, obj);
        //失去平衡
        if(height(tree->left)-height(tree->right) >= 2) {
            //LL情况
            if(key_compare(KeyOfValue()(obj), key(tree->left)))
                tree = leftLeftRotation(tree);
            //LR情况
            else tree = leftRightRotation(tree);
        }
    }
    else if(key_compare(key(tree), KeyOfValue()(obj))) {
        tree->right = __insert(tree->right, tree, obj);
        //失去平衡
        if(height(tree->right)-height(tree->left) >= 2) {
            //RR情况
            if(key_compare(key(tree->right) ,KeyOfValue()(obj)))
                tree = rightRightRotation(tree);
            //RL情况
            else tree = rightLeftRotation(tree);
        }
    }
    //当key相同时，以上两个if都返回false，插入失败 不应重复插入
    else {}
    //求高度 叶子节点高度为0，某节点的高度为max(其左右子节点高度)+1
    tree->height = max(height(tree->left), height(tree->right))+1;

    return tree;
}

/**********************************************************************
 * 辅助函数
 * 四种对应的旋转操作
 *********************************************************************/
// 插入点位于x 的左子节点的左子树
// 左左 ，外侧插入 ()为待插入点 
/*---------------------------------------*\
|       x                         n       |
|      / \                       / \      |
|     n   c    left left        a   x     |
|    / \       ===========>    /   / \    |
|   a   b                     ()  b   c   |
|  /                                      |
| ()                                      |
\*---------------------------------------*/
template<class Key, class Value, class KeyOfValue,
            class Compare, template <class T> class Alloc>
typename AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::node*
AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::leftLeftRotation(node* x) {
    node* n = x->left;
    x->left = n->right;
    n->right = x;
    //parent
    n->parent = x->parent;
    x->parent = n;
    if(nullptr != x->left) x->left->parent = x;

    x->height = max(height(x->left), height(x->right))+1;
    n->height = max(height(n->left), x->height)+1;
    
    return n;
}
// 插入点位于x 的右子节点的右子树
// 右右 ，外侧插入 ()为待插入点
/*---------------------------------------*\
|       x                         n       |
|      / \                       / \      |
|     c   n    right right      x   a     |
|        / \   ===========>    / \   \    |
|       b   a                 c   b   ()  |
|            \                            |
|            ()                           |
\*---------------------------------------*/
template<class Key, class Value, class KeyOfValue,
            class Compare, template <class T> class Alloc>
typename AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::node*
AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::rightRightRotation(node* x) {
    node* n = x->right;
    x->right = n->left;
    n->left = x;
    //parent
    n->parent = x->parent;
    x->parent = n;
    if(nullptr != x->right) x->right->parent = x;
    
    x->height = max(height(x->left), height(x->right))+1;
    n->height = max(height(n->right), x->height)+1;
    
    return n;
}
// 插入点位于x 的左子节点的右子树
// 左右 ，内侧插入 ()为待插入点
/*----------------------------------------------------------*\
|       k                         k                          |
|      / \                       / \                         |
|     x   d    left right       n   d              n         |
|    / \       ===========>    / \     ====>      /  \       |
|   a   n                     x                  x     k     |
|      / \                   / \                / \   / \    |
|     ()                    a  ()              a  ()     d   |
\*----------------------------------------------------------*/
template<class Key, class Value, class KeyOfValue,
            class Compare, template <class T> class Alloc>
typename AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::node*
AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::leftRightRotation(node* x) {
    x->left = rightRightRotation(x->left);
    
    return leftLeftRotation(x);
}
// 插入点位于x 的右子节点的左子树
// 右左 ，内侧插入 ()为待插入点
/*----------------------------------------------------------*\
|       k                         k                          |
|      / \                       / \                         |
|     d   x    left right       d   n               n        |
|        / \   ===========>        /  \ ====>      /  \      |
|       n   a                          x         k     x     |
|      / \                            / \       / \   / \    |
|         ()                         ()  a     d     ()  a   |
\*----------------------------------------------------------*/
template<class Key, class Value, class KeyOfValue,
            class Compare, template <class T> class Alloc>
typename AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::node*
AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::rightLeftRotation(node* x) {
    x->right = leftLeftRotation(x->right);

    return rightRightRotation(x);
}


} // end of namespace mystl


#endif