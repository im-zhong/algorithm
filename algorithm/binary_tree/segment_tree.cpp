// 2025/5/5
// zhangzhong
// segment tree
// https://cp-algorithms.com/data_structures/segment_tree.html
// https://oi-wiki.org/ds/seg/
// https://labuladong.online/algo/data-structure-basic/segment-tree-basic/

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <algorithm>
#include <doctest/doctest.h>
#include <functional>
#include <queue>
#include <vector>

class SegmentTree {
  // 构造函数，给定一个数组，初始化线段树，时间复杂度 O(N)
  // merge 是一个函数，用于定义 query 方法的行为
  // 通过修改这个函数，可以让 query 函数返回区间的元素和、最大值、最小值等
public:
  SegmentTree(std::vector<int> nums, std::function<void(int, int)> merge)
      : nums(nums), merge(merge) {
    // 第一步应该是建树，今天写不完了，明天再写吧。
  }

  // 查询闭区间 [i, j] 的元素和（也可能是最大最小值，取决于 merge
  // 函数），时间复杂度 O(logN)

  int query(int i, int j) {}

  // 更新 nums[i] = val，时间复杂度 O(logN)

  void update(int i, int val) {}

private:
  std::vector<int> nums;
  std::function<void(int, int)> merge;
};
