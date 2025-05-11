// 2025/5/11
// zhangzhong
// https://leetcode.cn/problems/find-k-pairs-with-smallest-sums/description/

// 373. 查找和最小的K对数字

// 给定两个以 非递减顺序排列 的整数数组 nums1 和 nums2 , 以及一个整数 k 。

// 定义一对值 (u,v)，其中第一个元素来自 nums1，第二个元素来自 nums2 。

// 请找到和最小的 k 个数对 (u1,v1),  (u2,v2)  ...  (uk,vk) 。

// 示例 1:

// 输入: nums1 = [1,7,11], nums2 = [2,4,6], k = 3
// 输出: [1,2],[1,4],[1,6]
// 解释: 返回序列中的前 3 对数：
//      [1,2],[1,4],[1,6],[7,2],[7,4],[11,2],[7,6],[11,4],[11,6]
// 示例 2:

// 输入: nums1 = [1,1,2], nums2 = [1,2,3], k = 2
// 输出: [1,1],[1,1]
// 解释: 返回序列中的前 2 对数：
//      [1,1],[1,1],[1,2],[2,1],[1,2],[2,2],[1,3],[1,3],[2,3]

// 提示:

// 1 <= nums1.length, nums2.length <= 105
// -109 <= nums1[i], nums2[i] <= 109
// nums1 和 nums2 均为 升序排列
// 1 <= k <= 104
// k <= nums1.length * nums2.length

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <queue>
#include <vector>

class Solution {
public:
  std::vector<std::vector<int>> kSmallestPairs(std::vector<int>& nums1,
                                               std::vector<int>& nums2, int k) {

    // 还是合并链表
    // 笛卡尔积，第一个链表，和另一个链表的所有元素，遍历，组成一条链表
    // 所以总共有nums1.size or nums2.size 条链表
    // 因为merge
    // list的时间复杂度与链表的条数有关，所以应该用短的那条链表作为第一条链表
    // 因为结果对顺序有要求 所以如果我们交换了数组的顺序，需要一个标记 bool flag
    // 在返回结果的时候交换位置

    std::vector<std::vector<int>> results;

    bool is_flip = false;
    if (nums1.size() > nums2.size()) {
      is_flip = true;
      // 为了处理flip的引用，只能再写一个函数了
      // 哎，我把flip传到函数里面，直接在构造results的时候，就反转 不更好吗？
      results = kSmallestPairsImpl(nums2, nums1, k, is_flip);
    } else {
      is_flip = false;
      results = kSmallestPairsImpl(nums1, nums2, k, is_flip);
    }

    // if (is_flip) {
    //   for (std::vector<int>& vec : results) {
    //     std::swap(vec[0], vec[1]);
    //   }
    // }

    return results;
  }

  struct Index {
    int index1;
    int index2;
  };

  std::vector<std::vector<int>> kSmallestPairsImpl(std::vector<int>& nums1,
                                                   std::vector<int>& nums2,
                                                   int k, bool is_flip) {
    // assert nums1.size <= nums2.size

    auto greater = [&nums1, &nums2](const Index& lhs,
                                    const Index& rhs) -> bool {
      return (nums1[lhs.index1] + nums2[lhs.index2]) >
             (nums1[rhs.index1] + nums2[rhs.index2]);
    };
    std::priority_queue<Index, std::vector<Index>, decltype(greater)> pq(
        greater);

    // init pq
    for (int index1 = 0; index1 < nums1.size(); index1++) {
      pq.push(Index{index1, 0});
    }

    std::vector<std::vector<int>> results;
    while (results.size() < k && !pq.empty()) {
      Index index = pq.top();
      pq.pop();

      if (is_flip) {
        results.push_back({nums2[index.index2], nums1[index.index1]});
      } else {
        results.push_back({nums1[index.index1], nums2[index.index2]});
      }

      // next
      index.index2++;
      if (index.index2 < nums2.size()) {
        pq.push(index);
      }
    }

    return results;
  }
};

TEST_CASE("Example 1: nums1 = [1,7,11], nums2 = [2,4,6], k = 3") {
  std::vector<int> nums1 = {1, 7, 11};
  std::vector<int> nums2 = {2, 4, 6};
  int k = 3;

  Solution solution;
  auto result = solution.kSmallestPairs(nums1, nums2, k);

  std::vector<std::vector<int>> expected = {{1, 2}, {1, 4}, {1, 6}};
  CHECK(result == expected);
}

TEST_CASE("Example 2: nums1 = [1,1,2], nums2 = [1,2,3], k = 2") {
  std::vector<int> nums1 = {1, 1, 2};
  std::vector<int> nums2 = {1, 2, 3};
  int k = 2;

  Solution solution;
  auto result = solution.kSmallestPairs(nums1, nums2, k);

  std::vector<std::vector<int>> expected = {{1, 1}, {1, 1}};
  CHECK(result == expected);
}

TEST_CASE("k greater than total number of pairs") {
  std::vector<int> nums1 = {1, 2};
  std::vector<int> nums2 = {3};
  int k = 10;

  Solution solution;
  auto result = solution.kSmallestPairs(nums1, nums2, k);

  std::vector<std::vector<int>> expected = {{1, 3}, {2, 3}};
  CHECK(result == expected);
}

TEST_CASE("Single element arrays") {
  std::vector<int> nums1 = {5};
  std::vector<int> nums2 = {8};
  int k = 1;

  Solution solution;
  auto result = solution.kSmallestPairs(nums1, nums2, k);

  std::vector<std::vector<int>> expected = {{5, 8}};
  CHECK(result == expected);
}
