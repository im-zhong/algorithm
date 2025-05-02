// 2025/4/25
// zhangzhong
// https://leetcode.cn/problems/permutations/

// 46. 全排列

// 给定一个不含重复数字的数组 nums ，返回其 所有可能的全排列 。你可以 按任意顺序
// 返回答案。

// 示例 1：

// 输入：nums = [1,2,3]
// 输出：[[1,2,3],[1,3,2],[2,1,3],[2,3,1],[3,1,2],[3,2,1]]
// 示例 2：

// 输入：nums = [0,1]
// 输出：[[0,1],[1,0]]
// 示例 3：

// 输入：nums = [1]
// 输出：[[1]]

// 提示：

// 1 <= nums.length <= 6
// -10 <= nums[i] <= 10
// nums 中的所有整数 互不相同

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <algorithm>
#include <doctest/doctest.h>
#include <string>
#include <vector>

// 回溯有三个点需要考虑
// 路径，就是当前已经做出的选择，是回溯树上的某条路径
// 选择列表，就是当前可以选择的选项，就是回溯树上的分支
// 结束条件：也就是到达决策树底层，无法再做选择的条件。递归结束条件，这个时候就拿到了一个回溯的解，可能需要用这个解来解决某个问题

class Solution {
public:
  std::vector<std::vector<int>> permutations;

  std::vector<std::vector<int>> permute(std::vector<int>& nums) {

    // 记录路径，因为路径会在尾部push pop 所以vector还真挺方便的
    std::vector<int> track;

    // 还要记录选择列表，最开始的选择列表就是nums啊
    // 每次选择一个元素，都会把其中一个给mask掉
    // 所以比较好的方式，是使用一个mask数组，选择的时候就 mask=tre
    // 后序的时候 就是 mask=false
    std::vector<bool> used(nums.size(), false);

    // 可以让permuteImpl将解放到参数里面
    // 或者使用一个全局变量

    // 然后需要一个额外的impl函数来做支持
    permuteImpl(nums, track, used);

    return permutations;
  }

  // 注意到，track和used都是可变引用
  void permuteImpl(const std::vector<int>& nums, std::vector<int>& track,
                   std::vector<bool> used) {

    // 递归结束条件
    if (track.size() == nums.size()) {
      permutations.push_back(track);
      return;
    }

    // 遍历所有的选择
    for (int i = 0; i < used.size(); i++) {
      // 被使用的跳过
      if (used[i]) {
        continue;
      }

      // 做出选择
      // choose i
      // used i
      used[i] = true;
      // track i
      track.push_back(nums[i]);

      // 递归
      permuteImpl(nums, track, used);

      // 后序
      // un chooise i
      // un used i
      used[i] = false;
      track.pop_back();
    }
  }
};

TEST_CASE("Example 1: nums = [1,2,3]") {
  Solution solution;
  std::vector<int> nums = {1, 2, 3};
  std::vector<std::vector<int>> result = solution.permute(nums);

  // Check the size first
  CHECK(result.size() == 6); // 3! = 6 permutations

  // Since the problem says we can return the answer in any order,
  // we'll check if all expected permutations are in the result
  std::vector<std::vector<int>> expected = {{1, 2, 3}, {1, 3, 2}, {2, 1, 3},
                                            {2, 3, 1}, {3, 1, 2}, {3, 2, 1}};

  // Sort both result and expected to ensure the comparison works
  std::sort(result.begin(), result.end());
  std::sort(expected.begin(), expected.end());

  CHECK(result == expected);
}

TEST_CASE("Example 2: nums = [0,1]") {
  Solution solution;
  std::vector<int> nums = {0, 1};
  std::vector<std::vector<int>> result = solution.permute(nums);

  // Check the size first
  CHECK(result.size() == 2); // 2! = 2 permutations

  std::vector<std::vector<int>> expected = {{0, 1}, {1, 0}};

  // Sort both result and expected to ensure the comparison works
  std::sort(result.begin(), result.end());
  std::sort(expected.begin(), expected.end());

  CHECK(result == expected);
}

TEST_CASE("Example 3: nums = [1]") {
  Solution solution;
  std::vector<int> nums = {1};
  std::vector<std::vector<int>> result = solution.permute(nums);

  // Check the size first
  CHECK(result.size() == 1); // 1! = 1 permutation

  std::vector<std::vector<int>> expected = {{1}};

  CHECK(result == expected);
}
