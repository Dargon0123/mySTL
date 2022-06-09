/**
 * @file my_slist_test.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-05-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "my_test.h"

#include "../mySTL/container/my_slist.h"

namespace mytest {

namespace slist_test {

void slist_test() {
    std::cout << "--构造函数 test--" << std::endl;
    mystl::slist<int> s0(2);
    s0.log_slist();
    mystl::slist<int> s1(2, 1);
    s1.log_slist();

    std::cout << "--copy构造 test--" << std::endl;
    mystl::slist<int> s3(s1);
    s3.log_slist();


    std::cout << "--push()/pop() test--" << std::endl;
    s0.push_front(2);
    s0.log_slist();
    s0.pop_front();
    s0.log_slist();


    std::cout << "--front() test--" << std::endl;
    auto v0 =s0.front();
    std::cout << v0 << std::endl;


    std::cout << "--empty() test--" << std::endl;
    mystl::slist<int> s2;
    s2.log_slist();
    std::cout << s2.empty() << std::endl;
    std::cout << s0.empty() << std::endl;


    std::cout << "--clear() test--" << std::endl;
    s0.log_slist();
    s0.clear();
    s0.log_slist();
    

    std::cout << "--operator== test--" << std::endl;
    s1.log_slist();
    s3.log_slist();
    s2.log_slist();
    std::cout << (s1 ==s3) << std::endl;
    std::cout << (s1 ==s2) << std::endl;

    std::cout << "--operator< test--" << std::endl;
    mystl::slist<int> s4(1,2);
    s4.push_front(1);
    s4.log_slist();
    std::cout << (s1 <s4) << std::endl;
    std::cout << (s1 <s3) << std::endl;

    std::cout << "--insert() test--" << std::endl;
    s4.insert(s4.cbegin(), 11);
    s4.log_slist();

    std::cout << "--reverse() test--" << std::endl;
    s4.reverse();
    s4.log_slist();

    std::cout << "--merge() test--" << std::endl;
    mystl::slist<int> s5;
    s5.push_front(4);
    s5.push_front(1);
    mystl::slist<int> s6;
    s6.push_front(5);
    s6.push_front(3);
    s6.push_front(2);
    s5.log_slist();
    s6.log_slist();
    
    s5.merge(s6);
    s5.log_slist();
    s6.log_slist();


}

} // end of namespace slist_test    

} // end of namespace mytest

int main() {
    mytest::slist_test::slist_test();

    return 0;
}