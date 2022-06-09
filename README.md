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
* `algorithm`（基本常用的算法）
  * `fill_n()` 的函数体
  * `fill(first, last, val)`
  * `copy()`
  * `copy_backward()`

# 参考资料

* [《STL源码剖析》侯捷](https://book.douban.com/subject/1110934/)
* [MyTinySTL测试框架]([MyTinySTL/Test at master · Alinshans/MyTinySTL (github.com)](https://github.com/Alinshans/MyTinySTL/tree/master/Test))
