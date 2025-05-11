// 2025/5/8
// zhangzhong
// https://leetcode.cn/problems/range-sum-query-mutable/description/

// 307. 区域和检索 - 数组可修改

// 给你一个数组 nums ，请你完成两类查询。

// 其中一类查询要求 更新 数组 nums 下标对应的值
// 另一类查询要求返回数组 nums 中索引 left 和索引 right 之间（ 包含
// ）的nums元素的 和 ，其中 left <= right 实现 NumArray 类：

// NumArray(int[] nums) 用整数数组 nums 初始化对象
// void update(int index, int val) 将 nums[index] 的值 更新 为 val
// int sumRange(int left, int right) 返回数组 nums 中索引 left 和索引 right
// 之间（ 包含 ）的nums元素的 和 （即，nums[left] + nums[left + 1], ...,
// nums[right]）

// 示例 1：

// 输入：
// ["NumArray", "sumRange", "update", "sumRange"]
// [[[1, 3, 5]], [0, 2], [1, 2], [0, 2]]
// 输出：
// [null, 9, null, 8]

// 解释：
// NumArray numArray = new NumArray([1, 3, 5]);
// numArray.sumRange(0, 2); // 返回 1 + 3 + 5 = 9
// numArray.update(1, 2);   // nums = [1,2,5]
// numArray.sumRange(0, 2); // 返回 1 + 2 + 5 = 8

// 提示：

// 1 <= nums.length <= 3 * 104
// -100 <= nums[i] <= 100
// 0 <= index < nums.length
// -100 <= val <= 100
// 0 <= left <= right < nums.length
// 调用 update 和 sumRange 方法次数不大于 3 * 104

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <queue>
#include <vector>

// 因为线段树是一种近似于完全二叉树的结构，所以也可以用数组来存储线段
// 类似二叉堆

// 线段树节点 SegmentNode 除了记录左右节点的指针 left, right
// 之外，还要记录当前节点对应的索引区间 [l, r]
// 和区间内元素的聚合值（比如元素和、最大值等等）mergeValue。

// 查询时根据当前节点的索引区间和待查找的区间，判断是往左，往右，还是都要
// 别忘了在后序位置 需要重新计算聚合值！

// 更新操作需要注意，后序位置需要根据左右子节点的聚合值来更新当前节点的聚合值，这样才能保证被更新叶子节点的整条路径上的聚合值都被正确更新

// https://oi-wiki.org/ds/seg/
// 我不理解为什么堆式存储的节点上限是4n-5,

// 如果用堆式存储的话，数组不需要保存孩子指针，因为指针通过索引 *2 /2 来得到
// 其实区间信息也不需要存储，区间信息都是在递归的过程中计算出来的，这样的话，其实就只需要一个int数组来保存聚合值
//

// 就是heap啊
// 完全二叉树(complete binary tree): 完全二叉树（Complete Binary
// Tree）是一种特殊的二叉树结构，它在节点填充方式和存储效率上有独特优势，常用于实现堆（Heap）等高效数据结构。以下是其核心要点：

// 定义
// 完全二叉树的节点按从上到下、从左到右的顺序依次填充，满足：

// 除最后一层外，其他各层的节点数均达到最大值。

// 最后一层的节点必须连续集中在左侧，右侧可以有空缺，但不能在中间出现空缺。

// 结构紧凑

// 所有节点尽可能向左对齐，无中间空缺。

// 高度为 h 的完全二叉树，节点数范围：
// [2^(h-1), 2^h - 1]
// （例如，高度 3 的完全二叉树最少有 4 个节点，最多有 7 个节点）。

// 高效数组存储

// 完全二叉树可用数组（无需指针）高效存储，节点关系通过索引直接计算：

// 父节点索引：父节点 = i // 2（若索引从 1 开始）。

// 左子节点：左孩子 = 2i。

// 右子节点：右孩子 = 2i + 1。

// 与满二叉树的区别

// 满二叉树：所有层都填满节点，节点数严格为 2^h - 1。

// 完全二叉树：最后一层可不满，但必须左对齐。

class NumArray {
public:
  // BUG：所有传入的参数都是从0开始的，都需要加一
  NumArray(std::vector<int>& nums)
      : nums(nums), n(nums.size()), tree(4 * n, 0) {
    build(get_root(), 1, n + 1);
  }

  void update(int index, int val) {
    update_impl(get_root(), 1, n + 1, index + 1, val);
  }

  int sumRange(int left, int right) {
    // 将左闭右闭区间转换成左闭右开区间
    return query_impl(get_root(), 1, n + 1, left + 1, right + 2);
  }

  // 只需要提供一组模拟链式树的操作即可
  int left_child(int i) { return 2 * i; }
  int right_child(int i) { return left_child(i) + 1; }
  int get_root() { return 1; }
  int get_middle(int begin, int end) { return begin + (end - begin) / 2; }
  // 卧槽！不对，tree的索引和num的索引差1，因为tree的索引从1开始，0不使用
  // 而nums的索引从0开始的。。。

private:
  // 在nums的[begin,end)区间上创建一颗树，并将其聚合值写到root索引处
  void build(int root, int begin, int end) {
    // 递归结束条件
    // 感觉这里的判断是永远不会触发的
    // if (end <= begin) {
    //   return;
    // }
    // leaf, just copy the value in nums[begin] to tree[begin]
    if (end - begin == 1) {
      // BUG：注意到tree的索引下标从1开始，nums从0开始
      // BUG: 应该是tree[root] 而不是 tree[begin]
      tree[root] = nums[begin - 1];
      return;
    }

    int middle = begin + (end - begin) / 2;
    build(left_child(root), begin, middle);
    build(right_child(root), middle, end);
    // 这里的merge就是 add
    // dp?
    tree[root] = tree[left_child(root)] + tree[right_child(root)];
  }

  // 在子树root里，更新index的val，不对，这里仅仅给一个root，我无法确定区间的位置啊
  void update_impl(int root, int root_begin, int root_end, int index, int val) {
    // 递归结束条件, leaf
    int begin = index;
    int end = index + 1;
    if (begin == root_begin && end == root_end) {
      // 我发现，我们建完树之后，再也不需要nums了 其实
      // BUG: 这里同样应该是tree[root], tree always should be root!
      tree[root] = val;
      return;
    }

    int middle = get_middle(root_begin, root_end);
    if (end <= middle) {
      update_impl(left_child(root), root_begin, middle, index, val);
    }
    if (begin >= middle) {
      update_impl(right_child(root), middle, root_end, index, val);
    }
    tree[root] = tree[left_child(root)] + tree[right_child(root)];
  }

  // 在子树root上查找[begin, end)的聚合值，并返回
  // 我发现了，虽然某个节点代表的区间信息不保存在数组里了，但是在递归的时候，还是要有这个信息，所以就通过函数的参数计算并传递出来
  int query_impl(int root, int root_begin, int root_end, int query_begin,
                 int query_end) {
    // 递归结束条件
    if (root_begin == query_begin && root_end == query_end) {
      return tree[root];
    }

    int middle = get_middle(root_begin, root_end);
    // 分三种情况，完全在左边
    if (query_end <= middle) {
      return query_impl(left_child(root), root_begin, middle, query_begin,
                        query_end);
    }
    // 完全在右边
    if (query_begin >= middle) {
      return query_impl(right_child(root), middle, root_end, query_begin,
                        query_end);
    }
    // 横跨两边，需要分别再两边寻找，然后merge起来
    return query_impl(left_child(root), root_begin, middle, query_begin,
                      middle) +
           query_impl(right_child(root), middle, root_end, middle, query_end);
  }

  // 元素数量， nums.size()
  const std::vector<int>& nums;
  int n;
  // 堆式存储完全二叉树，需要使用4n个节点
  std::vector<int> tree;
};

/**
 * Your NumArray object will be instantiated and called as such:
 * NumArray* obj = new NumArray(nums);
 * obj->update(index,val);
 * int param_2 = obj->sumRange(left,right);
 */

TEST_CASE("NumArray example from problem description") {
  std::vector<int> nums = {1, 3, 5};
  NumArray numArray(nums);

  CHECK(numArray.sumRange(0, 2) == 9); // 1 + 3 + 5 = 9

  numArray.update(1, 2);               // nums becomes [1, 2, 5]
  CHECK(numArray.sumRange(0, 2) == 8); // 1 + 2 + 5 = 8
}

TEST_CASE("NumArray with single element") {
  std::vector<int> nums = {42};
  NumArray numArray(nums);

  CHECK(numArray.sumRange(0, 0) == 42);

  numArray.update(0, 100);
  CHECK(numArray.sumRange(0, 0) == 100);
}

TEST_CASE("NumArray with multiple elements and various ranges") {
  std::vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  NumArray numArray(nums);

  // Check full range
  CHECK(numArray.sumRange(0, 9) == 55);

  // Check partial ranges
  CHECK(numArray.sumRange(0, 4) == 15);
  CHECK(numArray.sumRange(5, 9) == 40);
  CHECK(numArray.sumRange(2, 7) == 33);

  // Check single positions
  CHECK(numArray.sumRange(3, 3) == 4);
  CHECK(numArray.sumRange(9, 9) == 10);

  // Update and check ranges
  numArray.update(4, 10); // Change 5 to 10
  CHECK(numArray.sumRange(0, 9) == 60);
  CHECK(numArray.sumRange(3, 5) == 20);

  // Multiple updates
  numArray.update(0, 100);
  numArray.update(9, 100);
  CHECK(numArray.sumRange(0, 9) == 249);
}

TEST_CASE("NumArray with negative numbers") {
  std::vector<int> nums = {-10, -5, 0, 5, 10};
  NumArray numArray(nums);

  CHECK(numArray.sumRange(0, 4) == 0);
  CHECK(numArray.sumRange(0, 1) == -15);
  CHECK(numArray.sumRange(3, 4) == 15);

  numArray.update(2, -20);
  CHECK(numArray.sumRange(0, 4) == -20);

  numArray.update(0, 15);
  numArray.update(1, 10);
  CHECK(numArray.sumRange(0, 2) == 5);
}

TEST_CASE("NumArray with sequential updates") {
  std::vector<int> nums = {1, 1, 1, 1, 1};
  NumArray numArray(nums);

  CHECK(numArray.sumRange(0, 4) == 5);

  numArray.update(0, 2);
  CHECK(numArray.sumRange(0, 4) == 6);

  numArray.update(1, 3);
  CHECK(numArray.sumRange(0, 4) == 8);

  numArray.update(2, 4);
  CHECK(numArray.sumRange(0, 4) == 11);

  numArray.update(3, 5);
  CHECK(numArray.sumRange(0, 4) == 15);

  numArray.update(4, 6);
  CHECK(numArray.sumRange(0, 4) == 20);
}
