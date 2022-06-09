/**
 * @file my_alloc.h
 * @author your name (you@domain.com)
 * @brief 设计具备次配置能力的allocate，供allocator使用
 * @version 0.1
 * @date 2022-04-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __MY_ALLOC_H
#define __MY_ALLOC_H

#include <cstdlib>
#include <iostream>

namespace mystl {

class alloc {
private:
    enum{__ALIGN =8}; // 小型区块的上调边界
    enum{__MAX_BYTES =128}; // 小型区块的上限，超过部分直接使用malloc 分配
    enum{__FREELISTS =__MAX_BYTES /__ALIGN}; // freelists 的个数
    enum{__NOBJS =20}; // 默认每次从内存池取20个区块

private:
    // 将bytes 上调至8的倍数
    static size_t ROUND_UP(size_t bytes) {
        return ( (bytes +__ALIGN -1) & ~(__ALIGN -1) );
    }

private:
    // free_lists 的节点,用联合体，两者公用一块内存
    union obj {
        union obj *free_list_link;
        char client_data[1];
    };
    // 定义16个free_lists 的节点
    static obj * volatile free_list[__FREELISTS];

private:
    // 定义内存池 memory pool
    static char *start_free; // 内存池起始位置
    static char *end_free;   // 内存池结束位置
    static size_t heap_size; // 堆尺寸的大小

private:
    // 根据区块大小，决定使用第n号free_list,下标从0开始算起
    static size_t FREELIST_INDEX(size_t bytes) {
        return ( (bytes +__ALIGN -1) /__ALIGN -1 );
    }
    // 返回一个大小为n的对象，并可能加入大小为n的其它区块到free_list
    static void *refill(size_t n);
    // 配置一大块空间，可容纳nobjs 个大小为size的区块
    // 如果内存池空间不够，就减少nobjs的个数(传递的是引用，可以被改变的值)
    static char *chunk_alloc(size_t size, size_t &nobjs);

public:
    // 分配内存
    static void *allocate(size_t n);
    // 回收内存
    static void deallocate(void *ptr, size_t n);
    // 重新分配
    static void *reallocate(void *ptr, size_t old_sz, size_t new_size);

public:
    alloc(){}; // 默认构造函数
    ~alloc(){};// 默认析构函数

};

// 初始化相应的变量
char* alloc::start_free =nullptr;
char* alloc::end_free =nullptr;
size_t alloc::heap_size =0;

alloc::obj* volatile alloc::free_list[alloc::__FREELISTS] ={
    nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, nullptr
};

// 分配内存,以字节为单位
void *alloc::allocate(size_t n) {
    // > 128bytes,就直接调用一级配置器
    if (n >(size_t)__MAX_BYTES) {
        return (malloc(n));
    }
    // 寻找16个freelist中适当的一个
    size_t index =FREELIST_INDEX(n);
    obj* my_free_list =free_list[index];
    // 判断是否有可用空间
    if (my_free_list) {
        free_list[index] =my_free_list->free_list_link;
        return my_free_list; // 返回一个可用的区块
    }
    // 该区域没有可用的空间了,需要重新整理空间
    return refill(ROUND_UP(n));
}

// 回收内存
void alloc::deallocate(void *ptr, size_t n) {
    // > 128bytes,就直接调用一级配置器
    if (n >(size_t)__MAX_BYTES) {
        free(ptr);
        return;
    }
    // 寻找对应的free_list,并收回来    
    size_t index =FREELIST_INDEX(n);
    obj* my_free_list =static_cast<obj*>(ptr);
    my_free_list->free_list_link =free_list[index];
    free_list[index] =my_free_list;
}

// 需要分配的空间块没有了，重新从内存池里面默认构建20个（可以被改变）
// n已经上调到8的倍数
void *alloc::refill(size_t n) {
    // 默认补充20个区块
    size_t nobjs =__NOBJS;
    char* chunk =chunk_alloc(n, nobjs); // 申请一块chunk空间
    void* result;
    obj* cur_obj, *next_obj;
    if (nobjs ==1) { // 分配的内存，只配置一个size的obj
        return static_cast<void*>(chunk);
    }
    // 否则进行调整free_list, 纳入新节点
    size_t index =FREELIST_INDEX(n);

    // 在chunk 空间内建立free list
    result =static_cast<void*>(chunk); // 准备返回给客户使用
    free_list[index] =next_obj =reinterpret_cast<obj*>(chunk +n);
    // 将取出的多于空间加入到相应的free_list里面去
    for (int i =1; i <nobjs -1; ++i) {
        cur_obj =next_obj;
        next_obj =reinterpret_cast<obj*>(reinterpret_cast<char*>(next_obj) +n);
        cur_obj->free_list_link =next_obj;
    }
    // 注意最后接一个空指针
    next_obj->free_list_link =nullptr;
    return result;

}

// 配置一大块空间，可容纳nobjs 个大小为size的空间
// 如果内存池空间不够，就减少nobjs的个数(传递的是引用，可以被改变的值)
char *alloc::chunk_alloc(size_t size, size_t &nobjs) {
    char* result =nullptr;
    size_t total_bytes =size * nobjs;
    size_t bytes_left =end_free -start_free;
    
    if (bytes_left >=total_bytes) { 
        // 内存池剩余空间完全满足需求
        result =start_free;
        start_free +=total_bytes;
        return (result);
    }
    else if (bytes_left >=size) { 
        // 内存池剩余空间不能完全满足，但可以提供至少一个大小size的区块
        nobjs =bytes_left /size;
        total_bytes =size *nobjs;
        result =start_free;
        start_free +=total_bytes;
        return result;
    }
    else {
        // 内存池剩余空间一个size都不能提供        
        // 回收内存池中的残余零头
        if (bytes_left >0) {
            size_t index =FREELIST_INDEX(bytes_left);
            obj* my_free_list =reinterpret_cast<obj*>(start_free);
            my_free_list->free_list_link =free_list[index];
            free_list[index] =my_free_list;
            start_free =end_free;
        }
        // 配置heap空间，重新补充内存池
        size_t bytes_to_get =2 *total_bytes +ROUND_UP(heap_size >>4);
        start_free =static_cast<char*>(malloc(bytes_to_get));
        if (start_free ==nullptr) {
            // heap空间不足, malloc失败
            // 开始试着检视我们手上所有的东西，不会造成任何伤害，不打算尝试配置
            // 较小的区块，多进程上容易导致灾难
            // 搜寻适当的区块
            // 所谓适当，是指“尚有未用区块，且区块足够大”的free_list
            for (int i =size; i <__MAX_BYTES; i +=__ALIGN) {
                size_t index =FREELIST_INDEX(i);
                obj* p =free_list[index];
                if (p !=nullptr) { // free_list 尚有未用区块
                    // 调整free_list 释放出未用区块
                    free_list[index] =p->free_list_link;
                    start_free =reinterpret_cast<char*>(p);
                    end_free =start_free +i;
                    // 递归调用自己，为了修正nobjs
                    return (chunk_alloc(size, nobjs));
                    // 注意，任何残余零头终将被编入适当的free_list中备用
                }
            }
            // 如果出现意外，山穷水尽，到处都没有内存可以用了
            end_free =0;
            // 直接抛出异常，SGI STL去调用一级配置器的饿 out-of-memory 机制能否近点力
            std::cout << "out of memory!";
            throw std::bad_alloc();
        }
        heap_size +=bytes_to_get;
        end_free =start_free +bytes_to_get;
        return chunk_alloc(size, nobjs);
    }
}

} // end of namespace mystl

#endif