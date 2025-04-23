// 2025/4/22
// zhangzhong
// https://leetcode.cn/problems/remove-duplicates-from-sorted-array/description/

// 26. 删除有序数组中的重复项

// 给你一个 非严格递增排列 的数组 nums ，请你 原地
// 删除重复出现的元素，使每个元素 只出现一次 ，返回删除后数组的新长度。元素的
// 相对顺序 应该保持 一致 。然后返回 nums 中唯一元素的个数。
// 考虑 nums 的唯一元素的数量为 k ，你需要做以下事情确保你的题解可以被通过：
// 更改数组 nums ，使 nums 的前 k 个元素包含唯一元素，并按照它们最初在 nums
// 中出现的顺序排列。nums 的其余元素与 nums 的大小不重要。 返回 k 。 判题标准:

// 系统会用下面的代码来测试你的题解:

// int[] nums = [...]; // 输入数组
// int[] expectedNums = [...]; // 长度正确的期望答案
// int k = removeDuplicates(nums); // 调用
// assert k == expectedNums.length;
// for (int i = 0; i < k; i++) {
//     assert nums[i] == expectedNums[i];
// }
// 如果所有断言都通过，那么您的题解将被 通过。

// 示例 1：
// 输入：nums = [1,1,2]
// 输出：2, nums = [1,2,_]
// 解释：函数应该返回新的长度 2 ，并且原数组 nums 的前两个元素被修改为 1, 2
// 。不需要考虑数组中超出新长度后面的元素。

// 示例 2：
// 输入：nums = [0,0,1,1,1,2,2,3,3,4]
// 输出：5, nums = [0,1,2,3,4]
// 解释：函数应该返回新的长度 5 ， 并且原数组 nums 的前五个元素被修改为 0, 1, 2,
// 3, 4 。不需要考虑数组中超出新长度后面的元素。

// 提示：

// 1 <= nums.length <= 3 * 104
// -104 <= nums[i] <= 104
// nums 已按 非严格递增 排列

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <vector>

class Solution {
public:
  // 有更简单，更容易理解的写法
  // 下面这里是类似标准库的写法
  int removeDuplicates(std::vector<int>& nums) {
    // 快慢指针题目
    // 慢指针指向重复元素的第一次出现的位置
    // 快指针指向重复元素最后一次出现的位置
    // 然后可以一次性跳过所有的重复元素
    // 而且慢指针就是插入元素的位置
    // 而且这就是C++中的unique算法

    // 这不就是vector的unique吗
    // 1 <= nums.length <= 3 * 104
    // 数组至少存在一个元素
    // 所以可以安全的让slow和fast指向第一个位置
    int slow = 0;
    int fast = 0;

    // fast要指向下一个元素
    while (++fast < nums.size()) {
      // ++fast;
      // 什么时候要切换slow呢？
      // 当fast和slow指向不同的元素的时候
      if (nums[fast] != nums[slow]) {
        // 我们将slow指向下一个位置
        // 然后将fast的内容复制到slow中
        // 有一个额外的情况就是 如果slow + 1 = fast
        // 这说明有两个连续的不同的元素
        // 这个时候只需要增加指针
        // 而不需要复制
        ++slow;
        if (slow != fast) {
          nums[slow] = nums[fast];
        }
      }
    }

    return slow + 1;
  }

  int removeDuplicates1(std::vector<int>& nums) {
    // 这不就是vector的unique吗
    auto last = std::unique(nums.begin(), nums.end());
    nums.erase(last, nums.end());
    return nums.size();
  }

  int removeDuplicates2(std::vector<int>& nums) {
    int slow = 0;
    for (int fast = 1; fast < nums.size(); fast++) {
      if (nums[fast] == nums[slow]) {
        continue;
      }
      // slow始终指向已经复制好的元素，也就是slow的下一个位置才是要复制的位置
      slow++;
      nums[slow] = nums[fast];
    }
    return slow + 1;
  }
};

TEST_CASE("removeDuplicates - Example 1") {
  std::vector<int> nums = {1, 1, 2};
  Solution solution;
  int k = solution.removeDuplicates(nums);

  CHECK(k == 2);
  CHECK(nums[0] == 1);
  CHECK(nums[1] == 2);
}

TEST_CASE("removeDuplicates - Example 2") {
  std::vector<int> nums = {0, 0, 1, 1, 1, 2, 2, 3, 3, 4};
  Solution solution;
  int k = solution.removeDuplicates(nums);

  CHECK(k == 5);
  CHECK(nums[0] == 0);
  CHECK(nums[1] == 1);
  CHECK(nums[2] == 2);
  CHECK(nums[3] == 3);
  CHECK(nums[4] == 4);
}

TEST_CASE("removeDuplicates - Single element") {
  std::vector<int> nums = {5};
  Solution solution;
  int k = solution.removeDuplicates(nums);

  CHECK(k == 1);
  CHECK(nums[0] == 5);
}

TEST_CASE("removeDuplicates - No duplicates") {
  std::vector<int> nums = {1, 2, 3, 4, 5};
  Solution solution;
  int k = solution.removeDuplicates(nums);

  CHECK(k == 5);
  for (int i = 0; i < k; i++) {
    CHECK(nums[i] == i + 1);
  }
}

TEST_CASE("removeDuplicates - All duplicates") {
  std::vector<int> nums = {7, 7, 7, 7, 7};
  Solution solution;
  int k = solution.removeDuplicates(nums);

  CHECK(k == 1);
  CHECK(nums[0] == 7);
}

TEST_CASE("removeDuplicates1 - Example 1") {
  std::vector<int> nums = {1, 1, 2};
  Solution solution;
  int k = solution.removeDuplicates1(nums);

  CHECK(k == 2);
  CHECK(nums[0] == 1);
  CHECK(nums[1] == 2);
}

TEST_CASE("removeDuplicates1 - Example 2") {
  std::vector<int> nums = {0, 0, 1, 1, 1, 2, 2, 3, 3, 4};
  Solution solution;
  int k = solution.removeDuplicates1(nums);

  CHECK(k == 5);
  CHECK(nums[0] == 0);
  CHECK(nums[1] == 1);
  CHECK(nums[2] == 2);
  CHECK(nums[3] == 3);
  CHECK(nums[4] == 4);
}

TEST_CASE("removeDuplicates1 - Single element") {
  std::vector<int> nums = {5};
  Solution solution;
  int k = solution.removeDuplicates1(nums);

  CHECK(k == 1);
  CHECK(nums[0] == 5);
}

TEST_CASE("removeDuplicates1 - No duplicates") {
  std::vector<int> nums = {1, 2, 3, 4, 5};
  Solution solution;
  int k = solution.removeDuplicates1(nums);

  CHECK(k == 5);
  for (int i = 0; i < k; i++) {
    CHECK(nums[i] == i + 1);
  }
}

TEST_CASE("removeDuplicates1 - All duplicates") {
  std::vector<int> nums = {7, 7, 7, 7, 7};
  Solution solution;
  int k = solution.removeDuplicates1(nums);

  CHECK(k == 1);
  CHECK(nums[0] == 7);
}
