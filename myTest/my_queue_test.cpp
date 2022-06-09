/**
 * @file my_queue_test.cpp
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
#include "../mySTL/adapter/my_queue.h"
#include "../mySTL/adapter/my_stack.h"

namespace mytest {

namespace queue_test {

void queue_test() {
    std::cout << "--构造函数 test--" << std::endl;
    mystl::queue<int> que0(2);
    que0.log_queue();
    mystl::queue<int> que1(que0);
    que1.log_queue();

    std::cout << "--push()/pop() test--" << std::endl;
    que0.pop();
    que0.push(1);
    que0.push(2);
    que0.log_queue();

    std::cout << "--front() test--" << std::endl;
    int val0 = que0.front();
    std::cout << val0 << std::endl;

    std::cout << "--empty() test--" << std::endl;
    bool result0 =que0.empty();
    std::cout << result0 << std::endl;
    que1.pop();
    que1.pop();
    bool result1 =que1.empty();
    std::cout << result1 << std::endl;

    std::cout << "--operator== test--" << std::endl;
    mystl::queue<int> que2(1);
    mystl::queue<int> que3(1);
    std::cout << (que2 ==que3) << std::endl;

    std::cout << "--operator< test--" << std::endl;
    que2.push(1);
    que3.push(1);
    que3.log_queue();
    std::cout << (que2 <que3) << std::endl;
}

} // end of namespace queue_test

} // end of namespace mytest

int main() {
    mytest::queue_test::queue_test();

    return 0;
}