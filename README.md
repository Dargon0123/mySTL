[toc]

# mySTL

学习`C++ STL`中底层相关的数据结构和算法，深入学习`C++ Template`编程，并实现对应的STL容器以及相关组件

# 环境

* 操作系统 `Win 10/11` 
* 编译器 `Vscode + gcc`

# 主要容器与算法

* `allocator` （空间配置器）
  * `allocator/alloc`（`STL`容器的整个底层内存分配与释放，以字节为单位，供`allocator`类使用）
  * `construct`  （全局函数，实现`construct`和`destroy`的方式）
  * `my_uninitialized`（声明三个全局函数，用于对未初始化空间构造元素，fill or copy 方式）
* `iterator`（迭代器相关，`type_traits`萃取不同型别属性）
* `container`（基本容器）
  * `vector` 向量
  * `list `双向链表
  * `slist`单向链表
  * `stack`
  * `queue`
  * `set`
  * `map`
  * `hash_set`
* `algorithm`（基本常用的算法）
  * `fill_n()` 的函数体
  * `fill(first, last, val)`
  * `copy()`
  * `copy_backward()`

# 文件夹介绍

* `.vscode`环境配置文件

* `exercise`简单测试文件

* `myStl`主要的容器实现`.h`文件

* `myTest`主要的容器测试文件

* `textbook`主要的学习笔记记录

  * [vector笔记内容](https://github.com/Dargon0123/mySTL/blob/master/textbook/my_vector.md)

  * [list笔记内容](https://github.com/Dargon0123/mySTL/blob/master/textbook/my_list.md)
  * [slist笔记内容](https://github.com/Dargon0123/mySTL/blob/master/textbook/my_slist.md)
  * [AVLTree设计](https://github.com/Dargon0123/mySTL/blob/master/textbook/my_AVL_tree.md)
  * [set笔记内容](https://github.com/Dargon0123/mySTL/blob/master/textbook/my_set.md)
  * [map笔记内容](https://github.com/Dargon0123/mySTL/blob/master/textbook/my_map.md)
  * [hashtable笔记内容](https://github.com/Dargon0123/mySTL/blob/master/textbook/my_hash_table.md)
  * [hash_set笔记内容](https://github.com/Dargon0123/mySTL/blob/master/textbook/my_hash_set.md)

* `README`文件

# 容器测试

* 测试平台

  * 操作系统 `Win 10/corei5 10th` 
  * 编译器 `Vscode + gcc`

* 基本`API`测试

  * vector：[my_vector_test](https://github.com/Dargon0123/mySTL/blob/master/myTest/my_vector_test.cpp)
  * list：[list_test](https://github.com/Dargon0123/mySTL/blob/master/myTest/my_list_test.cpp)
  * slist: [slist_test](https://github.com/Dargon0123/mySTL/blob/master/myTest/my_slist_test.cpp)
  * stack：[stack_test](https://github.com/Dargon0123/mySTL/blob/master/myTest/my_stack_test.cpp)
  * queue：[queue_test](https://github.com/Dargon0123/mySTL/blob/master/myTest/my_queue_test.cpp)
  * set：[set_test](https://github.com/Dargon0123/mySTL/blob/master/myTest/my_set_test.cpp)
  * map：[map_test](https://github.com/Dargon0123/mySTL/blob/master/myTest/my_map_test.cpp)
  * hash_set：[hash_set_test](https://github.com/Dargon0123/mySTL/blob/master/myTest/my_hash_set_test.cpp)

* 性能测试

  * vector

    ```shell
    [--------------------- Performance Testing ---------------------]
    |---------------------|-------------|-------------|-------------|
    |        insert       |    100000   |   1000000   |  10000000   |
    |         std         |      3ms    |     26ms    |    256ms    |
    |        mystl        |      2ms    |     22ms    |    255ms    |
    |---------------------|-------------|-------------|-------------|
    ```

  * list

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
    
    ```

    * `std`对应的`sort`使用的是快排的方法，`mystl`对应的`sort`使用的是归并排序的方法。

  * set

    ```shell
    [--------------------- Performance Testing ---------------------]
    |---------------------|-------------|-------------|-------------|
    |        insert       |    100000   |   1000000   |  10000000   |
    |         std         |     47ms    |    386ms    |   3849ms    |
    |        mystl        |     55ms    |    550ms    |   5467ms    |
    |---------------------|-------------|-------------|-------------|
    ```

    * `stl`底层是红黑树，`mystl`底层是AVL树实现的，性能会有一定的差距

  * map

    ```shell
    [--------------------- Performance Testing ---------------------]
    |---------------------|-------------|-------------|-------------|
    |        insert       |    100000   |   1000000   |  10000000   |
    |         std         |     44ms    |    401ms    |   3984ms    |
    |        mystl        |     57ms    |    566ms    |   5537ms    |
    |---------------------|-------------|-------------|-------------|
    ```

    * 和`set`底层是同样的AVL树实现的

  * hash_set

    ```shell
    [--------------------- Performance Testing ---------------------]
    |---------------------|-------------|-------------|-------------|
    |        insert       |    100000   |   1000000   |  10000000   |
    |         std         |     21ms    |    143ms    |   1136ms    |
    |        mystl        |      9ms    |     63ms    |    584ms    |
    |---------------------|-------------|-------------|-------------|
    ```

    

# 参考资料

* [《STL源码剖析》侯捷](https://book.douban.com/subject/1110934/)

* [ MyTinySTL测试框架 ](https://github.com/Alinshans/MyTinySTL/tree/master/Test/)

  
