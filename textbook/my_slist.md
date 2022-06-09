[toc]



# slist 的迭代器类型

`slist`提供的是一个单向链表，迭代器只具备后移的能力，迭代器提供的是`forward_iterator_tag`形式，同时为符合`C++`的`OOP`的代码习惯，设计`slist_iterator_base` 和`slist_iterator`进行继承。

迭代器设计的过程中，重载作为指针应该用到的基本操作，

```cpp
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
```



# slist 的数据结构

一个空的`slist`的结构占用内存$4 + 4 =8bytes$ ，一个指针大小，一个整形变量表示链表的大小。

```cpp
protected:
    link_type       head_; // 头部，对应的是一个指针
    size_type       size_; // 链表大小
```





# slist实现的基本算法

```cpp

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
```





# slist的测试结果

```shell
--构造函数 test--    
[0 0 ]
[1 1 ]
--copy构造 test--    
[1 1 ]
--push()/pop() test--
[2 0 0 ]
[0 0 ]
--front() test--     
0
--empty() test--     
slist is empty       
1
0
--clear() test--
[0 0 ]
slist is empty
--operator== test--
[1 1 ]
[1 1 ]
slist is empty
1
0
--operator< test--
[1 2 ]
1
0
--insert() test--
[1 11 2 ]
--reverse() test--
[2 11 1 ]
--merge() test--
[1 4 ]
[2 3 5 ]
[1 2 3 4 5 ]
slist is empty
```

