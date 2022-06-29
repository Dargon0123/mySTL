/**
 * @file my_hash_set_test.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-06-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <functional>

#include "my_test.h"
#include "../mySTL/container/my_hashtable.h"
#include "../mySTL/container/my_hash_set.h"
#include "../mySTL/allocator/my_allocator.h"
#include <unordered_set>

namespace mytest {

namespace hash_set_test {

void hash_set_test() {
    std::cout << "--insert_unique() test--" << std::endl;
    mystl::hash_set<int> s1; // 底层默认是100个
    s1.insert(3);
    s1.insert(59);
    s1.insert(63);
    s1.insert(108);
    s1.insert(53);
    s1.insert(55);

    std::cout << "--logHashtable() test--" << std::endl; 
    s1.logSet();

    std::cout << "--size()/bucket_count() test--" << std::endl;
    std::cout << "--s1.size()--" << s1.size() << std::endl;
    std::cout << "--s1.bucket_count()--" << s1.bucket_count() << std::endl;
    
    std::cout << "--erase() test--" << std::endl;
    s1.erase(3);
    s1.logSet();

    
    std::cout << "--find() test--" << std::endl;
    std::cout << *(s1.find(53)) << std::endl; //2
    std::cout << s1.count(53) << std::endl; //2

    #if PERFORMANCE_TEST_ON
        std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
        std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
        std::cout << "|        insert       |";
    #if LARGER_TEST_DATA_ON
        SET_TEST_P1(hash_set<int>, insert, rand(), LEN1 _L, LEN2 _L, LEN3 _L);
    #else
        SET_TEST_P1(hash_set<int>, insert, rand(), LEN1 _M, LEN2 _M, LEN3 _M);
    #endif
        std::cout << std::endl;
        std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
        PASSED;
    #endif
        std::cout << "[------------------ End container test : hash_set -------------------]" << std::endl;

}    

} // end of namespace hash_set_test

} // end of namespace mytest

int main () {
    mytest::hash_set_test::hash_set_test();
    return 0;
}