/**
 * @file my_hashtable_test.cpp
 * @author your name (you@domain.com)
 * @brief test hashtable
 * @version 0.1
 * @date 2022-06-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <functional>

#include "my_test.h"
#include "../mySTL/container/my_hashtable.h"
#include "../mySTL/allocator/my_allocator.h"


namespace mytest {

namespace hashtable_test {

void hashtable_test() {
    std::cout << "--构造函数 test--" << std::endl;
    mystl::hashtable<int, 
                     int, 
                     std::hash<int>, 
                     std::_Identity<int>, 
                     std::equal_to<int>
                        > 
                     iht(50, std::hash<int>(), std::equal_to<int>()); // 指定保留元素50个bucket(桶)
    std::cout << "--iht.size()--" << iht.size() << std::endl;
    std::cout << "--iht.bucket_count()--" << iht.bucket_count() << std::endl;
    std::cout << "--iht.max_bucket_count()--" << iht.max_bucket_count() << std::endl;

    std::cout << "--insert_unique() test--" << std::endl; 
    iht.insert_unique(59);
    iht.insert_unique(63);
    iht.insert_unique(108);
    iht.insert_unique(2);
    iht.insert_unique(53);
    iht.insert_unique(55);
    std::cout << iht.size() << std::endl;                //6,此即hashtable<T>::num_elements

    std::cout << "--logHashtable() test--" << std::endl; 
    iht.logHashtable();

    std::cout << "--elems_in_bucket() test--" << std::endl;
    for (int i =0; i <iht.bucket_count(); ++i) {
        int n =iht.elems_in_bucket(i);
        if (n) {
            std::cout << "bucket[" << i << "] has " << n << " elems. " << std::endl;
        }
    }

    std::cout << "--resize() test--" << std::endl;
    for(int i = 0; i <=47;i++) {
        iht.insert_equal(i);
    }        
    std::cout << iht.size() << std::endl;        //54
    std::cout << iht.bucket_count() << std::endl;    //97确实扩容了(重建)

    for (int i =0; i <iht.bucket_count(); ++i) {
        int n =iht.elems_in_bucket(i);
        if (n) {
            std::cout << "bucket[" << i << "] has " << n << " elems. " << std::endl;
        }
    }
    iht.logHashtable();

    std::cout << "--find() test--" << std::endl;
    std::cout << *(iht.cfind(2)) << std::endl; //2
    std::cout << iht.count(2) << std::endl; //2

    mystl::hashtable<int, 
                    int, 
                    std::hash<int>, 
                    std::_Identity<int>, 
                    std::equal_to<int>
                    > iht2(50,std::hash<int>(), std::equal_to<int>());    //指定保留50个bucket(桶)

    std::cout << (iht!=iht2 ? "true" : "false") << std::endl;

}


} // end of hashtable test

} // end of mytest


int main() {
    mytest::hashtable_test::hashtable_test();
    return 0;
}