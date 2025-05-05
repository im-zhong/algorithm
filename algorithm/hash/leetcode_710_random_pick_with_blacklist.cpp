// 2025/5/5
// zhangzhong
// 710. 黑名单中的随机数

// 给定一个整数 n 和一个 无重复 黑名单整数数组 blacklist 。设计一种算法，从 [0,
// n - 1] 范围内的任意整数中选取一个 未加入 黑名单 blacklist
// 的整数。任何在上述范围内且不在黑名单 blacklist 中的整数都应该有 同等的可能性
// 被返回。

// 优化你的算法，使它最小化调用语言 内置 随机函数的次数。

// 实现 Solution 类:

// Solution(int n, int[] blacklist) 初始化整数 n 和被加入黑名单 blacklist 的整数
// int pick() 返回一个范围为 [0, n - 1] 且不在黑名单 blacklist 中的随机整数

// 示例 1：

// 输入
// ["Solution", "pick", "pick", "pick", "pick", "pick", "pick", "pick"]
// [[7, [2, 3, 5]], [], [], [], [], [], [], []]
// 输出
// [null, 0, 4, 1, 6, 1, 0, 4]

// 解释
// Solution solution = new Solution(7, [2, 3, 5]);
// solution.pick(); //
// 返回0，任何[0,1,4,6]的整数都可以。注意，对于每一个pick的调用，
//                  // 0、1、4和6的返回概率必须相等(即概率为1/4)。
// solution.pick(); // 返回 4
// solution.pick(); // 返回 1
// solution.pick(); // 返回 6
// solution.pick(); // 返回 1
// solution.pick(); // 返回 0
// solution.pick(); // 返回 4

// 提示:

// 1 <= n <= 109
// 0 <= blacklist.length <= min(105, n - 1)
// 0 <= blacklist[i] < n
// blacklist 中所有值都 不同
//  pick 最多被调用 2 * 104 次

// I have an idea, 直接把黑名单中的数字做一个映射
// 就是我们还是在 0 - (N-len(blacklist))中取

// 总结
// 1、如果想高效地，等概率地随机获取元素，就要使用数组作为底层容器。

// 2、如果要保持数组元素的紧凑性，可以把待删除元素换到最后，然后 pop
// 掉末尾的元素，这样时间复杂度就是 O(1)
// 了。当然，我们需要额外的哈希表记录值到索引的映射。

// 3、对于第二题，数组中含有「空洞」（黑名单数字），也可以利用哈希表巧妙处理映射关系，让数组在逻辑上是紧凑的，方便随机取元素。

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <chrono>
#include <random>
#include <unordered_map>
#include <vector>

class Solution {
public:
  Solution(int n, std::vector<int>& blacklist)
      : num_count(n),
        e(std::chrono::system_clock::now().time_since_epoch().count()),
        u(0, n - blacklist.size() - 1) {
    init(blacklist);
  }

  int pick() {
    // 现在又了 _blacklist map
    // 我们直接在 [0, num_count - blacklist.size() - 1] 中pick
    // 如果选择到了 _blacklist map中的数字，直接拿到他的映射即可
    int r = u(e);
    // 不对，即使r在map中，也可能是-1
    if (_blacklist_map.count(r) > 0 && _blacklist_map[r] != -1) {
      r = _blacklist_map[r];
    }
    return r;
  }

private:
  void init(std::vector<int>& blacklist) {

    // 1. first add all blacklist to _blacklist_map
    for (const int num : blacklist) {
      _blacklist_map.insert({num, -1});
    }

    // 2. assign number at the end of [0, n-1] to the blacklist nums
    int to_be_assign = num_count - blacklist.size();
    for (auto& iter : _blacklist_map) {
      // 如果被black的数字本来就比较大，那么就不用管了
      if (iter.first >= (num_count - blacklist.size())) {
        continue;
      }
      // to be assign 不能是已经存在于blacklist中的数字
      while (_blacklist_map.count(to_be_assign) > 0) {
        to_be_assign++;
      }
      iter.second = to_be_assign;
      to_be_assign++;
    }
  }

  int num_count;
  std::unordered_map<int, int> _blacklist_map;
  std::default_random_engine e;
  std::uniform_int_distribution<int> u;
};

/**
 * Your Solution object will be instantiated and called as such:
 * Solution* obj = new Solution(n, blacklist);
 * int param_1 = obj->pick();
 */

TEST_CASE("Solution example from problem description") {
  std::vector<int> blacklist = {2, 3, 5};
  Solution solution(7, blacklist);

  // We'll check that the picks are always from the valid set {0, 1, 4, 6}
  std::vector<int> valid_picks = {0, 1, 4, 6};

  for (int i = 0; i < 100; i++) {
    int picked = solution.pick();
    // Verify pick is one of the allowed values
    bool is_valid = false;
    for (int valid : valid_picks) {
      if (picked == valid) {
        is_valid = true;
        break;
      }
    }
    CHECK(is_valid);
  }
}

TEST_CASE("Solution with empty blacklist") {
  std::vector<int> blacklist = {};
  Solution solution(5, blacklist);

  // Pick should return numbers in range [0, 4]
  for (int i = 0; i < 50; i++) {
    int picked = solution.pick();
    CHECK(picked >= 0);
    CHECK(picked < 5);
  }
}

TEST_CASE("Solution with blacklist containing high-end numbers") {
  std::vector<int> blacklist = {7, 8, 9};
  Solution solution(10, blacklist);

  // Valid picks are [0, 1, 2, 3, 4, 5, 6]
  for (int i = 0; i < 50; i++) {
    int picked = solution.pick();
    CHECK(picked >= 0);
    CHECK(picked <= 6);
  }
}

TEST_CASE("Solution with large blacklist") {
  std::vector<int> blacklist = {1, 2, 3, 4, 5, 6, 7, 8};
  Solution solution(10, blacklist);

  // Valid picks are only 0 and 9
  for (int i = 0; i < 20; i++) {
    int picked = solution.pick();
    CHECK((picked == 0 || picked == 9));
  }
}

TEST_CASE("Solution with single valid number") {
  std::vector<int> blacklist = {0, 1, 3, 4};
  Solution solution(5, blacklist);

  // Only 2 is valid
  for (int i = 0; i < 10; i++) {
    CHECK(solution.pick() == 2);
  }
}

TEST_CASE("Solution with blacklist in the middle") {
  std::vector<int> blacklist = {3, 4, 5};
  Solution solution(10, blacklist);

  // Valid picks are [0, 1, 2, 6, 7, 8, 9]
  for (int i = 0; i < 50; i++) {
    int picked = solution.pick();
    CHECK(picked != 3);
    CHECK(picked != 4);
    CHECK(picked != 5);
    CHECK(picked >= 0);
    CHECK(picked < 10);
  }
}
