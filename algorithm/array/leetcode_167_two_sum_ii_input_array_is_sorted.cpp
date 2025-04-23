// 2025/4/22
// zhangzhong
// https://leetcode.cn/problems/two-sum-ii-input-array-is-sorted/description/

// 167. 两数之和2 - 输入有序数组

// 给你一个下标从 1 开始的整数数组 numbers ，该数组已按 非递减顺序排列
// ，请你从数组中找出满足相加之和等于目标数 target
// 的两个数。如果设这两个数分别是 numbers[index1] 和 numbers[index2] ，则 1 <=
// index1 < index2 <= numbers.length 。

// 以长度为 2 的整数数组 [index1, index2] 的形式返回这两个整数的下标 index1 和
// index2。

// 你可以假设每个输入 只对应唯一的答案 ，而且你 不可以 重复使用相同的元素。

// 你所设计的解决方案必须只使用常量级的额外空间。

// 示例 1：

// 输入：numbers = [2,7,11,15], target = 9
// 输出：[1,2]
// 解释：2 与 7 之和等于目标数 9 。因此 index1 = 1, index2 = 2 。返回 [1, 2] 。
// 示例 2：

// 输入：numbers = [2,3,4], target = 6
// 输出：[1,3]
// 解释：2 与 4 之和等于目标数 6 。因此 index1 = 1, index2 = 3 。返回 [1, 3] 。
// 示例 3：

// 输入：numbers = [-1,0], target = -1
// 输出：[1,2]
// 解释：-1 与 0 之和等于目标数 -1 。因此 index1 = 1, index2 = 2 。返回 [1, 2]
// 。

// 提示：

// 2 <= numbers.length <= 3 * 104
// -1000 <= numbers[i] <= 1000
// numbers 按 非递减顺序 排列
// -1000 <= target <= 1000
// 仅存在一个有效答案

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <vector>

class Solution {
public:
  // 只要数组有序，就应该想到双指针技巧
  std::vector<int> twoSum(std::vector<int>& numbers, int target) {
    // 双指针

    // 对于从两侧向内的双指针
    // 显然就没有办法使用 左必右开的方式了？
    // 不对，我记得快排的partition也是用的[begin, end) 的方式实现的
    // 但是内部也是用左必右必的方式写的
    // 因为两侧向内的双指针，就是适合这种方式

    int left = 0;
    int right = numbers.size() - 1;

    // 关键是这里的相遇条件
    // 因为right实际上是 end -1
    // 所以原来的 begin < end
    // 变成了 left <= right
    // 不对！这个相遇条件，说实话，每道题都不一样
    // 不能直接写
    // 就像这道题目，left != right, 因为相同的数字不能使用两次
    while (left < right) {
      // 因为数组是从小到大排序的
      // 所以我们将左右两侧的值加起来
      int num = numbers[left] + numbers[right];
      if (num < target) {
        // 如果和必target 小，说明那个小数太小了
        left++;
      } else if (num > target) {
        // 说明大数太大了
        right--;
      } else {
        // 相等！we found it
        return {left + 1, right + 1};
      }
    }

    // 题目中好像没有给出没有解的情况应该返回什么。
    return {-1, -1};
  }
};

TEST_CASE("Example 1: numbers = [2,7,11,15], target = 9") {
  Solution solution;
  std::vector<int> numbers = {2, 7, 11, 15};
  int target = 9;
  std::vector<int> result = solution.twoSum(numbers, target);

  CHECK(result.size() == 2);
  CHECK(result[0] == 1);
  CHECK(result[1] == 2);
}

TEST_CASE("Example 2: numbers = [2,3,4], target = 6") {
  Solution solution;
  std::vector<int> numbers = {2, 3, 4};
  int target = 6;
  std::vector<int> result = solution.twoSum(numbers, target);

  CHECK(result.size() == 2);
  CHECK(result[0] == 1);
  CHECK(result[1] == 3);
}

TEST_CASE("Example 3: numbers = [-1,0], target = -1") {
  Solution solution;
  std::vector<int> numbers = {-1, 0};
  int target = -1;
  std::vector<int> result = solution.twoSum(numbers, target);

  CHECK(result.size() == 2);
  CHECK(result[0] == 1);
  CHECK(result[1] == 2);
}

TEST_CASE("Edge case: minimal array size") {
  Solution solution;
  std::vector<int> numbers = {1, 2};
  int target = 3;
  std::vector<int> result = solution.twoSum(numbers, target);

  CHECK(result.size() == 2);
  CHECK(result[0] == 1);
  CHECK(result[1] == 2);
}

TEST_CASE("Edge case: numbers at array ends") {
  Solution solution;
  std::vector<int> numbers = {1, 3, 5, 7, 9};
  int target = 10;
  std::vector<int> result = solution.twoSum(numbers, target);

  CHECK(result.size() == 2);
  CHECK(result[0] == 1);
  CHECK(result[1] == 5);
}
