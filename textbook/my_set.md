[toc]

# set 的数据结构

`set`的根据所有元素的键值`key`自动排序，底层使用一个`AVL_tree`，`set`所开放的各种操作接口，都是直接调用的`AVL_tree`的接口。

# set的性能测试

* 测试结果

  ```cpp
  
  --构造函数 test--
  tree.size()----5 
  [0 1 2 3 4 ]     
  --insert() test--
  tree.size()----7 
  [0 1 2 3 4 7 8 ] 
  --count() test-- 
  3 count --1      
  --erase() test-- 
  3 count --0      
  --find() test--  
  *ite1--1
  tree.size()----6 
  [0 1 2 4 7 8 ]   
  --clear() test--
  tree.size()----0
  []
  ----------------std::set test------------------
  input nums: 10000
  mill--seconds: 6
  s2.size(): 8628
  s2.max_size(): 461168601842738790
  ----------------mystl::set test------------------
  mill--seconds: 7
  s3.size(): 8614
  s3.max_size(): 18446744073709551615
  [--------------------- Performance Testing ---------------------]
  |---------------------|-------------|-------------|-------------|
  |        insert       |    100000   |   1000000   |  10000000   |
  |         std         |     47ms    |    386ms    |   3849ms    |
  |        mystl        |     55ms    |    550ms    |   5467ms    |
  |---------------------|-------------|-------------|-------------|
  [ PASSED ]
  [------------------ End container test : set -------------------]
  
  按任意键关闭终端。
  
  ```

