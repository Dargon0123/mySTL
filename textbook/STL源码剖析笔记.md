[toc]

# 01 STL介绍

主要是学习`SGI STL `版本 `Silicon Graphics Computer Systems`

# 02 空间配置器 (allocator)

##  `allocator`的结构

* 普通`STL`是简单的`::operator new `和` ::operator delete`的封装

* `SGI`版本的关于`new` 和 `delete`的两阶段操作
  * `new`的话，先调用`::operator new `配置内存，在调用类的构造函数构造对象内容
  * `delete`的话，先调用类的析构函数析构该对象，在调用`::operator delete `释放内存

## 内部的3个`.h`文件，5个全局函数

1. `stl_construct.h`文件： 全局函数，实现`construct`和`destroy`的方式

2. `stl_alloc.h`：配置次配置能力的空间配置器

   1. ```cpp
      // 将一级配置器 typedef 为 malloc_alloc
      // 直接将参数 __inst 指定为0
      typedef __malloc_alloc_template<0> malloc_alloc;
      ```

   2. ```cpp
      // 将二级配置器 typedef 为 alloc vector等默认使用的这个配置器
      // 令 alloc 为第二级配置器
      typedef __default_alloc_template<__NODE_ALLOCATOR_THREADS, 0> alloc; 
      typedef __default_alloc_template<false, 0> single_client_alloc;
      ```

   3. 以分配内存的大小`128bytes`为界限，`>128 bytes`用一级，否则，二级属于SGI STL的内存配置器

3. `stl_uninitialized.h` : 用来初始化的申请的内存空间，多用在后面的容器初始化操作中，定义三个全局函数

   1. `uninitialized_copy()`
   2. `uninitialized_fill()`
   3. `uninitialized_fill_n()`