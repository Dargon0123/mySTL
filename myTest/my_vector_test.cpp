/**
 * @file my_vector_test.cpp
 * @author your name (you@domain.com)
 * @brief 测试my_vector 的接口与性能表现
 * @version 0.1
 * @date 2022-03-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __MY_VECTOR_TEST_H
#define __MY_VECTOR_TEST_H

#include "my_test.h"
#include "../mySTL/container/my_vector.h"


namespace mytest {

namespace vector_test {

void vector_test() {
    std::cout << "构造函数 test" << std::endl;
    mystl::vector<int> v0(5, 2);
    mytest::log(v0);
    std::cout << "拷贝构造 test" << std::endl;
    mystl::vector<int> v1(v0);
    mytest::log(v1);
    int nums[5] ={0, 1, 2, 3, 4};
    mystl::vector<int> v2(nums, nums +5);
    mytest::log(v2);
    mystl::vector<int> v3(nums, nums +2);
    mytest::log(v3);

    std::cout << "重载运算符\"=\" test" << std::endl;
    v3 =v2;
    mytest::log(v3);
    mystl::vector<int> v4 =v2;
    mytest::log(v4);

    std::cout << "迭代器iterator test" << std::endl;
    for (auto it =v1.begin(); it !=v1.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    std::cout << "访问元素相关[]/at() test" << std::endl;
    std::cout << v1[1] << " " << v1[2] << std::endl;
    std::cout << v1.at(1) << " " << v1.at(2) << std::endl;

    std::cout << "容量相关size() test" << std::endl;
    std::cout << "v1.size: " << v1.size() << std::endl;
    std::cout << "v1.capacity: " << v1.capacity() << std::endl;

    std::cout << "修改容器操作push_back()/pop_back() test" << std::endl;
    v1.push_back(5);
    v1.push_back(6);
    mytest::log(v1);
    v1.pop_back();
    mytest::log(v1);
    v1.pop_back();
    v1.pop_back();
    mytest::log(v1);
    std::cout << "v1.size: " << v1.size() << std::endl;
    std::cout << "v1.capacity: " << v1.capacity() << std::endl;
    v1.push_back(5);
    v1.push_back(6);
    mytest::log(v1);

    std::cout << "容量相关erase()/clear() test" << std::endl;
    auto it =v1.erase(v1.begin() +0);
    mytest::log(v1);
    v1.clear();
    mytest::log(v1);
    std::cout << "v1.size: " << v1.size() << std::endl;
    std::cout << "v1.capacity: " << v1.capacity() << std::endl;

    std::cout << "修改容器操作insert() test" << std::endl;
    v1.insert(v1.begin(), 10);
    mytest::log(v1);
    v1.insert(v1.begin(), (size_t)20, 8);
    mytest::log(v1);

    std::cout << "修改容器操作range_insert() test" << std::endl;
    std::cout << "v1.size: " << v1.size() << std::endl;
    std::cout << "v1.capacity: " << v1.capacity() << std::endl;
    mytest::log(v2);
    v1.insert(v1.begin() +1, v2.begin(), v2.begin() +4);
    mytest::log(v1);
    std::cout << "v1.size: " << v1.size() << std::endl;
    std::cout << "v1.capacity: " << v1.capacity() << std::endl;

    std::cout << "修改容器操作shrink() test" << std::endl;
    v1.shrink_to_fit();
    std::cout << "v1.size: " << v1.size() << std::endl;
    std::cout << "v1.capacity: " << v1.capacity() << std::endl;

    std::cout << "修改容器操作reserve() test" << std::endl;
    v1.reserve(40);
    std::cout << "v1.size: " << v1.size() << std::endl;
    std::cout << "v1.capacity: " << v1.capacity() << std::endl;


    #if PERFORMANCE_TEST_ON
        std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
        std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
        std::cout << "|        insert       |";
    #if LARGER_TEST_DATA_ON
        CON_TEST_P1(vector<int>, push_back, rand(), LEN1 _L, LEN2 _L, LEN3 _L);
    #else
        CON_TEST_P1(vector<int>, push_back, rand(), LEN1 _M, LEN2 _M, LEN3 _M);
    #endif
        std::cout << std::endl;
        std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
        PASSED;
    #endif
        std::cout << "[------------------ End container test : set -------------------]" << std::endl;
}



} // end of namespace vector_test

} // end of namespace mytest

int main() {
    mytest::vector_test::vector_test();

    return 0;
}



#endif