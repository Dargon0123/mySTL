/**
 * @file my_stack_test.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-05-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "my_test.h"

#include "../mySTL/container/my_list.h"
#include "../mySTL/adapter/my_stack.h"

namespace mytest {

namespace stack_test {

void stack_test() {
    std::cout << "--构造函数 test--" << std::endl;
    mystl::stack<int> st0(2);
    st0.log_stack();
    mystl::stack<int> st1(st0);
    st1.log_stack();

    std::cout << "--push()/pop() test--" << std::endl;
    st0.pop();
    st0.push(1);
    st0.push(2);
    st0.log_stack();
         
    std::cout << "--top() test--" << std::endl;
    int val0 = st0.top();
    std::cout << val0 << std::endl;

    std::cout << "--empty() test--" << std::endl;
    bool result0 =st0.empty();
    std::cout << result0 << std::endl;
    st1.pop();
    st1.pop();
    bool result1 =st1.empty();
    std::cout << result1 << std::endl;

    std::cout << "--operator== test--" << std::endl;
    mystl::stack<int> st2(1);
    mystl::stack<int> st3(1);
    std::cout << (st2 ==st3) << std::endl;

    // 测试 < 出现问题
    std::cout << "--operator< test--" << std::endl;
    st2.push(1);
    st3.push(1);
    st3.log_stack();
    std::cout << (st2 <st3) << std::endl;
}

}; // end of namespace stack_test

}; // end of namespace mytest

int main() {
    mytest::stack_test::stack_test();

    return 0;
}