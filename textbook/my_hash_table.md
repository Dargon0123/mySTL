[toc]

# hash_table的整体设计

* 特点
  * `vector<node* > `作为`buckets`,相当于`vector`元素是节点的指针，一个元素相当于一个`bucket`.
  * 一个`bucket`里面都是一串节点，连接形成单向链表。

# hash_table的节点的设计



# hash_table的迭代器设计

* 提供常规的`iterator`和`const_iterator`，只提供`forward_iterator_tag`只有前进的操作，没有后退的操作

# hash_table的基本算法设计

* `insert`操作
  * 所插入的`obj`都是在当前的所处的`bucket`**第一位置**进行插入的。
* `resize`操作
  * 当`hashtable`里面的节点元素 等于 `buckets`的个数的时候，就会进行`rehashing`，操作也即是将`bucket`的数量增加到下一个`stl_prime`上面。
  * 当插入的时候，先进行`size`的检查，元素个数超载了就要进行`rehashing`操作，重新迁移到新`hash_table`里面的时候，每一个旧的`bucket`都要倒序插入到新的映射的`bucket`里面。



# hash_table的测试结果

```shell
--构造函数 test--
--iht.size()--0
--iht.bucket_count()--53
--iht.max_bucket_count()--4294967291
--insert_unique() test--
6
--logHashtable() test--
53 55 2 108 59 63
--elems_in_bucket() test--
bucket[0] has 1 elems.
bucket[2] has 3 elems.
bucket[6] has 1 elems.
bucket[10] has 1 elems.
--resize() test--
54
97
bucket[0] has 1 elems.
bucket[1] has 1 elems.
bucket[2] has 2 elems.
bucket[3] has 1 elems.
bucket[4] has 1 elems.
bucket[5] has 1 elems.
bucket[6] has 1 elems.
bucket[7] has 1 elems.
bucket[8] has 1 elems.
bucket[9] has 1 elems.
bucket[10] has 1 elems.
bucket[11] has 2 elems.
bucket[12] has 1 elems.
bucket[13] has 1 elems.
bucket[14] has 1 elems.
bucket[15] has 1 elems.
bucket[16] has 1 elems.
bucket[17] has 1 elems.
bucket[18] has 1 elems.
bucket[19] has 1 elems.
bucket[20] has 1 elems.
bucket[21] has 1 elems.
bucket[22] has 1 elems.
bucket[23] has 1 elems.
bucket[24] has 1 elems.
bucket[25] has 1 elems.
bucket[26] has 1 elems.
bucket[27] has 1 elems.
bucket[28] has 1 elems.
bucket[29] has 1 elems.
bucket[30] has 1 elems.
bucket[31] has 1 elems.
bucket[32] has 1 elems. 
bucket[33] has 1 elems.
bucket[34] has 1 elems.
bucket[35] has 1 elems.
bucket[36] has 1 elems.
bucket[37] has 1 elems.
bucket[38] has 1 elems.
bucket[39] has 1 elems.
bucket[40] has 1 elems.
bucket[41] has 1 elems.
bucket[42] has 1 elems.
bucket[43] has 1 elems.
bucket[44] has 1 elems.
bucket[45] has 1 elems.
bucket[46] has 1 elems.
bucket[47] has 1 elems.
bucket[53] has 1 elems.
bucket[55] has 1 elems.
bucket[59] has 1 elems.
bucket[63] has 1 elems.
0 1 2 2 3 4 5 6 7 8 9 10 11 108 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 53 55 59 63
--find() test--
2
2
true
```

