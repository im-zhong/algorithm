// 2025/5/5
// zhangzhong
// 我觉得我可以在之类标注一下题目的难度 ，简单 普通 困难， 比如说这是一道简单题
// 303. 区域和检索-数组不可变

// 给定一个整数数组  nums，处理以下类型的多个查询:

// 计算索引 left 和 right （包含 left 和 right）之间的 nums 元素的 和 ，其中
// left <= right 实现 NumArray 类：

// NumArray(int[] nums) 使用数组 nums 初始化对象
// int sumRange(int i, int j) 返回数组 nums 中索引 left 和 right 之间的元素的
// 总和 ，包含 left 和 right 两点（也就是 nums[left] + nums[left + 1] + ... +
// nums[right] )

// 示例 1：

// 输入：
// ["NumArray", "sumRange", "sumRange", "sumRange"]
// [[[-2, 0, 3, -5, 2, -1]], [0, 2], [2, 5], [0, 5]]
// 输出：
// [null, 1, -1, -3]

// 解释：
// NumArray numArray = new NumArray([-2, 0, 3, -5, 2, -1]);
// numArray.sumRange(0, 2); // return 1 ((-2) + 0 + 3)
// numArray.sumRange(2, 5); // return -1 (3 + (-5) + 2 + (-1))
// numArray.sumRange(0, 5); // return -3 ((-2) + 0 + 3 + (-5) + 2 + (-1))

// 提示：

// 1 <= nums.length <= 104
// -105 <= nums[i] <= 105
// 0 <= i <= j < nums.length
// 最多调用 104 次 sumRange 方法

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <vector>

// 这道题一看就是前缀和数组啊，

class NumArray {
public:
  NumArray(std::vector<int>& nums) : prefix_sums(nums.size() + 1, 0) {

    // just create a prefix sum array
    // prefix_sum.size = nums.size + 1
    // prefix_sum[0] = 0
    // prefix_sum[i] = nums[0, i].sum

    int sum = 0;
    for (int i = 0; i < nums.size(); i++) {
      sum += nums[i];
      prefix_sums[i + 1] = sum;
      // 也可以写成下面的样子
      // prefix_sums[i + 1] = prefix_sums[i] + nums[i];
      // 这个思想也是可以扩展的
      // i+1 = op(i, num[i])
      // 这个op不一定是加起来，
    }
  }

  int sumRange(int left, int right) {
    return prefix_sums[right + 1] - prefix_sums[left];
  }

private:
  std::vector<int> prefix_sums;
};

/**
 * Your NumArray object will be instantiated and called as such:
 * NumArray* obj = new NumArray(nums);
 * int param_1 = obj->sumRange(left,right);
 */
TEST_CASE("NumArray example from problem description") {
  std::vector<int> nums = {-2, 0, 3, -5, 2, -1};
  NumArray numArray(nums);

  CHECK(numArray.sumRange(0, 2) == 1);  // (-2) + 0 + 3 = 1
  CHECK(numArray.sumRange(2, 5) == -1); // 3 + (-5) + 2 + (-1) = -1
  CHECK(numArray.sumRange(0, 5) == -3); // (-2) + 0 + 3 + (-5) + 2 + (-1) = -3
}

TEST_CASE("NumArray with single element") {
  std::vector<int> nums = {5};
  NumArray numArray(nums);

  CHECK(numArray.sumRange(0, 0) == 5);
}

TEST_CASE("NumArray with all positive elements") {
  std::vector<int> nums = {1, 2, 3, 4, 5};
  NumArray numArray(nums);

  CHECK(numArray.sumRange(0, 4) == 15); // Sum of all elements
  CHECK(numArray.sumRange(1, 3) == 9);  // 2 + 3 + 4 = 9
  CHECK(numArray.sumRange(2, 2) == 3);  // Single element
  CHECK(numArray.sumRange(0, 0) == 1);  // First element
  CHECK(numArray.sumRange(4, 4) == 5);  // Last element
}

TEST_CASE("NumArray with all negative elements") {
  std::vector<int> nums = {-1, -2, -3, -4, -5};
  NumArray numArray(nums);

  CHECK(numArray.sumRange(0, 4) == -15); // Sum of all elements
  CHECK(numArray.sumRange(1, 3) == -9);  // -2 + -3 + -4 = -9
  CHECK(numArray.sumRange(0, 2) == -6);  // -1 + -2 + -3 = -6
}

TEST_CASE("NumArray example from problem description") {
  std::vector<int> nums = {-2, 0, 3, -5, 2, -1};
  NumArray numArray(nums);

  CHECK(numArray.sumRange(0, 2) == 1);  // (-2) + 0 + 3 = 1
  CHECK(numArray.sumRange(2, 5) == -1); // 3 + (-5) + 2 + (-1) = -1
  CHECK(numArray.sumRange(0, 5) == -3); // (-2) + 0 + 3 + (-5) + 2 + (-1) = -3
}

TEST_CASE("NumArray with single element") {
  std::vector<int> nums = {5};
  NumArray numArray(nums);

  CHECK(numArray.sumRange(0, 0) == 5);
}

TEST_CASE("NumArray with all positive elements") {
  std::vector<int> nums = {1, 2, 3, 4, 5};
  NumArray numArray(nums);

  CHECK(numArray.sumRange(0, 4) == 15); // Sum of all elements
  CHECK(numArray.sumRange(1, 3) == 9);  // 2 + 3 + 4 = 9
  CHECK(numArray.sumRange(2, 2) == 3);  // Single element
  CHECK(numArray.sumRange(0, 0) == 1);  // First element
  CHECK(numArray.sumRange(4, 4) == 5);  // Last element
}

TEST_CASE("NumArray with all negative elements") {
  std::vector<int> nums = {-1, -2, -3, -4, -5};
  NumArray numArray(nums);

  CHECK(numArray.sumRange(0, 4) == -15); // Sum of all elements
  CHECK(numArray.sumRange(1, 3) == -9);  // -2 + -3 + -4 = -9
  CHECK(numArray.sumRange(0, 2) == -6);  // -1 + -2 + -3 = -6
}
