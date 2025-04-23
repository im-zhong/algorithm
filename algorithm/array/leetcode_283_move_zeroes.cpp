// 2025/4/22
// zhangzhong
// https://leetcode.cn/problems/move-zeroes/description/

// 283. 移动零

// 给定一个数组 nums，编写一个函数将所有 0
// 移动到数组的末尾，同时保持非零元素的相对顺序。

// 请注意 ，必须在不复制数组的情况下原地对数组进行操作。

// 示例 1:
// 输入: nums = [0,1,0,3,12]
// 输出: [1,3,12,0,0]

// 示例 2:
// 输入: nums = [0]
// 输出: [0]

// 提示:
// 1 <= nums.length <= 104
// -231 <= nums[i] <= 231 - 1

// 进阶：你能尽量减少完成的操作次数吗？

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <vector>

class Solution {
public:
  //  这道题目和leetcode 27 一样啊，只不过从移动参数传入的val 变成了移动0而已
  // 还有一个区别就是从assign变成了swap
  // 因为题目要求被删除的元素还在，不对，这就不是删除，感觉这才是vector的unique的实现啊
  // 不是。unique的时间就是assign, not swap
  void moveZeroes(std::vector<int>& nums) {
    int slow = 0;
    for (int fast = 0; fast < nums.size(); fast++) {
      if (nums[fast] != 0) {
        // this should be a move, not an single assign
        // nums[slow] = nums[fast];
        std::swap(nums[slow], nums[fast]);
        slow++;
      }
    }
  }
};

TEST_CASE("Example 1: [0,1,0,3,12] -> [1,3,12,0,0]") {
  Solution solution;
  std::vector<int> nums = {0, 1, 0, 3, 12};
  std::vector<int> expected = {1, 3, 12, 0, 0};

  solution.moveZeroes(nums);

  CHECK(nums == expected);
}

TEST_CASE("Example 2: [0] -> [0]") {
  Solution solution;
  std::vector<int> nums = {0};
  std::vector<int> expected = {0};

  solution.moveZeroes(nums);

  CHECK(nums == expected);
}

TEST_CASE("No zeros: [1,2,3] -> [1,2,3]") {
  Solution solution;
  std::vector<int> nums = {1, 2, 3};
  std::vector<int> expected = {1, 2, 3};

  solution.moveZeroes(nums);

  CHECK(nums == expected);
}

TEST_CASE("All zeros: [0,0,0] -> [0,0,0]") {
  Solution solution;
  std::vector<int> nums = {0, 0, 0};
  std::vector<int> expected = {0, 0, 0};

  solution.moveZeroes(nums);

  CHECK(nums == expected);
}

TEST_CASE("Zeros at the end: [1,2,3,0,0] -> [1,2,3,0,0]") {
  Solution solution;
  std::vector<int> nums = {1, 2, 3, 0, 0};
  std::vector<int> expected = {1, 2, 3, 0, 0};

  solution.moveZeroes(nums);

  CHECK(nums == expected);
}

TEST_CASE("Zeros at the beginning: [0,0,1,2,3] -> [1,2,3,0,0]") {
  Solution solution;
  std::vector<int> nums = {0, 0, 1, 2, 3};
  std::vector<int> expected = {1, 2, 3, 0, 0};

  solution.moveZeroes(nums);

  CHECK(nums == expected);
}

TEST_CASE("Mixed array: [4,0,5,0,3,7] -> [4,5,3,7,0,0]") {
  Solution solution;
  std::vector<int> nums = {4, 0, 5, 0, 3, 7};
  std::vector<int> expected = {4, 5, 3, 7, 0, 0};

  solution.moveZeroes(nums);

  CHECK(nums == expected);
}
