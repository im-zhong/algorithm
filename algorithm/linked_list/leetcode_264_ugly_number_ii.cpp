// 2025/5/10
// zhangzhong
// https://leetcode.cn/problems/ugly-number-ii/description/

// 264. 丑数 2

// 给你一个整数 n ，请你找出并返回第 n 个 丑数 。

// 丑数 就是质因子只包含 2、3 和 5 的正整数。

// 示例 1：

// 输入：n = 10
// 输出：12
// 解释：[1, 2, 3, 4, 5, 6, 8, 9, 10, 12] 是由前 10 个丑数组成的序列。
// 示例 2：

// 输入：n = 1
// 输出：1
// 解释：1 通常被视为丑数。

// 提示：

// 1 <= n <= 1690

// 思考：
// 这道题目是构造一个序列
// 我们知道丑数由 2^a * 3^b * 5^c 组成
// 而且任意给出一个丑数p 2p 3p 5p 都是丑数
// 哎！这个又掉像素数筛啊，我们构造一个丑数筛 不就行了吗
// 我们只需要枚举丑数，
// 从p = 1 开始，将 2p = 2, 3p = 3, 5p = 5 加入到丑数集合中
// 1 2 3 5
// 2 * 2 = 4， 2 * 3 = 6， 2 * 5 = 10， 4 6 10
// 我们需要始终保证序列有序，其实就是将 链表 3 5 和链表 4 6 10 合并起来
// 然后n++ ， 因为现在只有 1 2
// 是确定的，只有已经枚举完的才能计数，负责你不知道是否中间还有空隙，还有丑数没有找到
// 知道枚举了n个，才是结束 返回n处的丑数即可

// 题解给出的思路和我的不一样，不过也不是完全不一样
// 题解的思路是，将 ugly*2 , ugly*3, ugly*5 分别看成一条链表
// 最开始的时候 ugly = [1]
// 然后三条链表应该保留一个只想ugly节点的指针，并构造 k*ugly_value
// 的一个值，如果被消耗了，就将节点指向下一个，构造新的值
// 那么整体上，就是一个三条有序链表的合并，当合并到第n个的时候，程序结束，返回值
// 题解给出的思路更好一些，更容易理解，也更容易写？

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <bitset>
#include <doctest/doctest.h>
#include <list>
#include <vector>

// 我有一个邪恶的想法
// 这道题目给出的n不算大，我可以枚举出所有的丑数，然后嘿嘿嘿。直接O(1)的算法，哈哈哈
class Solution {
public:
  // 虽然过了，但是为什么这么慢呢？
  // 草，tmd，不用链表，而是用数组啊，更快，因为我们合并的不是真的链表
  // 三个假想的有序的序列而已！
  int nthUglyNumberSlow(int n) {

    // 因为题目中没有提供ListNode，所以当然使用stl的list了
    std::list<int> ugly_numbers;
    ugly_numbers.push_back(1);

    // 我们写一个generator
    // 不用，直接用迭代器就行！
    auto ugly_2 = ugly_numbers.begin();
    auto ugly_3 = ugly_numbers.begin();
    auto ugly_5 = ugly_numbers.begin();

    // BUG:卧槽，忘了去重了
    while (ugly_numbers.size() != n) {
      // 到时候别忘了把他们 *2 *3 *5 就行了
      // 找到他们中的最小值
      //   int k = 5;
      //   // 不对，引用不能换绑吧
      //   auto& iter = ugly_5;
      if (*ugly_2 * 2 <= *ugly_3 * 3 && *ugly_2 * 2 <= *ugly_5 * 5) {
        // ugly_2 最小
        // TODO: 感觉我这里写的代码太多的重复了
        // 我们可以把倍数用一个变量来表示，然后拿到一个迭代器的引用，引用不能换绑，只能拿指针，就算了
        // 后面的这些逻辑就都可以合并了

        if (*ugly_2 * 2 > ugly_numbers.back()) {
          ugly_numbers.push_back(*ugly_2 * 2);
        }
        ugly_2++;

      } else if (*ugly_3 * 3 <= *ugly_2 * 2 && *ugly_3 * 3 <= *ugly_5 * 5) {
        // // ugly_3 最小
        if (*ugly_3 * 3 > ugly_numbers.back()) {
          ugly_numbers.push_back(*ugly_3 * 3);
        }
        ugly_3++;

      } else {
        // ugly_5 最小
        if (*ugly_5 * 5 > ugly_numbers.back()) {
          ugly_numbers.push_back(*ugly_5 * 5);
        }
        ugly_5++;
      }
    }
    return ugly_numbers.back();
  }

  // 提升了一点，但是不多
  // 果然，题解上的方法速度和我们的是差不多的
  // 所以那些速度很快的应该就是打表的，不用管了
  int nthUglyNumber(int n) {
    std::vector<int> ugly_numbers{1};
    ugly_numbers.reserve(1700);

    // 剩下的逻辑和上面是一样的
    // 只不过换成vector操作
    // 甚至我们可以预先申请足够的容量，还能防止出发多次申请释放内存
    // 哎！迭代器是通用的，直接复制过来就行，啊哈哈哈！
    auto ugly_2 = ugly_numbers.begin();
    auto ugly_3 = ugly_numbers.begin();
    auto ugly_5 = ugly_numbers.begin();

    // BUG:卧槽，忘了去重了
    while (ugly_numbers.size() != n) {
      // 到时候别忘了把他们 *2 *3 *5 就行了
      // 找到他们中的最小值
      //   int k = 5;
      //   // 不对，引用不能换绑吧
      //   auto& iter = ugly_5;
      if (*ugly_2 * 2 <= *ugly_3 * 3 && *ugly_2 * 2 <= *ugly_5 * 5) {
        // ugly_2 最小
        // TODO: 感觉我这里写的代码太多的重复了
        // 我们可以把倍数用一个变量来表示，然后拿到一个迭代器的引用，引用不能换绑，只能拿指针，就算了
        // 后面的这些逻辑就都可以合并了

        if (*ugly_2 * 2 > ugly_numbers.back()) {
          ugly_numbers.push_back(*ugly_2 * 2);
        }
        ugly_2++;

      } else if (*ugly_3 * 3 <= *ugly_2 * 2 && *ugly_3 * 3 <= *ugly_5 * 5) {
        // // ugly_3 最小
        if (*ugly_3 * 3 > ugly_numbers.back()) {
          ugly_numbers.push_back(*ugly_3 * 3);
        }
        ugly_3++;

      } else {
        // ugly_5 最小
        if (*ugly_5 * 5 > ugly_numbers.back()) {
          ugly_numbers.push_back(*ugly_5 * 5);
        }
        ugly_5++;
      }
    }
    return ugly_numbers.back();
  }
};

TEST_CASE("Example 1: n = 10") {
  Solution solution;
  CHECK(solution.nthUglyNumber(10) == 12);
  // Sequence: [1, 2, 3, 4, 5, 6, 8, 9, 10, 12]
}

TEST_CASE("Example 2: n = 1") {
  Solution solution;
  CHECK(solution.nthUglyNumber(1) == 1);
  // 1 is considered an ugly number
}

TEST_CASE("n = 7") {
  Solution solution;
  CHECK(solution.nthUglyNumber(7) == 8);
  // Sequence: [1, 2, 3, 4, 5, 6, 8]
}

TEST_CASE("n = 15") {
  Solution solution;
  CHECK(solution.nthUglyNumber(15) == 24);
  // Sequence: [1, 2, 3, 4, 5, 6, 8, 9, 10, 12, 15, 16, 18, 20, 24]
}

TEST_CASE("Edge case: n = 1690") {
  Solution solution;
  CHECK(solution.nthUglyNumber(1690) == 2123366400);
  // Maximum n from the problem constraints
}
