/**
 * @file my_set_test.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-06-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <functional>
#include <set>
#include <time.h>

#include "my_test.h"
#include "../mySTL/container/my_set.h"

namespace mytest {

namespace set_test {

void set_test() {
    std::cout << "--构造函数 test--" << std::endl;
    int i;
    int ia[5] ={0,1,2,3,4};
    mystl::set<int> s1(ia, ia +5);
    s1.log_set();

    std::cout << "--insert() test--" << std::endl;
    s1.insert(7);
    s1.insert(8);
    s1.log_set();

    std::cout << "--count() test--" << std::endl;
    std::cout << "3 count --" <<  s1.count(3) << std::endl;

    std::cout << "--erase() test--" << std::endl;
    s1.erase(3); // 测试通过
    std::cout << "3 count --" <<  s1.count(3) << std::endl;

    std::cout << "--find() test--" << std::endl;
    auto ite1 =s1.find(1);
    std::cout << "*ite1--" << *ite1 << std::endl;
    s1.log_set();
           
    std::cout << "--clear() test--" << std::endl;
    s1.clear();
    s1.log_set();
    
    std::cout << "----------------std::set test------------------" << std::endl;
    std::set<int> s2;
    std::cout << "input nums: ";
    int nums;
    std::cin >> nums;
    clock_t start_time =clock();
    for (long i =0; i <nums; ++i) {
        try {
            int val =(int) (((double)rand() / RAND_MAX) * 1000000);
            s2.insert(val);
        }
        catch(...) {
            std::cout << "i:" << i << " " << std::endl;
            abort();
        }
    }
    std::cout << "mill--seconds: " << clock() -start_time << std::endl;
    std::cout << "s2.size(): " << s2.size() << std::endl;
    std::cout << "s2.max_size(): " << s2.max_size() << std::endl;

    std::cout << "----------------mystl::set test------------------" << std::endl;
    mystl::set<int> s3;
    start_time =clock();
    for (long i =0; i <nums; ++i) {
        try {
            int val =(int) (((double)rand() / RAND_MAX) * 1000000);
            s3.insert(val);
        }
        catch(...) {
            std::cout << "i:" << i << " " << std::endl;
            abort();
        }
    }
    std::cout << "mill--seconds: " << clock() -start_time << std::endl;
    std::cout << "s3.size(): " << s3.size() << std::endl;
    std::cout << "s3.max_size(): " << s3.max_size() << std::endl;
    
    #if PERFORMANCE_TEST_ON
        std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
        std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
        std::cout << "|        insert       |";
    #if LARGER_TEST_DATA_ON
        CON_TEST_P1(set<int>, insert, rand(), LEN1 _L, LEN2 _L, LEN3 _L);
    #else
        CON_TEST_P1(set<int>, insert, rand(), LEN1 _M, LEN2 _M, LEN3 _M);
    #endif
        std::cout << std::endl;
        std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
        PASSED;
    #endif
        std::cout << "[------------------ End container test : set -------------------]" << std::endl;

}

} // end of namespace set_test

} // end of namespace mytest

int main() {
    mytest::set_test::set_test();

    return 0;
}