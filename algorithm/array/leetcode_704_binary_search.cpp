// 2025/4/22
// zhangzhong
// https://leetcode.cn/problems/binary-search/description/

// 704. 二分查找

// 给定一个 n 个元素有序的（升序）整型数组 nums 和一个目标值 target
// ，写一个函数搜索 nums 中的 target，如果目标值存在返回下标，否则返回 -1。

// 示例 1:

// 输入: nums = [-1,0,3,5,9,12], target = 9
// 输出: 4
// 解释: 9 出现在 nums 中并且下标为 4
// 示例 2:

// 输入: nums = [-1,0,3,5,9,12], target = 2
// 输出: -1
// 解释: 2 不存在 nums 中因此返回 -1

// 提示：

// 你可以假设 nums 中的所有元素是不重复的。
// n 将在 [1, 10000]之间。
// nums 的每个元素都将在 [-9999, 9999]之间。

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <vector>

class Solution {
public:
  int search(std::vector<int>& nums, int target) {

    int begin = 0;
    int end = nums.size();

    while (begin < end) {
      int middle = begin + (end - begin) / 2;

      if (nums[middle] < target) {
        // 右侧
        begin = middle + 1;
      } else if (nums[middle] > target) {
        // 左侧
        end = middle;
      } else {
        // 注意到，这里和leetcode 34 就是二分的三种写法
        // binary search, lower bound, upper bound
        // 他们其实都需要考虑三种情况 < > =
        // 其实就是在这三种情况下，选择不同的区间，产生了三种分别
        // 不等于的两种情况都非常容易判断，=的情况仔细思考，这三种二分算法都非常好理解。
        // 相等
        return middle;
      }
    }

    return -1;
  }
};
