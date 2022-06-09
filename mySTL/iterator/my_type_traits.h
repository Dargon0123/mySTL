/**
 * @file my_type_traits.h
 * @author your name (you@domain.com)
 * @brief type_traits class 针对不同型别属性，编译时期
 * 决定使用哪个函数，更加高效
 * @version 0.1
 * @date 2022-03-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __MY_TYPE_TRAITS_H
#define __MY_TYPE_TRAITS_H

#include <type_traits>

namespace mystl {

// 编译器只有面对 class object 形式的参数
struct __true_type { };
struct __false_type { };

// 类型萃取，应用都是最保守的值
// POD = plain old data 朴素无实的数据
template <class type>
struct __type_traits {
    typedef __true_type     this_dummy_member_must_be_first;
    typedef __false_type    has_trivial_default_constructor;
    typedef __false_type    has_trivial_copy_constructor;
    typedef __false_type    has_trivial_assignment_operator;
    typedef __false_type    has_trivial_destructor;
    typedef __false_type    is_POD_type;
};

// 后面进行 一般具体化（general instantiation）

} // end of namespace mystl

#endif