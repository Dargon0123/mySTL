/**
 * @file my_AVLTree_test.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-06-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <functional>

#include "my_test.h"
#include "../mySTL/container/my_AVL_tree.h"


namespace mytest {

namespace AVL_tree_test {

void AVL_tree_test() {
    std::cout << "--构造函数 test--" << std::endl;
    mystl::AVLTree <int, int, std::_Identity<int>, std::less<int> > itree;
    itree.log_tree();

    std::cout << "--insert_unique() test--" << std::endl;     
    itree.insert_unique(10);
    itree.insert_unique(7); 
    itree.insert_unique(12);
    itree.insert_unique(3);   
    itree.insert_unique(6);
    itree.insert_unique(20);
    itree.log_tree();

    std::cout << "--find() test--" << std::endl;  
    auto ite1 =itree.find(10);
    if (ite1 !=itree.end())
        itree.log_node(ite1.cur);
    ite1 =itree.find(101);
    if (ite1 !=itree.end())
        itree.log_node(ite1.cur); 
    else  
        std::cout << "not find !!" << std::endl; 
    
    std::cout << "--erase() test--" << std::endl; 
    itree.erase(10);
    itree.log_tree();

    std::cout << "--clear() test--" << std::endl; 
    itree.clear();
    itree.log_tree();


}

} // end of AVL_tree_test

} // end of mytest

int main() {
    mytest::AVL_tree_test::AVL_tree_test();
    return 0;
}
