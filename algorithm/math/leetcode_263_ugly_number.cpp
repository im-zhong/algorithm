// 2025/5/10
// zhangzhong
// https://leetcode.cn/problems/ugly-number/description/

// 263. 丑数

// 丑数 就是只包含质因数 2、3 和 5 的 正 整数。

// 给你一个整数 n ，请你判断 n 是否为 丑数 。如果是，返回 true ；否则，返回
// false 。

// 示例 1：

// 输入：n = 6
// 输出：true
// 解释：6 = 2 × 3
// 示例 2：

// 输入：n = 1
// 输出：true
// 解释：1 没有质因数。
// 示例 3：

// 输入：n = 14
// 输出：false
// 解释：14 不是丑数，因为它包含了另外一个质因数 7 。

// 提示：

// -231 <= n <= 231 - 1

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <bitset>
#include <doctest/doctest.h>
#include <vector>

// 我不太确定leetcode是怎么测试的
// 是每个测试用例都会构造一个solution吗？

class Solution {
public:
  // 应该是找出所有的质数，然后试除这些质数，如果除尽了
  // 如果是找所有的因数，试除的更多

  bool is_prime(int n) {
    uint64_t nn = n;

    // 什么是素数？
    // 就是出了1和n自身，不能有其他因子
    // 总感觉这里不如调用sqrt快啊
    // p <= sqrt(n)
    for (uint64_t p = 2; p * p <= nn; p++) {
      if (nn % p == 0) {
        return false;
      }
    }
    return true;
  }

  bool isUglySlow(int n) {
    // n > 0 正整数才行
    if (n <= 0) {
      return false;
    }

    // 只能是找所有的因数了
    uint64_t nn = n;
    // BUG：因数需要从1开始枚举啊！！！质数才需要从2开始枚举
    for (uint64_t i = 1; i * i <= nn; i++) {
      if (nn % i != 0) {
        continue;
      }

      // i and n/i is factor of nn
      // just check if
      int factor = i;
      if (factor > 5 && is_prime(factor)) {
        return false;
      }
      factor = nn / i;
      if (factor > 5 && is_prime(factor)) {
        return false;
      }
    }

    return true;

    // 哎！不对，n的因数可能比sqrt(n)大呀，所以如果想要对素数打表的话
    // 这个题目需要对1 - 2^31 里面的素数打表才行。。。
    // 所以还不如找出某个数的所有因数，然后看看里面的因数是不是素数的了
    // 不行，对这么大的范围打表是不现实的。
    // std::vector<uint64_t> primes = GetPrimes(INT_MAX / 2);
    // // 我们要去掉 2 3 5
    // for (int i = 3; i < primes.size(); i++) {
    //   if ((uint64_t)n % primes[i] == 0) {
    //     return false;
    //   }
    // }
    // return true;
  }

  bool isUgly(int n) {

    // 草，果然我的数学不好。。。
    // 官方题解：
    // 根据丑数的定义，0 和负整数一定不是丑数。

    // 当 n>0 时，若 n 是丑数，则 n 可以写成 n = 2^a * 3^b * 5^c
    // 的形式，其中 a,b,c >= 0
    // 都是非负整数。特别地，当 a,b,c 都是 0 时，n=1。

    // 为判断 n 是否满足上述形式，可以对 n 反复除以 2,3,5，直到 n 不再包含质因数
    // 2,3,5。若剩下的数等于 1，则说明 n 不包含其他质因数，是丑数；否则，说明 n
    // 包含其他质因数，不是丑数。

    if (n <= 0) {
      return false;
    }

    while (n % 2 == 0) {
      n /= 2;
    }
    while (n % 3 == 0) {
      n /= 3;
    }
    while (n % 5 == 0) {
      n /= 5;
    }

    return n == 1;
  }

private:
  std::vector<int> create_prime_table() {
    // 只在32位整数范围内实现
    // 我忘了那个生成素数表的方法了

    // 原理是什么来着， 好像就是倍数筛
    // 2^31 开根号是多少？46340, 这么小吗？卧槽 还真是
    // 因为是质因数，所以我们只需要枚举小于 sqrt(INT_MAX)的数字就行了

    // 可以用uint64 来算，然后每次计算乘法 i * i <= INT_MAX
    // 我记得就是倍数筛，从k=2开始，只要是2的倍数的，肯定不是素数
    // 那么我首先需要一个数组
    // 用bit比较合适
    // 不如这样，既然默认的情况下所有的bit都是unset的
    // 咱们直接让这个数组变成 notset 咱们也省的初始化了
    const int sqrt_int_max = 65536;
    std::bitset<sqrt_int_max> not_prime;

    // 哦，还有一件事，这里的下标就是数字，也就是下标从1开始的
    for (int p = 2; p < sqrt_int_max; p++) {
      if (not_prime[p]) {
        continue;
      }

      // p = 2 is prime
      // then all the k * p, k > 1, is not prime
      for (int kp = p * 2; kp < sqrt_int_max; kp += p) {
        not_prime[kp] = true;
      }
    }

    std::vector<int> primes;
    for (int i = 2; i < sqrt_int_max; i++) {
      if (!not_prime[i]) {
        primes.push_back(i);
      }
    }
    return primes;
  }

  std::vector<uint64_t> GetPrimes(uint64_t n) {
    // return [1, n] 中的所有素数

    // all is false, means all is prime
    // 用这个就会出错
    // std::bitset<1000000000 + 16> not_prime;

    // 用这个就ok
    std::vector<bool> not_prime(n + 16);

    for (uint64_t p = 2; p * p <= n; p++) {
      if (not_prime[p]) {
        continue;
      }

      // all 2p 3p 4p ... <= n 的 都不是素数
      for (uint64_t kp = 2 * p; kp <= n; kp += p) {
        not_prime[kp] = true;
      }
    }

    // finally traverse the not_prime, collect the primes
    std::vector<uint64_t> primes;
    for (uint64_t p = 2; p <= n; p++) {
      if (!not_prime[p]) {
        primes.push_back(p);
      }
    }

    return primes;
  }

  std::vector<int> primes;
};

TEST_CASE("Example 1: n = 6") {
  Solution solution;
  CHECK(solution.isUgly(6) == true); // 6 = 2 × 3
}

TEST_CASE("Example 1: n = 7") {
  Solution solution;
  CHECK(solution.isUgly(7) == false); // 7 = 1 x 7
}

TEST_CASE("Example 2: n = 1") {
  Solution solution;
  CHECK(solution.isUgly(1) == true); // 1 has no prime factors
}

TEST_CASE("Example 3: n = 14") {
  Solution solution;
  CHECK(solution.isUgly(14) == false); // 14 contains prime factor 7
}

TEST_CASE("Edge case: n = 0") {
  Solution solution;
  CHECK(solution.isUgly(0) == false); // 0 is not a positive integer
}

TEST_CASE("Edge case: n = -6") {
  Solution solution;
  CHECK(solution.isUgly(-6) == false); // Negative numbers are not ugly numbers
}

TEST_CASE("n = 30") {
  Solution solution;
  CHECK(solution.isUgly(30) == true); // 30 = 2 × 3 × 5
}

TEST_CASE("n = 25") {
  Solution solution;
  CHECK(solution.isUgly(25) == true); // 25 = 5²
}

TEST_CASE("n = 22") {
  Solution solution;
  CHECK(solution.isUgly(22) == false); // 22 = 2 × 11, contains prime factor 11
}

TEST_CASE("n = 937351770") {
  Solution solution;
  CHECK(solution.isUgly(937351770) ==
        false); // Contains prime factors other than 2, 3, 5
}
