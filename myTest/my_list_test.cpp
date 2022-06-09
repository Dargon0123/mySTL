/**
 * @file my_list_test.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-05-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef MY_LIST_TEST_H
#define MY_LIST_TEST_H

#include <list>
#include <functional>

#include "my_test.h"
#include "../mySTL/container/my_list.h"

namespace mytest {

namespace list_test {

void list_test() {
    std::cout << "--构造函数 test--" << std::endl;
    mystl::list<int> list0(5);
    list0.log_list();
    mystl::list<int> list1(5,1);
    list1.log_list();

    std::cout << "--拷贝构造 test--" << std::endl;
    mystl::list<int> list2(list1);
    list2.log_list();

    std::cout << "--front/back() test--" << std::endl;
    int val0 =list0.front();
    int val1 =list1.front();
    int val2 =list0.back();
    int val3 =list1.back();
    std::cout << val0 << " " << val1 << " "
            << val2 << " " << val3 << " "
            << std::endl;

    std::cout << "--clear() test--" << std::endl;
    list1.clear();
    list1.log_list();

    // Note: 我们不能从int --> const int，但是，反之则可以
    std::cout << "--erase() test--" << std::endl;
    mystl::list<int>::const_iterator ite =list2.cbegin();
    std::cout << *ite << std::endl;
    mystl::list<int>::iterator ite2 =list2.erase(ite);
    std::cout << *ite2 << std::endl;
    list2.log_list();
    list2.erase(list2.cbegin(), list2.cend());
    list2.log_list();

    std::cout << "--push_back/front()  test--" << std::endl;
    mystl::list<int> list3(1, 1);
    list3.push_back(2);
    list3.push_back(3);
    list3.push_back(4);
    list3.push_front(0);
    list3.log_list();
    list3.pop_back();
    list3.pop_front();
    list3.log_list();
    std::cout << list3.size() << std::endl;

    std::cout << "--insert() test--" << std::endl;
    list3.insert(list3.cbegin(), 10);
    list3.log_list();

    std::cout << "--splice() test--" << std::endl;    
    list0.push_back(11);
    list0.log_list();
    list3.splice(list3.cbegin(), list0);
    list3.log_list();
    list0.push_back(1);
    list0.push_back(2);
    list0.log_list();
    list3.splice(list3.cbegin(), list0, list0.cbegin());
    list3.log_list();
    list0.push_front(1);
    list0.log_list();
    list3.splice(list3.cend(), list0, list0.cbegin(), list0.cend());
    list3.log_list();

    std::cout << "--unique() test--" << std::endl; 
    list3.unique_v2();
    list3.log_list();

    std::cout << "--remove() test--" << std::endl; 
    list3.remove(1);
    list3.log_list();

    std::cout << "--merge() test--" << std::endl;
    list0.push_back(1);
    list0.push_back(3);
    list0.push_back(5);
    list0.log_list();
    mystl::list<int> list4(1,2);
    list4.push_back(4);
    list4.log_list();
    list0.merge(list4);
    list0.log_list();

    std::cout << "--merge_sort() test--" << std::endl;
    list0.remove(2);
    list0.remove(3);   
    list0.push_back(2);
    list0.push_back(3);
    list0.log_list();
    list0.merge_sort();
    list0.log_list();

    std::cout << "--reverse() test--" << std::endl;
    list0.reverse();
    list0.log_list();

    // 测试insert
    #if PERFORMANCE_TEST_ON
        std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
        std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
        std::cout << "|        insert       |";
    #if LARGER_TEST_DATA_ON
        CON_TEST_P2(vector<int>, insert, cend, rand(), LEN1 _L, LEN2 _L, LEN3 _L);
    #else
        CON_TEST_P2(list<int>, insert, cend, rand(), LEN1 _M, LEN2 _M, LEN3 _M);
    #endif
        std::cout << std::endl;
        std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
        PASSED;
    #endif
        std::cout << "[------------------ End container test : list -------------------]" << std::endl;

    // 测试sort
    #if PERFORMANCE_TEST_ON
        std::cout << std::endl;
        std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
        std::cout << "|         sort        |";
    #if LARGER_TEST_DATA_ON
        LIST_SORT_TEST(LEN1 _M, LEN2 _M, LEN3 _M);
    #else
        LIST_SORT_TEST(LEN1 _S, LEN2 _S, LEN3 _S);
    #endif
        std::cout << std::endl;
        std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
        PASSED;
    #endif
        std::cout << "[------------------ End container test : list ------------------]" << std::endl;
    

}


} // end of namespace vector_test

} // end of namespace mytest

int main() {
    mytest::list_test::list_test();

    return 0;
}


#endif