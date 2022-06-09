[toc]

# list 的迭代器类型

`list`提供的是一个双向链表，迭代器必须具备前移和后移的能力，迭代器提供的是 `Bidirectional iterations`

运用自己设计的迭代器，重载作为一个指针应该用到的基本操作，重载

```shell
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
```

# list 的数据结构

双向链表，准确的说是环形双向链表

# list 的构造，拷贝，析构函数设计

* 构造：先申请内存`get_node()`，再进行构造`creat_node()`
* 析构：先调用析构`destroy()`，再释放内存`put_node()`

# list实现的基本算法

* ` push_back/front, pop_back/front`在`list`的首尾进行单个插入
* `clear and erase`操作
* `insert `操作, 在pos 处插入value值的元素
* `splice` 针对`list`结合操作
* `remove `节点操作
  * 使用`lambda `的隐式的引用捕获方式
* `unique`节点, 移除数值相同的连续元素，只剩一个元素
* `merge `节点，合并链表到 该链表上面
* `sort `节点 ,使用归并排序
* `reverse` 节点 ,翻转该`list`

# list的测试结果

```shell
--构造函数 test--    
[0 0 0 0 0 ]
[1 1 1 1 1 ]
--拷贝构造 test--    
[1 1 1 1 1 ]
--front/back() test--
0 1 0 1
--clear() test--
list is empty
--erase() test--
1
1
[1 1 1 1 ]
list is empty
--push_back/front()  test--
[0 1 2 3 4 ]
[1 2 3 ]
3
--insert() test--
[10 1 2 3 ]
--splice() test--
[0 0 0 0 0 11 ]
[0 0 0 0 0 11 10 1 2 3 ]
[1 2 ]
[1 0 0 0 0 0 11 10 1 2 3 ]
[1 2 ]
[1 0 0 0 0 0 11 10 1 2 3 1 2 ]
--unique() test--
[1 0 11 10 1 2 3 1 2 ]
--remove() test--
[0 11 10 2 3 2 ]
--merge() test--
[1 3 5 ]
[2 4 ]
[1 2 3 4 5 ]
--merge_sort() test--
[1 4 5 2 3 ]
[1 2 3 4 5 ]
--reverse() test--
[5 4 3 2 1 ]
```

对比于`std::list`的测试，`inset`和`sort`排序

```shell
[--------------------- Performance Testing ---------------------]
|---------------------|-------------|-------------|-------------|
|        insert       |    100000   |   1000000   |  10000000   |
|         std         |     10ms    |    101ms    |    993ms    |
|        mystl        |      5ms    |     52ms    |    516ms    |
|---------------------|-------------|-------------|-------------|
[ PASSED ]
[------------------ End container test : list -------------------]

|---------------------|-------------|-------------|-------------|
|         sort        |     20000   |    200000   |   2000000   |
|         std         |     11ms    |    121ms    |   1706ms    |
|        mystl        |      5ms    |     73ms    |   1120ms    |
|---------------------|-------------|-------------|-------------|
[ PASSED ]
[------------------ End container test : list ------------------]
```

`std`对应的`sort`使用的是快排的方法，`mystl`对应的`sort`使用的是归并排序的方法。

