// 2025/5/5
// zhangzhong
// segment tree
// https://cp-algorithms.com/data_structures/segment_tree.html
// https://oi-wiki.org/ds/seg/
// https://labuladong.online/algo/data-structure-basic/segment-tree-basic/

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <algorithm>
#include <cassert>
#include <doctest/doctest.h>
#include <functional>
#include <queue>
#include <vector>

class SegmentTree {
  // 构造函数，给定一个数组，初始化线段树，时间复杂度 O(N)
  // merge 是一个函数，用于定义 query 方法的行为
  // 通过修改这个函数，可以让 query 函数返回区间的元素和、最大值、最小值等
public:
  SegmentTree(std::vector<int> nums, std::function<int(int, int)> merge)
      : nums(nums), merge(merge) {
    // 第一步应该是建树，今天写不完了，明天再写吧。
    // 二分递归建树即可
    root = create_segment_tree(0, nums.size());
  }

  // 采用内部类的方式实现树，因为外部其实并不知道内部有一棵树，没必要暴露出来
  struct TreeNode {
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;
    // 每个节点，包括叶子节点，都要保存一个区间
    int begin = 0;
    int end = 0;
    // 每个节点，包括叶子节点，都要保存本区间的聚合值，和，最值，等等
    int value = -1;
  };

  // 查询闭区间 [i, j] 的元素和（也可能是最大最小值，取决于 merge
  // 函数），时间复杂度 O(logN)
  // 这是一个闭区间，别忘了转换成开区间
  int query(int i, int j) {
    // 查找和更新都可以写成递归的
    // 咱们先写递归的吧
    // 总共就三种情况，区间完全在左边，区间完全在右边，区间横跨root节点
    return query_impl(i, j + 1, root);
  }

  // 这个query不用递归写不太好写
  int query_impl(int begin, int end, TreeNode* root) {
    assert(root);
    assert(begin >= root->begin && end <= root->end);

    // 递归结束条件，刚好命中
    if (begin == root->begin && end == root->end) {
      return root->value;
    }

    // BUG: 这里应该计算的root的middle！！！
    int middle = root->begin + (root->end - root->begin) / 2;
    // 一种比较好的写法是返回一个对merge操作没有影响的默认值
    // 但是这是在知道merge操作的情况下，可以写，这里不知道，只能做非常多的边界判断
    if (end <= middle) {
      // 如果完全在左边
      assert(root->left);
      return query_impl(begin, end, root->left);
    }

    if (begin >= middle) {
      assert(root->right);
      return query_impl(begin, end, root->right);
    }

    assert(root->left);
    assert(root->right);
    return merge(query_impl(begin, middle, root->left),
                 query_impl(middle, end, root->right));
  }

  // 更新 nums[i] = val，时间复杂度 O(logN)

  void update(int i, int val) {
    // 这里同样需要用递归来写，并且是后序的
    // 先向下找到i 所在的区间，然后修改其值，并返回新的值
    // 然后向上，递归的修改值, 知道根节点
    update_impl(i, val, root);
  }

  // 修改root中 i, i+1 区间的值，并返回新的聚合值
  int update_impl(int i, int val, TreeNode* root) {

    // 感觉有返回值的递归没法这么写
    // 必须在进入递归之前判断是否为空
    // 因为一旦进入了一个空节点，我们不知道要返回什么
    // 除非返回一个对merge操作没有影响的数字，才ok
    // if (!root) {
    //   return -1;
    // }

    assert(root);

    // 寻找区间 [i, i+1)
    if (root->begin == i && root->end == i + 1) {
      nums[i] = val;
      root->value = val;
      return val;
    }

    int begin = i;
    int end = begin + 1;
    // 因为这个区间只有一个数，那么他绝对不可能跨越到两个子树上
    // 所以他要么在左边 要么在右边
    int middle = root->begin + (root->end - root->begin) / 2;
    // 不对！这里实现的不对！！！如果end = middle, begin = middle
    // 应该在哪边？是对的，因为我们的区间[begim,
    // end)一定是非空的，所以下面的两个条件一定只有可能有一个成立
    if (end <= middle) {
      assert(root->left);
      int updated_value = update_impl(i, val, root->left);
      // BUG: == 写成了 =, 编译器应该检查这样的错误才对！
      // 首先禁止任何隐式类型转换，然后assert中的表达式必须是布尔表达式才对！
      assert(updated_value == root->left->value);

      // 这是root 的左孩子改了，那么root自己的值也要改
      // 但是有一种情况，就是我们没有右孩子，这个时候要怎么计算merge的结果？
      // 傻逼吗？没有右孩子，节点的值就是左孩子的值啊
      if (root->right) {
        root->value = merge(root->left->value, root->right->value);
      } else {
        root->value = root->left->value;
      }
      return root->value;
    }

    if (begin >= middle) {
      assert(root->left);
      assert(root->right);

      int updated_value = update_impl(i, val, root->right);
      root->value = merge(root->left->value, root->right->value);
      return root->value;
    }

    assert(false);
  }

private:
  TreeNode* create_segment_tree(int begin, int end) {
    // 对于每个区间，我们同样采用[) 的方式
    // 因为是建树，我们用字问题的方式，函数会递归的返回建立好的树的根节点

    // todo: 递归结束条件
    // 有可能分配到空的区间，直接返回空指针即可，表示没有子区间了
    if (end <= begin) {
      return nullptr;
    }

    // end > begin
    if (end - begin == 1) {
      // 区间中只有一个值的时候
      // 就是叶子节点

      return new TreeNode{nullptr, nullptr, begin, end, nums[begin]};
    }

    int middle = begin + (end - begin) / 2;

    TreeNode* node = new TreeNode{};
    node->left = create_segment_tree(begin, middle);
    node->right = create_segment_tree(middle, end);

    node->begin = begin;
    node->end = end;
    node->value = merge(node->left->value, node->right->value);
    return node;
  }

  TreeNode* root;

  std::vector<int> nums;
  std::function<int(int, int)> merge;
};

TEST_CASE("SegmentTree with sum operation") {
  std::vector<int> nums = {1, 3, 5, 7, 9, 11};
  auto sum = [](int a, int b) { return a + b; };
  SegmentTree tree(nums, sum);

  // Test queries
  CHECK(tree.query(0, 0) == 1);
  CHECK(tree.query(1, 1) == 3);
  CHECK(tree.query(0, 1) == 4);
  CHECK(tree.query(0, 5) == 36);
  CHECK(tree.query(2, 4) == 21);

  // Test updates
  tree.update(0, 10);
  CHECK(tree.query(0, 0) == 10);
  CHECK(tree.query(0, 5) == 45);

  tree.update(3, 20);
  CHECK(tree.query(3, 3) == 20);
  CHECK(tree.query(2, 4) == 34);
  CHECK(tree.query(0, 5) == 58);
}

TEST_CASE("SegmentTree with max operation") {
  std::vector<int> nums = {5, 2, 7, 1, 9, 3};
  auto max_func = [](int a, int b) { return std::max(a, b); };
  SegmentTree tree(nums, max_func);

  // Test queries
  CHECK(tree.query(0, 0) == 5);
  CHECK(tree.query(0, 2) == 7);
  CHECK(tree.query(3, 5) == 9);
  CHECK(tree.query(0, 5) == 9);
  CHECK(tree.query(1, 3) == 7);

  // Test updates
  tree.update(0, 10);
  CHECK(tree.query(0, 2) == 10);
  CHECK(tree.query(0, 5) == 10);

  tree.update(4, 12);
  CHECK(tree.query(3, 5) == 12);
  CHECK(tree.query(0, 5) == 12);
}

TEST_CASE("SegmentTree with min operation") {
  std::vector<int> nums = {8, 2, 5, 1, 9, 3};
  auto min_func = [](int a, int b) { return std::min(a, b); };
  SegmentTree tree(nums, min_func);

  // Test queries
  CHECK(tree.query(0, 0) == 8);
  CHECK(tree.query(0, 2) == 2);
  CHECK(tree.query(3, 5) == 1);
  CHECK(tree.query(0, 5) == 1);
  CHECK(tree.query(1, 2) == 2);

  // Test updates
  tree.update(3, 10);
  CHECK(tree.query(3, 5) == 3);
  CHECK(tree.query(0, 5) == 2);

  tree.update(1, 0);
  CHECK(tree.query(0, 2) == 0);
  CHECK(tree.query(0, 5) == 0);
}

TEST_CASE("SegmentTree with single element") {
  std::vector<int> nums = {42};
  auto sum = [](int a, int b) { return a + b; };
  SegmentTree tree(nums, sum);

  CHECK(tree.query(0, 0) == 42);

  tree.update(0, 100);
  CHECK(tree.query(0, 0) == 100);
}

TEST_CASE("SegmentTree with arbitrary operation") {
  std::vector<int> nums = {1, 2, 3, 4, 5};
  // A custom operation that multiplies numbers
  auto multiply = [](int a, int b) { return a * b; };
  SegmentTree tree(nums, multiply);

  CHECK(tree.query(0, 4) == 120);
  CHECK(tree.query(1, 3) == 24);

  tree.update(2, 6);
  CHECK(tree.query(0, 4) == 240);
  CHECK(tree.query(2, 4) == 120);
}
