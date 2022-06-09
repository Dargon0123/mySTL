/**
 * @file my_map_test.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-06-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <functional>
#include <map>
#include <time.h>

#include "my_test.h"
#include "../mySTL/container/my_map.h"

namespace mytest {

namespace map_test {

void map_test() {
    std::cout << "\n-构造函数 test--" << std::endl;
    mystl::map<int, char> map1;
    std::pair<int, char> v1(1, 'a');
    std::pair<int, char> v2(2, 'q');
    std::cout << "\n--insert() test--" << std::endl;
    map1.insert(v1);
    map1.insert(v2);
    map1.log_map();

    std::cout << "\n--operator[] test--" << std::endl;
    std::cout << "map1[1]: " << map1[1] << std::endl;
    map1[10] ='o';
    std::cout << "map1[10]: " << map1[10] << std::endl;
    map1[3] ='p';
    map1[5] ='q';
    map1.log_map();

    std::cout << "\n--find() test--" << std::endl;
    auto ite1 =map1.find(3);
    map1.log_node(ite1);

    std::cout << "\n--通过迭代器修改value test--" << std::endl;
    (*ite1).second ='e';
    map1.log_node(ite1);

    std::cout << "\n--count() test--" << std::endl;
    std::cout << "3 count --" <<  map1.count(3) << std::endl;

    std::cout << "\n--erase() test--" << std::endl;
    map1.log_map();
    map1.erase(3); // erase 测试未通过, 通过
    map1.log_map();

    std::cout << "\n--clear() test--" << std::endl;
    map1.clear();
    map1.log_map();

    // 测试pair的赋值情况
    std::pair<const int, char> a1 ={1, 'a'};
    std::cout << "\na1 =" << a1.first << a1.second << std::endl;
    std::pair<const int, char> a2 ={2, 'c'};
    std::cout << "\na2 =" << a2.first << a2.second << std::endl;
    // a1 =a2; // 不可以直接进行赋值，错误的原因
    std::pair<const int, char> a3 =a1;
    std::cout << "\na3 =" << a3.first << a3.second << std::endl;

    std::cout << "----------------std::map test------------------" << std::endl;
    std::map<int, char> map2;
    std::cout << "input nums: ";
    int nums;
    std::cin >> nums;
    clock_t start_time =clock();
    for (long i =0; i <nums; ++i) {
        try {
            int val =(int) (((double)rand() / RAND_MAX) * 1000000);
            std::pair<int, char> p1{val, 'c'};
            map2.insert(p1);
        }
        catch(...) {
            std::cout << "i:" << i << " " << std::endl;
            abort();
        }
    }
    std::cout << "mill--seconds: " << clock() -start_time << std::endl;
    std::cout << "s2.size(): " << map2.size() << std::endl;
    std::cout << "s2.max_size(): " << map2.max_size() << std::endl;

    std::cout << "----------------mystl::map test------------------" << std::endl;
    mystl::map<int, char> map3;
    start_time =clock();
    for (long i =0; i <nums; ++i) {
        try {
            int val =(int) (((double)rand() / RAND_MAX) * 1000000);
            std::pair<int, char> p2{val, 'c'};
            map3.insert(p2);
        }
        catch(...) {
            std::cout << "i:" << i << " " << std::endl;
            abort();
        }
    }
    std::cout << "mill--seconds: " << clock() -start_time << std::endl;
    std::cout << "s3.size(): " << map3.size() << std::endl;
    std::cout << "s3.max_size(): " << map3.max_size() << std::endl;


    #if PERFORMANCE_TEST_ON
        std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
        std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
        std::cout << "|        insert       |";
    #if LARGER_TEST_DATA_ON
        MAP_INSERT_TEST(mao, LEN1 _L, LEN2 _L, LEN3 _L);
    #else
        MAP_INSERT_TEST(map, LEN1 _M, LEN2 _M, LEN3 _M);
    #endif
        std::cout << std::endl;
        std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
        PASSED;
    #endif
        std::cout << "[------------------ End container test : map -------------------]" << std::endl;

    // m1[1] =std::string("dar");
    // m1[2] =std::string("wang");
    // m1[3] =std::string("dargon");
}

} // end of namespace map_test 

} // end of namespace mytest 

int main() {

    mytest::map_test::map_test();

    return 0;
}