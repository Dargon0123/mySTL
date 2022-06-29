/**
 * @file my_hash_set.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-06-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __MY_HASH_SET_H
#define __MY_HASH_SET_H

// 标准库文件
#include <cstdlib>
#include <cstddef>
#include <utility>
#include <functional>

#include "../allocator/my_allocator.h"
#include "my_hashtable.h"

namespace mystl {

template <class Value, class HashFcn =std::hash<Value>, class EqualKey=std::equal_to<Value>, template<class T> class Alloc=allocator> 
class hash_set {
private:
    // 底层以hashtable实现
    typedef mystl::hashtable<Value, Value, HashFcn, std::_Identity<Value>, EqualKey, Alloc> ht;

    ht rep;

public:
    // C++ 11新特性
    using key_type = typename ht::key_type;
    using value_type = typename ht::value_type;
    using hasher = typename ht::hasher;
    using key_equal = typename ht::key_equal;
    
    using size_type = typename ht::size_type;
    using difference_type = typename ht::difference_type;
    using pointer = typename ht::const_pointer;
    using const_pointer = typename ht::const_pointer;
    using reference = typename ht::const_reference;
    using const_reference = typename ht::const_reference;

    using iterator = typename ht::const_iterator;
    using const_iterator = typename ht::const_iterator;

    hasher hash_funct() const { return rep.hash_funct(); }
    key_equal key_eq() const { return rep.key_eq(); }

    //构造函数 默认使用大小100的表格，hashtable将自动调整为最接近且较大的质数
    hash_set(): rep(100, hasher(), key_equal()) {}
    explicit hash_set(size_type n): rep(n, hasher(), key_equal()) {}
    hash_set(size_type n, const hasher& hf): rep(n, hf, key_equal()) {}
    hash_set(size_type n, const hasher& hf, const key_equal& eql): rep(n, hf, eql) {}

    hash_set(const hash_set& x): rep(x.rep) {}

    //赋值操作
    hash_set& operator=(const hash_set& x) {
        rep = x.rep;
        return *this;
    }

    // 遍历
    void logSet() { rep.logHashtable(); }

    //几个size相关函数
    size_type size() const { return rep.size(); }
    size_type max_size() const { return rep.max_size(); }
    bool empty() const { return rep.empty(); }
    
    //与bucket相关
    size_type bucket_count() const { return rep.bucket_count(); }
    size_type max_bucket_count() const { return rep.max_bucket_count(); }
    size_type elems_in_bucket(size_type bucket) const { return rep.elems_in_bucket(bucket); }

    void swap(hash_set& us) { rep.swap(us.rep); }
    void clear() { rep.clear(); }
    void resize(size_type num_elements_hint) { rep.resize(num_elements_hint); }

    //begin end
    const_iterator begin() const { return rep.begin(); }
    const_iterator end() const { return rep.end(); }

    // 插入操作 不允许键值重复
    void insert(const value_type& obj) {
        // 注意 hash_set::iterator是ht::const_iterator而不是ht::iterator
        rep.insert_unique(obj);
    }
    template<class InputIterator>
    void insert(InputIterator first, InputIterator last) { rep.insert_unique(first, last); }
    //插入前 不进行表格重建（扩充bucket）的判断
    void insert_noresize(const value_type& obj) {
        insert_unique_noresize(obj);
    }

    // 查找相关
    // find调用const类型的find
    iterator find(const key_type& key) const { return rep.cfind(key); }
    size_type count(const key_type& key) const { return rep.count(key); }
    std::pair<iterator, iterator> equal_range(const key_type& key) const { return rep.equal_range(key); }

    // erase相关
    size_type erase(const key_type& key) { return rep.erase(key); }
    void erase(iterator it) { rep.erase(it); }
    void erase(iterator first, iterator last) { rep.erase(first, last); }

    // 声明友元 operator==
    template<class Vl, class HF,
                class Eq, template<class t> class Al>
    friend bool operator== (const hash_set<Vl, HF, Eq, Al>& ht1,
                            const hash_set<Vl, HF, Eq, Al>& ht2);


}; // end of class hash_set 

/**********************************************************************
 * operator== operator!=
 *********************************************************************/
template<class Vl, class HF,
            class Eq, template<class t> class Al>
bool operator== (const hash_set<Vl, HF, Eq, Al>& us1,
                    const hash_set<Vl, HF, Eq, Al>& us2) {
    return us1.rep == us2.rep;                     
}
template<class Vl, class HF,
            class Eq, template<class t> class Al>
bool operator!= (const hash_set<Vl, HF, Eq, Al>& us1,
                    const hash_set<Vl, HF, Eq, Al>& us2) {
    return !(us1==us2);              
}


} // end of namespace mystl


#endif