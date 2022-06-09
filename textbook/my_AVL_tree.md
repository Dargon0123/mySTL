[toc]

# AVL_tree的迭代器设计

* 迭代器的实现，包括`__AVLTree_iterator`和`__AVLTree_const_iterator`两种类型。迭代器之间的成员变量只有一个，如下

  ```shell
  	// 成员变量
      link_type cur; // 与容器之间产生的一个连接关系
  ```

* 构建成员函数，重载操作符`++i, i++, --i ,i--, *, ->`,等操作符，使该迭代器具有像指针一样的访问元素的能力。
* AVL tree的迭代器属于双向迭代器，不具备随机定位的能力，`++`和`--`的前进或者后退操作行为，完全依据二叉搜索树的节点排列法则进行移动的。

# AVL_tree的节点的数据结构

* `__AVLtree_node`的实现，设计每一个节点包含现在的成员

  ```shell
  value_type val; 	// 节点值
  long long height; 	// 节点高度
  base_ptr left; 		// 左节点
  base_ptr right; 	// 右节点
  base_ptr parent; 	// 父节点
  
  	// 两个成员函数
  	// 求该子树上的最小值
      static base_ptr minmum(base_ptr x) {
          // 一直搜索左子树
          while (x->left) {
              x =x->left;
          }
          return x;
      }
  
      // 求该子树上的最大值
      static base_ptr maximum(base_ptr x) {
          // 一直搜索右子树
          while (x->right) {
              x =x->right;
          }
          return x;
      }
  ```

# AVL_tree的数据结构

对于一颗`AVL`平衡二叉树来说，内部的数据成员定义，以及关于`header`节点的定义分别如下

```shell
private:
    // 一个AVL Tree的占用字节的大小 （4 +4 +1）bytes，
    // 由于内存对齐，总体占用12bytes。

    //关于AVL树中的节点说明：
    //header是AVL树中辅助的头节点，其实值无意义
    //保证其父节点指向根节点root，同时root的父节点也指向header
    //左子节点指向AVL树中最小节点，右子节点指向AVL树中最大节点
    //另：header的height设为-1，此值无意义，但可以用来标识header节点
    //（iterator::operator--()中有用到） 
    size_type node_count;
    node* header;
    Compare key_compare;
```

`AVLtree`的类模板形式

```shell
template <class Key, class Value, class KeyOfValue, 
            class Compare, template<class T> class Alloc =mystl::allocator>
```

# AVL_tree类的实现



# 测试结果

* 测试函数

  ```cpp
  void AVL_tree_test() {
      std::cout << "--构造函数 test--" << std::endl;
      mystl::AVLTree <int, int, std::_Identity<int>, std::less<int> > itree;
      itree.log_tree();
  
      std::cout << "--insert_unique() test--" << std::endl;     
      itree.insert_unique(10);
      itree.insert_unique(7); 
      itree.insert_unique(12);
      itree.insert_unique(3);   
      itree.insert_unique(6);
      itree.insert_unique(20);
      itree.log_tree();
  
      std::cout << "--find() test--" << std::endl;  
      auto ite1 =itree.find(10);
      if (ite1 !=itree.end())
          itree.log_node(ite1.cur);
      ite1 =itree.find(101);
      if (ite1 !=itree.end())
          itree.log_node(ite1.cur); 
      else  
          std::cout << "not find !!" << std::endl; 
      
      std::cout << "--erase() test--" << std::endl; 
      itree.erase(10);
      itree.log_tree();
  
      std::cout << "--clear() test--" << std::endl; 
      itree.clear();
      itree.log_tree();
  
  
  ```

* 测试结果

  ```shell
  --构造函数 test--       
  tree.size()----0        
  []
  --insert_unique() test--
  tree.size()----6        
  [3 6 7 10 12 20 ]       
  --find() test--
  cur->height----2        
  cur->val----10
  not find !!
  --erase() test--        
  tree.size()----5        
  [3 6 7 12 20 ]
  --clear() test--
  tree.size()----0
  []
  ```

  
