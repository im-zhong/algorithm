// 2025/4/22
// zhangzhong
// https://leetcode.cn/problems/find-first-and-last-position-of-element-in-sorted-array/

// 注意：本题与主站 34
// 题相同（仅返回值不同）：https://leetcode-cn.com/problems/find-first-and-last-position-of-element-in-sorted-array/
// https://leetcode.cn/problems/zai-pai-xu-shu-zu-zhong-cha-zhao-shu-zi-lcof/

// 34. 在排序数组中查找元素的第一个和最后一个位置

// 给你一个按照非递减顺序排列的整数数组 nums，和一个目标值
// target。请你找出给定目标值在数组中的开始位置和结束位置。
// 如果数组中不存在目标值 target，返回 [-1, -1]。
// 你必须设计并实现时间复杂度为 O(log n) 的算法解决此问题。

// 示例 1：
// 输入：nums = [5,7,7,8,8,10], target = 8
// 输出：[3,4]

// 示例 2：
// 输入：nums = [5,7,7,8,8,10], target = 6
// 输出：[-1,-1]

// 示例 3：
// 输入：nums = [], target = 0
// 输出：[-1,-1]

// 提示：
// 0 <= nums.length <= 105
// -109 <= nums[i] <= 109
// nums 是一个非递减数组
// -109 <= target <= 109

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <vector>

class Solution {
public:
  int lower_bound(std::vector<int>& nums, int target) {
    int first = 0;
    int last = nums.size();

    while (first < last) {

      int middle = first + (last - first) / 2;

      // ！！！ 无敌，过滤相等的情况就行了
      // 其实，如果只考虑不等号，在左侧还是右侧是非常容易理解的
      // 关键是等号的处理
      // 如果相等, 显然下界应该在左侧
      if (nums[middle] >= target) {
        // 下界在左侧
        last = middle;
      } else {
        // 下界在右侧
        // 哦！我知道了，如果在上面的那个分支里，middle刚好就是下界
        // 那么在下一次循环中，这里的first = middle + 1 就会调整回来？
        // 可能吧
        // 要么就是我写错了
        first = middle + 1;
      }
    }
    return first;
  }

  int upper_bound(std::vector<int>& nums, int target) {
    int first = 0;
    int last = nums.size();

    while (first < last) {
      int middle = first + (last - first) / 2;
      // 如果相等，显然上界应该在右侧
      if (nums[middle] <= target) {
        // 上界在右侧
        first = middle + 1;
      } else {
        last = middle;
      }
    }
    return first;
  }
  // 当然了，我们也可以自己实现 lower bound和 upper bound
  std::vector<int> searchRange(std::vector<int>& nums, int target) {
    // 这就是upper bound和ower bound啊

    // auto begin = std::lower_bound(nums.begin(), nums.end(), target);
    // auto end = std::upper_bound(nums.begin(), nums.end(), target);
    // 还有一个更简单的方式，就是使用equal_range 直接返回一对迭代器
    int begin = lower_bound(nums, target);
    int end = upper_bound(nums, target);
    if (begin == end) {
      return {-1, -1};
    } else {
      return {begin, end - 1};
    }
  }

  // 当然了，我们也可以自己实现 lower bound和 upper bound
  std::vector<int> searchRange1(std::vector<int>& nums, int target) {
    // 这就是upper bound和ower bound啊

    // auto begin = std::lower_bound(nums.begin(), nums.end(), target);
    // auto end = std::upper_bound(nums.begin(), nums.end(), target);
    // 还有一个更简单的方式，就是使用equal_range 直接返回一对迭代器
    auto iter_pair = std::equal_range(nums.begin(), nums.end(), target);
    if (iter_pair.first == iter_pair.second) {
      return {-1, -1};
    } else {
      return {static_cast<int>(iter_pair.first - nums.begin()),
              static_cast<int>(iter_pair.second - nums.begin() - 1)};
    }
  }
};

TEST_CASE("Example 1: Target exists with multiple occurrences") {
  Solution solution;
  std::vector<int> nums = {5, 7, 7, 8, 8, 10};
  int target = 8;
  std::vector<int> result = solution.searchRange(nums, target);
  std::vector<int> expected = {3, 4};

  CHECK(result == expected);
}

TEST_CASE("Example 2: Target does not exist") {
  Solution solution;
  std::vector<int> nums = {5, 7, 7, 8, 8, 10};
  int target = 6;
  std::vector<int> result = solution.searchRange(nums, target);
  std::vector<int> expected = {-1, -1};

  CHECK(result == expected);
}

TEST_CASE("Example 3: Empty array") {
  Solution solution;
  std::vector<int> nums = {};
  int target = 0;
  std::vector<int> result = solution.searchRange(nums, target);
  std::vector<int> expected = {-1, -1};

  CHECK(result == expected);
}

TEST_CASE("Target exists with single occurrence") {
  Solution solution;
  std::vector<int> nums = {1, 3, 5, 7, 9};
  int target = 5;
  std::vector<int> result = solution.searchRange(nums, target);
  std::vector<int> expected = {2, 2};

  CHECK(result == expected);
}

TEST_CASE("Target at the beginning of array") {
  Solution solution;
  std::vector<int> nums = {5, 5, 5, 7, 8, 9};
  int target = 5;
  std::vector<int> result = solution.searchRange(nums, target);
  std::vector<int> expected = {0, 2};

  CHECK(result == expected);
}

TEST_CASE("Target at the end of array") {
  Solution solution;
  std::vector<int> nums = {1, 2, 3, 7, 7, 7};
  int target = 7;
  std::vector<int> result = solution.searchRange(nums, target);
  std::vector<int> expected = {3, 5};

  CHECK(result == expected);
}

TEST_CASE("All elements are the target") {
  Solution solution;
  std::vector<int> nums = {8, 8, 8, 8};
  int target = 8;
  std::vector<int> result = solution.searchRange(nums, target);
  std::vector<int> expected = {0, 3};

  CHECK(result == expected);
}
