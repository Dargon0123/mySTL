/**
 * @file my_hashtable.h
 * @author your name (you@domain.com)
 * @brief hashtable 的底层实现
 * @version 0.1
 * @date 2022-06-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __MY_HASHTABLE_H
#define __MY_HASHTABLE_H

// 标准库文件
#include <cstdlib>
#include <cstddef>
#include <utility>
#include <functional>


#include "../allocator/my_allocator.h"
#include "../allocator/my_uninitialized.h"
#include "../algorithm/my_algorithm.h"
#include "../iterator/my_iterator.h"
#include "my_vector.h"


namespace mystl {

/**********************************************************************
 * hashtable 的节点设计
 *********************************************************************/
template <class Value>
struct __hashtable_node {
    Value val; // 数据域
    __hashtable_node* next; // 对应的下一个节点
};

/**********************************************************************
 * 前置声明
 *********************************************************************/
template <class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, template<class T> class Alloc =mystl::allocator> 
class hashtable;

template <class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, template<class T> class Alloc>
struct __hashtable_iterator;

template <class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, template<class T> class Alloc>
struct __hashtable_const_iterator;


/**********************************************************************
 * hashtable 的迭代器设计
 * __hashtable_iterator（可以更改指向的元素值的）
 * __hashtable_const_iterator （不可以更改指向元素值的）
 *********************************************************************/
template <class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, template<class T> class Alloc>
struct __hashtable_iterator {
    typedef hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> hashtable;
    typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> iterator;
    typedef __hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> const_iterator;
    typedef __hashtable_node<Value> node;

    typedef forward_iterator_tag    iterator_category;
    typedef Value                   value_type;
    typedef ptrdiff_t               difference_type;
    typedef size_t                  size_type;
    typedef Value&                  reference;
    typedef Value*                  pointer;

    node* cur; //迭代器目前所指向的节点
    hashtable* ht; // 保持对hashtable的容器的连接关系

    // ctor
    __hashtable_iterator(node* n, hashtable* tab) : cur(n), ht(tab) {}
    __hashtable_iterator() {}
    __hashtable_iterator(iterator& it) : cur(it.cur), ht(it.ht) {}

    // 操作符重载
    reference operator*() const { return cur->val; }
    pointer operator->() const { return &(operator*()); }
    // hashtable只提供前向迭代器
    iterator& operator++();
    iterator operator++(int);
    bool operator==(const iterator& it) const { return cur ==it.cur; }
    bool operator!=(const iterator& it) const { return cur !=it.cur; }
};

// ++i的实现
template <class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, template<class T> class Alloc> 
__hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>& 
__hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::operator++() {
    const node* old =cur;
    cur =cur->next; // 如果存在就是下一个
    if (!cur) {
        // 定位old所在的bucket，需要转移到下一个bucket上的头节点
        size_type bucket =ht->bkt_num(old->val);
        while (!cur && ++bucket <ht->buckets.size()) {
            cur =ht->buckets[bucket]; 
        }
    }
    return *this;
}
// i++的实现
template <class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, template<class T> class Alloc> 
__hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> 
__hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::operator++(int) {
    iterator tmp =*this;
    ++*this;
    return tmp;
}

/**********************************************************************
 * __hashtable_const_iterator
 *********************************************************************/
template <class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, template<class T> class Alloc>
struct __hashtable_const_iterator {
    typedef hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> hashtable;
    typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> iterator;
    typedef __hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> const_iterator;
    typedef __hashtable_node<Value> node;

    typedef forward_iterator_tag    iterator_category;
    typedef Value                   value_type;
    typedef ptrdiff_t               difference_type;
    typedef size_t                  size_type;
    typedef const Value&            reference;
    typedef const Value*            pointer;

    const node* cur; //迭代器目前所指向的节点
    const hashtable* ht; // 保持对hashtable的容器的连接关系

    // ctor
    __hashtable_const_iterator(const node* n, const hashtable* tab) : cur(n), ht(tab) {}
    __hashtable_const_iterator() {}
    __hashtable_const_iterator(const iterator& it) : cur(it.cur), ht(it.ht) {}

    // 操作符重载
    reference operator*() const { return cur->val; }
    pointer operator->() const { return &(operator*()); }
    // hashtable只提供前向迭代器
    const_iterator& operator++();
    const_iterator operator++(int);
    bool operator==(const const_iterator& it) const { return cur ==it.cur; }
    bool operator!=(const const_iterator& it) const { return cur !=it.cur; }
};

// ++i的实现
template <class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, template<class T> class Alloc> 
__hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>& 
__hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::operator++() {
    const node* old =cur;
    cur =cur->next; // 如果存在就是下一个
    if (!cur) {
        // 定位old所在的bucket，需要转移到下一个bucket上的头节点
        size_type bucket =ht->bkt_num(old->val);
        while (!cur && ++bucket <ht->buckets.size()) {
            cur =ht->buckets[bucket]; 
        }
    }
    return *this;
}
// i++的实现
template <class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, template<class T> class Alloc> 
__hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> 
__hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::operator++(int) {
    const_iterator tmp =*this;
    ++*this;
    return tmp;
}

/**********************************************************************
 * hashtable 类的实现
 *********************************************************************/
// 提供 28 个质数，用质数大小来设计 buckets
static const int __stl_num_primes = 28;
static const unsigned long __stl_prime_list[__stl_num_primes] = {
    53ul,         97ul,         193ul,       389ul,       769ul,
    1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
    49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
    1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
    50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul, 
    1610612741ul, 3221225473ul, 4294967291ul
};

// 找出上述28个质数中最接近并大等于n的那个质数 
unsigned long __stl_next_prime(unsigned long n) {
    // 使用二分查找 >= n的最近的数
    int left =0;
    int right =28;
    while (left <right) {
        int mid =left +((right -left) >>1);
        if (n <__stl_prime_list[mid]) {
            right =mid;
        }
        else if (n >__stl_prime_list[mid]) {
            left =mid +1;
        }
        else {
            return __stl_prime_list[mid];
        }
    }
    return __stl_prime_list[left];
}

template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, template<class T> class Alloc>
class hashtable {
public:
    typedef Key         key_type;
    typedef Value       value_type;
    typedef HashFcn     hasher;
    typedef EqualKey    key_equal;

    typedef size_t              size_type;
    typedef ptrdiff_t           difference_type;
    typedef value_type*         pointer;
    typedef const value_type*   const_pointer;
    typedef value_type&         reference;
    typedef const value_type&   const_reference;


private:
    typedef __hashtable_node<Value>     node;
    typedef Alloc<node>                 node_allocator;

    // 以下三者都是function objects
    hasher hash;
    key_equal equals;
    ExtractKey get_key;

    // 数据型别
    mystl::vector<node*> buckets; // 一个向量以node* 为基本元素的vector
    size_type num_elements; // 记录节点个数

public:
    hasher hash_funct() const { return hash; }
    key_equal key_eq() const  { return equals; }

public:
    typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> iterator;
    typedef __hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> const_iterator;

    // 声明右元 iterator::operator++中使用了 bkt_num()
    friend iterator;
    friend const_iterator;

public:
    // ctor
    hashtable(size_type n, const HashFcn& hf, const EqualKey& eql, const ExtractKey& ext) : hash(hf), equals(eql), get_key(ext), num_elements(0) {
        initialize_buckets(n);
    }
    hashtable(size_type n, const HashFcn& hf, const EqualKey& eql) : hash(hf), equals(eql), get_key(ExtractKey()), num_elements(0) {
        initialize_buckets(n);
    }
    // 拷贝构造，深拷贝
    hashtable(const hashtable& ht) : hash(ht.hash), equals(ht.equals), get_key(ht.get_key), num_elements(0) {
        copy_from(ht);
    }
    hashtable& operator=(const hashtable& ht) {
        if (&ht ==this) return *this;
        // 进行赋值
        clear();
        hash =ht.hash;
        equals =ht.equals;
        get_key =ht.get_key;
        copy_from(ht);
    }

    // 析构函数
    ~hashtable() { clear(); }

    // 节点数量相关的函数
    size_type size() const { return num_elements; }
    size_type max_size() const { return size_type(-1); }
    bool enpty() const { return size() ==0; }


    // 遍历hashtable
    void logHashtable() {
        const_iterator ite =cbegin();
        for (int i =0; i <size(); ++i) {
            std::cout << *ite ++ << " ";
        }
        std::cout << std::endl;
    }

    // buckets数量相关的函数
    size_type bucket_count() const { return buckets.size(); }
    size_type max_bucket_count() const { return __stl_prime_list[__stl_num_primes -1]; }
    size_type elems_in_bucket(size_type bucket) const;

    void swap(hashtable& ht);
    void clear();
    // 判断是否重建表格resize
    // 判断原则：节点个数 >= buckets个数，选择重新rehashing
    void resize(size_type num_elements_hint);

    // 迭代器相关
    iterator begin();
    iterator end() { return iterator(nullptr, this); }
    const_iterator cbegin() const;
    const_iterator cend() const { return const_iterator(nullptr, this); }

    // 节点插入相关
    //插入元素，不允许重复
    void insert_unique(const value_type& obj);
    //插入元素，允许重复
    void insert_equal(const value_type& obj);
    //供以上两个函数使用
    void insert_unique_noresize(const value_type& obj);
    void insert_equal_noresize(const value_type& obj);

    // 插入序列，调用单个插入（暂不实现！）
    template<class InputIterator>
    void insert_unique(InputIterator first, InputIterator last, input_iterator_tag);
    template<class InputIterator>
    void insert_equal(InputIterator first, InputIterator last, input_iterator_tag);
    //ForwardIterator类型及以上
    template<class ForwardIterator>
    void insert_unique(ForwardIterator first, ForwardIterator last, forward_iterator_tag);
    template<class ForwardIterator>
    void insert_equal(ForwardIterator first, ForwardIterator last, forward_iterator_tag);


    // 查找相关
    iterator find(const key_type& key);
    const_iterator cfind(const key_type& key) const;
    size_type count(const key_type& key) const;
    std::pair<iterator, iterator> equal_range(const key_type& key);
    std::pair<const_iterator, const_iterator> equal_range(const key_type& key) const;

    // erase 相关
    size_type erase(const key_type& key);
    void erase(const iterator& it);
    void erase(iterator first, iterator last);
    void erase(const const_iterator& it);
    void erase(const_iterator first, const_iterator last);

    //声明友元 operator== 注意此处template不能省略且模板参数名字不能和hashtable类相同
    template<class Vl, class Ky, class HF,
                class Ex, class Eq, template<class t> class Al>
    friend bool operator== (const hashtable<Vl, Ky, HF, Ex, Eq, Al>& ht1,
                            const hashtable<Vl, Ky, HF, Ex, Eq, Al>& ht2);


private:
    // 节点配置与释放
    node* new_node(const value_type& obj);
    void delete_node(node* n);
    size_type next_size(size_type n) const { return __stl_next_prime(n); }
    void initialize_buckets(size_type n);
    void copy_from(const hashtable& ht);

    // 判断元素的落脚处
    //版本1：接受value和buckets个数
    size_type bkt_num(const value_type& obj, size_type n) const 
    { return bkt_num_key(get_key(obj), n);  }
    //版本2：接受value
    size_type bkt_num(const value_type& obj) const
    { return bkt_num_key(get_key(obj)); }
    //版本3：接受key
    size_type bkt_num_key(const key_type& key) const
    { return bkt_num_key(key, buckets.size()); }
    //版本4：接受key和buckets个数
    size_type bkt_num_key(const key_type& key, size_type n) const
    { return hash(key)%n; }

};

/**********************************************************************
 * buckets数量相关的函数
 *********************************************************************/
template<class Value, class Key, class HashFcn, 
        class ExtractKey, class EqualKey, template<class T> class Alloc>
typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type 
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::elems_in_bucket(size_type bucket) const {
    size_type result =0;
    for (node* cur =buckets[bucket]; cur; cur =cur->next) {
        result ++;
    }
    return result;
}

template<class Value, class Key, class HashFcn, 
        class ExtractKey, class EqualKey, template<class T> class Alloc>
void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::clear() {
    // 逐个桶里的逐个节点进行删除
    // 注意最后，的vector并没有清楚掉，后面被一些函数进行调用
    for (size_type i =0; i <buckets.size(); ++i) {
        node* cur =buckets[i];
        while (cur) {
            node* next =cur->next;
            delete_node(cur);
            cur =next;
        }
        buckets[i] =nullptr;
    }
    num_elements =0;
}

template<class Value, class Key, class HashFcn, 
        class ExtractKey, class EqualKey, template<class T> class Alloc>
void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::swap(hashtable& ht) {
    mystl::swap(hash, ht.hash);
    mystl::swap(equals, ht.equals);
    mystl::swap(get_key, ht.get_key);
    buckets.swap(ht.buckets); // 该功能可能没实现
    mystl::swap(num_elements, ht.num_elements);
}

template<class Value, class Key, class HashFcn, 
        class ExtractKey, class EqualKey, template<class T> class Alloc>
void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::resize(size_type num_elements_hint) {
    const size_type old_n =buckets.size();
    if (num_elements_hint <=old_n) return;

    // 进行重新配置
    const size_type n =next_size(num_elements_hint);
    if(n <= old_n) return;
    vector< node*> tmp(n, nullptr);
    try {
        for (size_type bucket =0; bucket <old_n; ++bucket) {
            node* first =buckets[bucket];
            while (first) {
                size_type new_bucket =bkt_num(first->val, n);
                // 以下的四个操作处理的很舒服，刚好和原序列反过来
                buckets[bucket] =first->next;
                first->next =tmp[new_bucket];
                tmp[new_bucket] =first;
                first =buckets[bucket];
            }
        }
        buckets.swap(tmp);
    }
    catch(...) {
        //commit or callback
        for(size_type bucket=0; bucket<tmp.size(); bucket++) {
            while(tmp[bucket]) {
                node *next = tmp[bucket]->next;
                delete_node(tmp[bucket]);
                tmp[bucket] = next;
            }
        }
        throw;
    }
}

/**********************************************************************
 * 迭代器相关
 *********************************************************************/
template<class Value, class Key, class HashFcn, 
        class ExtractKey, class EqualKey, template<class T> class Alloc>
typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::begin() {
    for (size_type n =0; n <buckets.size(); ++n) {
        // 返回第一个有的节点
        if (buckets[n]) {
            return iterator(buckets[n], this);
        }
    }
    return end();
}

template<class Value, class Key, class HashFcn, 
        class ExtractKey, class EqualKey, template<class T> class Alloc>
typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::const_iterator
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::cbegin() const {
    for (size_type n =0; n <buckets.size(); ++n) {
        // 返回第一个有的节点
        if (buckets[n]) {
            return const_iterator(buckets[n], this);
        }
    }
    return cend();
}

/**********************************************************************
 * 节点插入相关
 *********************************************************************/
template<class Value, class Key, class HashFcn,
            class ExtractKey, class EqualKey, template<class T> class Alloc>
void
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_unique(const value_type& obj) {
    //判断新加入一个元素后是否需要重建表格
    resize(num_elements+1);
    return insert_unique_noresize(obj);
}

//供insert_unique使用
template<class Value, class Key, class HashFcn,
            class ExtractKey, class EqualKey, template<class T> class Alloc>
void
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_unique_noresize(const value_type& obj) {
    const size_type n = bkt_num(obj);
    node * first = buckets[n];
    for(node* cur=first; cur; cur=cur->next) {
        // 判断hashtable中是否已存在相同的键值
        if(equals(get_key(cur->val), get_key(obj)))
            return;
    }
    //头插法
    node * tmp = new_node(obj);
    tmp->next = first;
    buckets[n] = tmp;
    num_elements++;
    return;
}

template<class Value, class Key, class HashFcn,
            class ExtractKey, class EqualKey, template<class T> class Alloc>
void
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_equal(const value_type& obj) {
    resize(num_elements+1);
    insert_equal_noresize(obj);
}
//供insert_equal使用
template<class Value, class Key, class HashFcn,
            class ExtractKey, class EqualKey, template<class T> class Alloc>
void
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_equal_noresize(const value_type& obj) {
    const size_type n = bkt_num(obj);
    node * first = buckets[n];
    for(node* cur=first; cur; cur=cur->next) {
        // 键值同，插入后面
        if(equals(get_key(cur->val), get_key(obj))) {
            node * tmp = new_node(obj);
            tmp->next = cur->next;
            cur->next = tmp;
            num_elements++;
            return;
        }
    }
    //头插法
    node * tmp = new_node(obj);
    tmp->next = first;
    buckets[n] = tmp;
    num_elements++;
    return;
}

//插入序列
//InputIterator类型
template<class Value, class Key, class HashFcn,
            class ExtractKey, class EqualKey, template<class T> class Alloc>
template<class InputIterator>
void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_unique(InputIterator first, InputIterator last, input_iterator_tag) {
    for( ; first!=last; first++)
        insert_unique(*first);
}
template<class Value, class Key, class HashFcn,
            class ExtractKey, class EqualKey, template<class T> class Alloc>
template<class InputIterator>
void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_equal(InputIterator first, InputIterator last, input_iterator_tag) {
    for( ; first!=last; first++)
        insert_equal(*first);
}

//ForwardIterator类型及以上
template<class Value, class Key, class HashFcn,
            class ExtractKey, class EqualKey, template<class T> class Alloc>
template<class ForwardIterator>
void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_unique(ForwardIterator first, ForwardIterator last, forward_iterator_tag) {
    size_type n = distance(first, last);
    resize(num_elements+n);
    for( ; n>0; n--,first++)
        insert_unique_noresize(*first);
}
template<class Value, class Key, class HashFcn,
            class ExtractKey, class EqualKey, template<class T> class Alloc>
template<class ForwardIterator>
void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_equal(ForwardIterator first, ForwardIterator last, forward_iterator_tag) {
    size_type n = distance(first, last);
    resize(num_elements+n);
    for( ; n>0; n--,first++)
        insert_equal_noresize(*first);
}

/**********************************************************************
 * 查找相关
 *********************************************************************/
template<class Value, class Key, class HashFcn,
            class ExtractKey, class EqualKey, template<class T> class Alloc>
typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::find(const key_type& key) {
    size_type n = bkt_num_key(key);
    node * first;
    for(first=buckets[n]; first&&!equals(key, get_key(first->val)); first=first->next) ;
    //若未找到，first=nullptr
    return iterator(first, this);
}
template<class Value, class Key, class HashFcn,
            class ExtractKey, class EqualKey, template<class T> class Alloc>
typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::const_iterator
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::cfind(const key_type& key) const {
    size_type n = bkt_num_key(key);
    const node * first;
    for(first=buckets[n]; first&&!equals(key, get_key(first->val)); first=first->next) ;
    return const_iterator(first, this);
}

// count计数
template<class Value, class Key, class HashFcn,
            class ExtractKey, class EqualKey, template<class T> class Alloc>
typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::count(const key_type& key) const {
    const size_type n = bkt_num_key(key);
    size_type result = 0;
    for(const node* cur=buckets[n]; cur; cur=cur->next) {
        if(equals(key, get_key(cur->val)))
            result++;
    }
    return result;
}

// equal range
template<class Value, class Key, class HashFcn,
            class ExtractKey, class EqualKey, template<class T> class Alloc>
std::pair<typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator, 
            typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator> 
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::equal_range(const key_type& key) {
    using Pii = std::pair<iterator, iterator>;
    const size_type n = bkt_num_key(key);
    for(node* first=buckets[n]; first; first=first->next) {
        //键值相同的节点是相连在一起的
        if(equals(key, get_key(first->val))) {
            for(node* cur=first->next; cur; cur=cur->next) {
                if(!equals(key, get_key(cur->val)))
                    return Pii(iterator(first, this), iterator(cur, this));
            }
            //尾后节点
            for(size_type m=n+1; m<buckets.size(); m++) {
                if(buckets[m]) {
                    return Pii(iterator(first, this), iterator(buckets[m], this));
                }
            }
            return Pii(iterator(first, this), end());
        }
    }
    return Pii(end(), end());
}
template<class Value, class Key, class HashFcn,
            class ExtractKey, class EqualKey, template<class T> class Alloc>
std::pair<typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::const_iterator, 
            typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::const_iterator> 
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::equal_range(const key_type& key) const {
    using Pii = std::pair<const_iterator, const_iterator>;
    const size_type n = bkt_num_key(key);
    for(const node* first=buckets[n]; first; first=first->next) {
        //键值相同的节点是相连在一起的
        if(equals(key, get_key(first->val))) {
            for(const node* cur=first->next; cur; cur=cur->next) {
                if(!equals(key, get_key(cur->val)))
                    return Pii(const_iterator(first, this), const_iterator(cur, this));
            }
            //尾后节点
            for(size_type m=n+1; m<buckets.size(); m++) {
                if(buckets[m]) {
                    return Pii(const_iterator(first, this), const_iterator(buckets[m], this));
                }
            }
            return Pii(const_iterator(first, this), end());
        }
    }
    return Pii(end(), end());
}

/**********************************************************************
 * erase相关
 *********************************************************************/
//删除指定节点（键值不唯一时可能会删除多个节点）
template<class Value, class Key, class HashFcn,
            class ExtractKey, class EqualKey, template<class T> class Alloc>
typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::erase(const key_type& key) {
    const size_type n = bkt_num_key(key);
    node * first = buckets[n];
    size_type erased = 0;
    if(first) {
        node * cur = first;
        node * next = cur->next;
        while(next) {
            if(equals(key, get_key(next->val))) {
                cur->next = next->next;
                delete_node(next);
                next = cur->next;
                erased++;
                num_elements--;
            }
            else {
                cur = next;
                next = cur->next;
            }
        }
        if(equals(key, get_key(first->val))) {
            buckets[n] = first->next;
            delete_node(first);
            erased++;
            num_elements--;
        }
    }
    return erased;
}

//删除一个节点
template<class Value, class Key, class HashFcn,
            class ExtractKey, class EqualKey, template<class T> class Alloc>
void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::erase(const iterator& it) {
    node *p = it.cur;
    if(p) {
        const size_type n = bkt_num(p->val);
        node * cur = buckets[n];
        if(cur == p) {
            buckets[n] = cur->next;
            delete_node(cur);
            num_elements--;
        }
        else {
            node * next = cur->next;
            while(next) {
                if(next == p) {
                    cur->next = next->next;
                    delete_node(next);
                    num_elements--;
                    break;
                }
                else {
                    cur = next;
                    next = cur->next;
                }
            }
        }
    }
}

template<class Value, class Key, class HashFcn,
            class ExtractKey, class EqualKey, template<class T> class Alloc>
void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::erase(const const_iterator& it) {
    erase(iterator(const_cast<node*>(it.cur), const_cast<hashtable*>(it.ht)));
}

//删除序列
template<class Value, class Key, class HashFcn,
            class ExtractKey, class EqualKey, template<class T> class Alloc>
void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::erase(iterator first, iterator last) {
    size_type first_bucket = first.cur ? bkt_num(first.cur->val) : buckets.size();
    size_type last_bucket = last.cur ? bkt_num(last.cur->val) : buckets.size();
    if(first.cur == last.cur) return;
    else if(first_bucket == last_bucket)
        erase_bucket(first_bucket, first.cur, last.cur);
    else {
        erase_bucket(first_bucket, first.cur, nullptr);
        for(size_type n=first_bucket+1; n<last_bucket; n++)
            erase_bucket(n, buckets[n], nullptr);
        if(last_bucket != buckets.size())
            erase_bucket(last_bucket, buckets[last_bucket], last.cur);
    }
}
template<class Value, class Key, class HashFcn,
            class ExtractKey, class EqualKey, template<class T> class Alloc>
void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::erase(const_iterator first, const_iterator last) {
    erase(iterator(const_cast<node*>(first.cur), const_cast<hashtable*>(first.ht)),
            iterator(const_cast<node*>(last.cur), const_cast<hashtable*>(last.ht)));
}


/**********************************************************************
 * operator ==, !=
 *********************************************************************/
template<class Vl, class Ky, class HF,
            class Ex, class Eq, template<class t> class Al>
bool operator== (const hashtable<Vl, Ky, HF, Ex, Eq, Al>& ht1,
                    const hashtable<Vl, Ky, HF, Ex, Eq, Al>& ht2) {
    using node  = typename hashtable<Vl, Ky, HF, Ex, Eq, Al>::node;
    if(ht1.buckets.size() != ht2.buckets.size()) return false;
    for(int n=0; n<ht1.buckets.size(); n++) {
        node * cur1 = ht1.buckets[n];
        node * cur2 = ht2.buckets[n];
        for( ; cur1&&cur2&&cur1->val==cur2->val; cur1=cur1->next, cur2=cur2->next) ;
        if(cur1 || cur2) return false;
    }
    return true;
}
template<class Vl, class Ky, class HF,
            class Ex, class Eq, template<class t> class Al>
bool operator!= (const hashtable<Vl, Ky, HF, Ex, Eq, Al>& ht1,
                    const hashtable<Vl, Ky, HF, Ex, Eq, Al>& ht2) {
    return !(ht1==ht2);
} 


/**********************************************************************
 * 辅助函数相关
 *********************************************************************/
template<class Value, class Key, class HashFcn, 
        class ExtractKey, class EqualKey, template<class T> class Alloc>
typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::node* 
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::new_node(const value_type& obj) {
    // 分配内存，构建对象
    node* n =node_allocator::allocate();
    n->next =nullptr;
    try {
        mystl::construct(&(n->val), obj);
        return n;
    }
    catch(...) {
        // "commit or rollback"
        node_allocator::deallocate(n);
        throw;
    }
}

template<class Value, class Key, class HashFcn, 
        class ExtractKey, class EqualKey, template<class T> class Alloc>
void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::delete_node(node* n) {
    // 销毁对象，释放内存
    mystl::destroy(&(n->val));
    node_allocator::deallocate(n);
}


template<class Value, class Key, class HashFcn, 
        class ExtractKey, class EqualKey, template<class T> class Alloc>
void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::initialize_buckets(size_type n) {
    const size_type n_buckets =next_size(n);
    buckets.reserve(n_buckets);
    buckets.insert(buckets.end(), n_buckets, 0);
    num_elements =0;
}

template<class Value, class Key, class HashFcn, 
        class ExtractKey, class EqualKey, template<class T> class Alloc>
void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::copy_from(const hashtable& ht) {
    // (注意：调用此函数之前，已经调用hashtable::clear函数将bucket list上的节点销毁并释放内存，同时所有buckets[i]=nullptr)
    // 清除掉己方的buckets，调用vector::clear
    buckets.clear();
    // 以对方的大小来初始化己方的vector大小
    size_type new_size =ht.buckets.size();
    buckets.reverse(new_size);
    // 初始化元素为空指针
    buckets.insert(buckets.end(), new_size, 0);

    // 逐个进行复制
    try
    {
        for (size_type i =0; i <new_size; ++i) {
            // 复制vector的每一个元素
            if (const node* cur =ht.buckets[i]) {
                node* copy =new_node(cur->val);
                buckets[i] =copy;
                for (node* next =cur->next; next; cur =next, next =cur->next) {
                    copy->next =new_node(next->val);
                    copy =copy->next;
                }
            }
        }
        num_elements =ht.num_elements;
    }
    catch(...)
    {
        // commit or rollback
        clear();
        throw;
    }
    
}


} // end of namespave mystl



#endif