[toc]

# map的数据结构

节点的是`pair<key | data>`的形式

`map`的所有元素都是`pair`的形式存在的，同时拥有实值`value`和键值`key`。`map`根据元素的`key`值进行排序，底层是一颗平衡二叉树`AVL_Tree || RB_Tree`。

# map的迭代器设计

`map`是允许通过迭代器来修改节点的data值，也就是说，通过`map`的迭代器可以改一部分的值，所以它既不是`const_iterator`也不是`mutable_iterator`（可修改的）。

注意此处迭代器并不想 set 一样设置为const_iterator， 因为它允许用户通过迭代器修改元素的value值，key是不能修改的。

# 测试结果

```shell
-构造函数 test--   

--insert() test--  
tree.size()----2   
[[1: a], [2: q], ] 

--operator[] test--
map1[1]: a
map1[10]: o
tree.size()----5
[[1: a], [2: q], [3: p], [5: q], [10: o], ]

--find() test--
cur->height----0
cur->val----[3: p]

--通过迭代器修改value test--
cur->height----0
cur->val----[3: e]

--count() test--
3 count --1

--erase() test--
tree.size()----5
[[1: a], [2: q], [3: e], [5: q], [10: o], ]
tree.size()----4
[[1: a], [2: q], [5: q], [10: o], ]

--clear() test--
tree.size()----0
[]

a1 =1a

a2 =2c

a3 =1a
----------------std::map test------------------
input nums: 100000
mill--seconds: 42
s2.size(): 31196
s2.max_size(): 461168601842738790
----------------mystl::map test------------------
mill--seconds: 55
s3.size(): 31217
s3.max_size(): 18446744073709551615
[--------------------- Performance Testing ---------------------]
|---------------------|-------------|-------------|-------------|
|        insert       |    100000   |   1000000   |  10000000   |
|         std         |     44ms    |    401ms    |   3984ms    |
|        mystl        |     57ms    |    566ms    |   5537ms    |
|---------------------|-------------|-------------|-------------|
[ PASSED ]
[------------------ End container test : map -------------------]

按任意键关闭终端。
```

