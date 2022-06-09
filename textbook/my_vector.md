[toc]

# vector 的迭代器设计



# vector 的数据结构



# vector 的具体实现



# 测试结果

* 测试结果

  ```shell
  
  构造函数 test
  [2 2 2 2 2 ]
  拷贝构造 test
  [2 2 2 2 2 ]
  [0 1 2 3 4 ]
  [0 1 ]
  重载运算符"=" test      
  [0 1 2 3 4 ]
  [0 1 2 3 4 ]
  迭代器iterator test     
  2 2 2 2 2
  访问元素相关[]/at() test
  2 2
  2 2
  容量相关size() test
  v1.size: 5
  v1.capacity: 5
  修改容器操作push_back()/pop_back() test
  [2 2 2 2 2 5 6 ]
  [2 2 2 2 2 5 ]
  [2 2 2 2 ]
  v1.size: 4
  v1.capacity: 10
  [2 2 2 2 5 6 ]
  容量相关erase()/clear() test
  [2 2 2 5 6 ]
  v1.size: 0
  v1.capacity: 10
  修改容器操作insert() test
  [10 ]
  [8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 10 ]
  修改容器操作range_insert() test
  v1.size: 21
  v1.capacity: 21
  [0 1 2 3 4 ]
  [8 0 1 2 3 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 10 ]
  v1.size: 25
  v1.capacity: 42
  修改容器操作shrink() test
  v1.size: 25
  v1.capacity: 25
  修改容器操作reserve() test
  v1.size: 25
  v1.capacity: 40
  [--------------------- Performance Testing ---------------------]
  |---------------------|-------------|-------------|-------------|
  |        insert       |    100000   |   1000000   |  10000000   |
  |         std         |      3ms    |     26ms    |    256ms    |
  |        mystl        |      2ms    |     22ms    |    255ms    |
  |---------------------|-------------|-------------|-------------|
  [ PASSED ]
  [------------------ End container test : set -------------------]
  ```

  