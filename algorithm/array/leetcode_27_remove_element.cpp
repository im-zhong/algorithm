// 2025/4/22
// zhangzhong
// https://leetcode.cn/problems/remove-element/

// 27. 移除元素

// 给你一个数组 nums 和一个值 val，你需要 原地 移除所有数值等于 val
// 的元素。元素的顺序可能发生改变。然后返回 nums 中与 val 不同的元素的数量。
// 假设 nums 中不等于 val 的元素数量为 k，要通过此题，您需要执行以下操作：
// 更改 nums 数组，使 nums 的前 k 个元素包含不等于 val 的元素。nums 的其余元素和
// nums 的大小并不重要。 返回 k。 用户评测：

// 评测机将使用以下代码测试您的解决方案：

// int[] nums = [...]; // 输入数组
// int val = ...; // 要移除的值
// int[] expectedNums = [...]; // 长度正确的预期答案。
//                             // 它以不等于 val 的值排序。

// int k = removeElement(nums, val); // 调用你的实现

// assert k == expectedNums.length;
// sort(nums, 0, k); // 排序 nums 的前 k 个元素
// for (int i = 0; i < actualLength; i++) {
//     assert nums[i] == expectedNums[i];
// }
// 如果所有的断言都通过，你的解决方案将会 通过。

// 示例 1：
// 输入：nums = [3,2,2,3], val = 3
// 输出：2, nums = [2,2,_,_]
// 解释：你的函数函数应该返回 k = 2, 并且 nums 中的前两个元素均为 2。
// 你在返回的 k 个元素之外留下了什么并不重要（因此它们并不计入评测）。

// 示例 2：
// 输入：nums = [0,1,2,2,3,0,4,2], val = 2
// 输出：5, nums = [0,1,4,0,3,_,_,_]
// 解释：你的函数应该返回 k = 5，并且 nums 中的前五个元素为 0,0,1,3,4。
// 注意这五个元素可以任意顺序返回。
// 你在返回的 k 个元素之外留下了什么并不重要（因此它们并不计入评测）。

// 提示：
// 0 <= nums.length <= 100
// 0 <= nums[i] <= 50
// 0 <= val <= 100

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <vector>

class Solution {
public:
  // 注意这道题目和 leetcode 26 移除有序数组中重复元素的细微差别
  // 在那道题目中，fast指针总是比slow指针快，slow指针的下一个元素才是要插入的位置
  // 但是这里，slow指针就是指向要插入的位置
  // 为什么？因为26
  // 需要移除重复，第一个元素需要保留，slow指针指向的第一个位置一定需要保留
  // 但是这里，slow指针指向的不一定是有效的，不一定需要保留
  int removeElement(std::vector<int>& nums, int val) {

    // 如果 fast 遇到值为 val 的元素，则直接跳过，否则就赋值给 slow 指针，并让
    // slow 前进一步
    // if (nums.empty()) {
    //   return 0;
    // }

    // int slow = 0;
    // int fast = 0;
    // while (fast < nums.size()) {
    //   if (nums[fast] != val) {
    //     nums[slow] = nums[fast];
    //     slow++;
    //     // fast++;
    //   }
    //   //   else {
    //   //     // fast++;
    //   //   }
    //   fast++;
    // }

    int slow = 0;
    for (int fast = 0; fast < nums.size(); fast++) {
      if (nums[fast] != val) {
        nums[slow] = nums[fast];
        slow++;
      }
    }
    return slow;
  }
};

TEST_CASE("Example 1: [3,2,2,3], val=3") {
  Solution solution;
  std::vector<int> nums = {3, 2, 2, 3};
  int val = 3;
  int k = solution.removeElement(nums, val);

  CHECK(k == 2);

  // Check that none of the first k elements are equal to val
  for (int i = 0; i < k; i++) {
    CHECK(nums[i] != val);
  }

  // Check that we have the expected elements (order doesn't matter)
  std::vector<int> expected = {2, 2};
  std::sort(nums.begin(), nums.begin() + k);
  std::sort(expected.begin(), expected.end());
  for (int i = 0; i < k; i++) {
    CHECK(nums[i] == expected[i]);
  }
}

TEST_CASE("Example 2: [0,1,2,2,3,0,4,2], val=2") {
  Solution solution;
  std::vector<int> nums = {0, 1, 2, 2, 3, 0, 4, 2};
  int val = 2;
  int k = solution.removeElement(nums, val);

  CHECK(k == 5);

  // Check that none of the first k elements are equal to val
  for (int i = 0; i < k; i++) {
    CHECK(nums[i] != val);
  }

  // Check that we have the expected elements (order doesn't matter)
  std::vector<int> expected = {0, 1, 3, 0, 4};
  std::sort(nums.begin(), nums.begin() + k);
  std::sort(expected.begin(), expected.end());
  for (int i = 0; i < k; i++) {
    CHECK(nums[i] == expected[i]);
  }
}

TEST_CASE("Empty array") {
  Solution solution;
  std::vector<int> nums = {};
  int val = 5;
  int k = solution.removeElement(nums, val);

  CHECK(k == 0);
}

TEST_CASE("All elements equal to val") {
  Solution solution;
  std::vector<int> nums = {3, 3, 3, 3};
  int val = 3;
  int k = solution.removeElement(nums, val);

  CHECK(k == 0);
}

TEST_CASE("No elements equal to val") {
  Solution solution;
  std::vector<int> nums = {1, 2, 3, 4};
  int val = 5;
  int k = solution.removeElement(nums, val);

  CHECK(k == 4);
  // Original array should be unchanged
  std::vector<int> expected = {1, 2, 3, 4};
  for (int i = 0; i < k; i++) {
    CHECK(nums[i] == expected[i]);
  }
}
