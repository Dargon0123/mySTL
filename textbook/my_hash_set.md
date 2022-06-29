[toc]

# hash_set的整体设计

* `hash_set`的底层是一个`hashtable`，同时，`set`的各种功能调用也即是调用`hashtable`的函数。
* 是不进行排序的对于数据来说，默认`bucekts`大小时100个

# hash_set的测试结果

```shell
--insert_unique() test--      
--logHashtable() test--       
3 53 55 59 63 108
--size()/bucket_count() test--
--s1.size()--6
--s1.bucket_count()--193      
--erase() test--
53 55 59 63 108
--find() test--
53
1
[--------------------- Performance Testing ---------------------]
|---------------------|-------------|-------------|-------------|
|        insert       |    100000   |   1000000   |  10000000   |
|         std         |     21ms    |    143ms    |   1136ms    |
|        mystl        |      9ms    |     63ms    |    584ms    |
|---------------------|-------------|-------------|-------------|
[ PASSED ]
[------------------ End container test : hash_set -------------------]
```

* 注，std整体的功能设计比较全面，我是简单设计插入操作，中间扩展的时候使用的二分法进行查找下一个数。

