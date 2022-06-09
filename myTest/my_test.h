/**
 * @file my_test.h
 * @author your name (you@domain.com)
 * @brief 包含一些test 使用的函数模板
 * @version 0.1
 * @date 2022-03-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __MY_TEST_H
#define __MY_TEST_H

#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip> //setw

namespace mytest {

// 针对一维数组，string形式
template<class T>
void log(T & ans) {
    if (!ans.size()) return;
    std::cout << "[";
    for (auto val : ans) {
        std::cout << val << " ";
    }
    std::cout << "]" << std::endl;
}

// 针对二维数组，string形式
template<class T>
void logV2( T& ans) {
    std::cout << "[" << std::endl;
    for (auto nums : ans) {
        std::cout << "[";
        for (auto val : nums) {
            std:: cout << val << " ";
        }
        std::cout << "]" << std::endl;
    }
    std::cout << "]" << std::endl;
}


/**********************************************************************
 * 关于性能测试，常用的宏定义
 *********************************************************************/
// 是否开启性能测试
#ifndef PERFORMANCE_TEST_ON
#define PERFORMANCE_TEST_ON 1
#endif // !PERFORMANCE_TEST_ON

// 是否开启大数据量测试
#ifndef LARGER_TEST_DATA_ON
#define LARGER_TEST_DATA_ON 0
#endif // !LARGER_TEST_DATA_ON

// 不同情况的测试数量级
#if defined(_DEBUG) || defined(DEBUG)
#define LEN1    10000
#define LEN2    100000
#define LEN3    1000000
#else
#define LEN1    100000
#define LEN2    1000000
#define LEN3    10000000
#endif

// 关于数据量倍数的扩增
#define _LLL * 20
#define _LL  * 10
#define _L   * 5
#define _M
#define _S   / 5
#define _SS  / 10
#define _SSS / 20

#define WIDE    14

// 输出通过提示
#define PASSED    std::cout << "[ PASSED ]\n"

// 输出测试数量级
void test_len(size_t len1, size_t len2, size_t len3, size_t wide)
{
  std::string str1, str2, str3;
  std::stringstream ss;
  ss << len1 << " " << len2 << " " << len3;
  ss >> str1 >> str2 >> str3;
  str1 += "   |";
  std::cout << std::setw(wide) << str1;
  str2 += "   |";
  std::cout << std::setw(wide) << str2;
  str3 += "   |";
  std::cout << std::setw(wide) << str3 << "\n";
}

#define TEST_LEN(len1, len2, len3, wide) \
  test_len(len1, len2, len3, wide)

// 常用测试性能的宏
#define FUN_TEST_FORMAT1(mode, fun, arg, count) do {         \
  srand((int)time(0));                                       \
  clock_t start, end;                                        \
  mode c;                                                    \
  char buf[10];                                              \
  start = clock();                                           \
  for (size_t i = 0; i < count; ++i)                         \
    c.fun(arg);                                              \
  end = clock();                                             \
  int n = static_cast<int>(static_cast<double>(end - start)  \
      / CLOCKS_PER_SEC * 1000);                              \
  std::snprintf(buf, sizeof(buf), "%d", n);                  \
  std::string t = buf;                                       \
  t += "ms    |";                                            \
  std::cout << std::setw(WIDE) << t ;                        \
} while(0)

#define FUN_TEST_FORMAT2(mode, fun, arg1, arg2, count) do {  \
  srand((int)time(0));                                       \
  clock_t start, end;                                        \
  mode c;                                                    \
  char buf[10];                                              \
  start = clock();                                           \
  for (size_t i = 0; i < count; ++i)                         \
    c.fun(c.arg1(), arg2);                                   \
  end = clock();                                             \
  int n = static_cast<int>(static_cast<double>(end - start)  \
      / CLOCKS_PER_SEC * 1000);                              \
  std::snprintf(buf, sizeof(buf), "%d", n);                  \
  std::string t = buf;                                       \
  t += "ms    |";                                            \
  std::cout << std::setw(WIDE) << t;                         \
} while(0)

// map insert
#define MAP_INSERT_DO_TEST(mode, con, count) do {           \
  srand((int)time(0));                                       \
  clock_t start, end;                                        \
  mode::con<int, int> c;                                     \
  char buf[10];                                              \
  start = clock();                                           \
  for (size_t i = 0; i < count; ++i)                         \
    c.insert(std::make_pair(rand(), rand()));              \
  end = clock();                                             \
  int n = static_cast<int>(static_cast<double>(end - start)  \
      / CLOCKS_PER_SEC * 1000);                              \
  std::snprintf(buf, sizeof(buf), "%d", n);                  \
  std::string t = buf;                                       \
  t += "ms    |";                                            \
  std::cout << std::setw(WIDE) << t;                         \
} while(0)

// sort
#define LIST_SORT_DO_TEST(mode, count) do {                  \
  srand((int)time(0));                                       \
  clock_t start, end;                                        \
  mode::list<int> l;                                         \
  char buf[10];                                              \
  for (size_t i = 0; i < count; ++i)                         \
    l.insert(l.cend(), rand());                               \
  start = clock();                                           \
  l.sort();                                                  \
  end = clock();                                             \
  int n = static_cast<int>(static_cast<double>(end - start)  \
      / CLOCKS_PER_SEC * 1000);                              \
  std::snprintf(buf, sizeof(buf), "%d", n);                  \
  std::string t = buf;                                       \
  t += "ms    |";                                            \
  std::cout << std::setw(WIDE) << t;                         \
} while(0)

// 重构重复代码
#define CON_TEST_P1(con, fun, arg, len1, len2, len3)         \
  TEST_LEN(len1, len2, len3, WIDE);                          \
  std::cout << "|         std         |";                    \
  FUN_TEST_FORMAT1(std::con, fun, arg, len1);                \
  FUN_TEST_FORMAT1(std::con, fun, arg, len2);                \
  FUN_TEST_FORMAT1(std::con, fun, arg, len3);                \
  std::cout << "\n|        mystl        |";                  \
  FUN_TEST_FORMAT1(mystl::con, fun, arg, len1);              \
  FUN_TEST_FORMAT1(mystl::con, fun, arg, len2);              \
  FUN_TEST_FORMAT1(mystl::con, fun, arg, len3);    


#define CON_TEST_P2(con, fun, arg1, arg2, len1, len2, len3)  \
  TEST_LEN(len1, len2, len3, WIDE);                          \
  std::cout << "|         std         |";                    \
  FUN_TEST_FORMAT2(std::con, fun, arg1, arg2, len1);         \
  FUN_TEST_FORMAT2(std::con, fun, arg1, arg2, len2);         \
  FUN_TEST_FORMAT2(std::con, fun, arg1, arg2, len3);         \
  std::cout << "\n|        mystl        |";                  \
  FUN_TEST_FORMAT2(mystl::con, fun, arg1, arg2, len1);       \
  FUN_TEST_FORMAT2(mystl::con, fun, arg1, arg2, len2);       \
  FUN_TEST_FORMAT2(mystl::con, fun, arg1, arg2, len3);  

// map insert
#define MAP_INSERT_TEST(con, len1, len2, len3)               \
  TEST_LEN(len1, len2, len3, WIDE);                          \
  std::cout << "|         std         |";                    \
  MAP_INSERT_DO_TEST(std, con, len1);                        \
  MAP_INSERT_DO_TEST(std, con, len2);                        \
  MAP_INSERT_DO_TEST(std, con, len3);                        \
  std::cout << "\n|        mystl        |";                  \
  MAP_INSERT_DO_TEST(mystl, con, len1);                      \
  MAP_INSERT_DO_TEST(mystl, con, len2);                      \
  MAP_INSERT_DO_TEST(mystl, con, len3); 

#define LIST_SORT_TEST(len1, len2, len3)                     \
  TEST_LEN(len1, len2, len3, WIDE);                          \
  std::cout << "|         std         |";                    \
  LIST_SORT_DO_TEST(std, len1);                              \
  LIST_SORT_DO_TEST(std, len2);                              \
  LIST_SORT_DO_TEST(std, len3);                              \
  std::cout << "\n|        mystl        |";                  \
  LIST_SORT_DO_TEST(mystl, len1);                            \
  LIST_SORT_DO_TEST(mystl, len2);                            \
  LIST_SORT_DO_TEST(mystl, len3);



} // end of namespace mytest

#endif