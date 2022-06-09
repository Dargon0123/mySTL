#include <iostream>
#include <vector>

#include "../mySTL/container/my_vector.h"
#include "../myTest/my_test.h"


namespace mytest {

template<class T>
void log(T & ans) {
    if (!ans.size()) return;
    for (auto val : ans) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

} // end of myetst

int main()
{
    std::cout << "Hello,VSCode!" << std::endl;


    return 0;
}  