// 2025/4/25
// zhangzhong
// https://leetcode.cn/problems/fibonacci-number/

// 509. 斐波那契数

// 斐波那契数 （通常用 F(n) 表示）形成的序列称为 斐波那契数列 。该数列由 0 和 1
// 开始，后面的每一项数字都是前面两项数字的和。也就是：

// F(0) = 0，F(1) = 1
// F(n) = F(n - 1) + F(n - 2)，其中 n > 1
// 给定 n ，请计算 F(n) 。

// 示例 1：

// 输入：n = 2
// 输出：1
// 解释：F(2) = F(1) + F(0) = 1 + 0 = 1
// 示例 2：

// 输入：n = 3
// 输出：2
// 解释：F(3) = F(2) + F(1) = 1 + 1 = 2
// 示例 3：

// 输入：n = 4
// 输出：3
// 解释：F(4) = F(3) + F(2) = 2 + 1 = 3

// 提示：

// 0 <= n <= 30

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include <array>
#include <vector>

class Solution {

public:
  int fib(int n) {

    // 1.
    // return fib_easy(n);

    // 2, 这个就已经相当快了
    // return fib_memo(n);

    // 3. 还有一种更快的写法
    // 就是写成迭代的方式
    return fib_iter(n);
  }

  int fib_easy(int n) {
    // 最简单的写法就是直接递归

    // 递归结束条件
    if (n == 0 || n == 1) {
      return n;
    }

    return fib_easy(n - 1) + fib_easy(n - 2);
  }

  // 因为包含了重复计算，所以使用一个备忘录来记录每个字问题的解
  // f[n] = ?
  int fib_memo(int n) {
    if (n == 0 || n == 1) {
      return n;
    }

    if (memo[n] != 0) {
      return memo[n];
    }

    memo[n - 1] = fib_memo(n - 1);
    memo[n - 2] = fib_memo(n - 2);
    return memo[n - 1] + memo[n - 2];
  }

  void fib_dp() {
    fibs[0] = 0;
    fibs[1] = 1;

    for (int i = 2; i < fibs.size(); i++) {
      fibs[i] = fibs[i - 1] + fibs[i - 2];
    }
  }

  int fib_iter(int n) {
    // 状态是什么 fib[n] 这个就是状态
    // 这个就是状态转移方程啊：fib[n] = fib[n-1] + fib[n-2]
    // 计算顺序：从0，1 开始

    // 用dp把所有解都算出来，然后后面每次测试，直接查表就行
    fib_dp();
    return fibs[n];
  }

private:
  // 这样写他会认为是一个函数签名。。。。
  std::array<int, 32> memo{};
  std::array<int, 32> fibs{};
};
