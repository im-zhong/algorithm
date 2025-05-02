// 2025/4/25
// zhangzhong
// https://leetcode.cn/problems/diameter-of-binary-tree/description/

// 543. 二叉树的直径

// 给你一棵二叉树的根节点，返回该树的 直径 。

// 二叉树的 直径 是指树中任意两个节点之间最长路径的 长度
// 。这条路径可能经过也可能不经过根节点 root 。

// 两节点之间路径的 长度 由它们之间边数表示。

// 示例 1：

// 输入：root = [1,2,3,4,5]
// 输出：3
// 解释：3 ，取路径 [4,2,1,3] 或 [5,2,1,3] 的长度。
// 示例 2：

// 输入：root = [1,2]
// 输出：1

// 提示：

// 树中节点数目在范围 [1, 104] 内
// -100 <= Node.val <= 100

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <algorithm>
#include <doctest/doctest.h>
#include <queue>
#include <vector>

struct TreeNode {

  int val;
  TreeNode* left;
  TreeNode* right;
  TreeNode() : val(0), left(nullptr), right(nullptr) {}
  TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
  TreeNode(int x, TreeNode* left, TreeNode* right)
      : val(x), left(left), right(right) {}
};

class Solution {
public:
  // int max_depth = 0;
  int max_diameter = 0;

  // 这道题明显是后序，且明显要从子问题的视角来考虑
  // 树的直径就是左子树的最大深度 + 右子树的最大深度
  // 明显，这里的子问题就是树的最大深度
  // 而树的最大深度 = 1（root)  + max(left, right)
  // 而我们在哪里计算这个直径呢？当然是后序的位置
  // 现在我对这道题目的理解比一开始更加的深入了
  int diameterOfBinaryTree(TreeNode* root) {
    maxDepth(root);
    return max_diameter;
  }

  // I need a function to calculate maxDepth
  int maxDepth(TreeNode* root) {
    if (!root) {
      return 0;
    }

    int left_max = maxDepth(root->left);
    int right_max = maxDepth(root->right);

    // 在后序的位置计算直径
    int diameter = left_max + right_max;
    // 比较并更新最大的diameter
    max_diameter = std::max(max_diameter, diameter);

    return 1 + std::max(left_max, right_max);
  }
};

// Helper function to create a binary tree from level order traversal array
TreeNode* createTreeFromArray(const std::vector<int>& nodes) {
  if (nodes.empty())
    return nullptr;

  TreeNode* root = new TreeNode(nodes[0]);
  std::queue<TreeNode*> q;
  q.push(root);

  int i = 1;
  while (!q.empty() && i < nodes.size()) {
    TreeNode* current = q.front();
    q.pop();

    // Left child
    if (i < nodes.size() &&
        nodes[i] != -1001) { // Using -1001 to represent null
      current->left = new TreeNode(nodes[i]);
      q.push(current->left);
    }
    i++;

    // Right child
    if (i < nodes.size() &&
        nodes[i] != -1001) { // Using -1001 to represent null
      current->right = new TreeNode(nodes[i]);
      q.push(current->right);
    }
    i++;
  }

  return root;
}

// Helper function to free memory used by the tree
void deleteTree(TreeNode* root) {
  if (root == nullptr)
    return;
  deleteTree(root->left);
  deleteTree(root->right);
  delete root;
}

TEST_CASE("Example 1: [1,2,3,4,5]") {
  // Create the tree for example 1:
  //     1
  //    / \
    //   2   3
  //  / \
    // 4   5
  std::vector<int> nodes = {1, 2, 3, 4, 5};
  TreeNode* root = createTreeFromArray(nodes);

  Solution solution;
  int diameter = solution.diameterOfBinaryTree(root);

  CHECK(diameter == 3);

  deleteTree(root);
}

TEST_CASE("Example 2: [1,2]") {
  // Create the tree for example 2:
  //     1
  //    /
  //   2
  std::vector<int> nodes = {1, 2};
  TreeNode* root = createTreeFromArray(nodes);

  Solution solution;
  int diameter = solution.diameterOfBinaryTree(root);

  CHECK(diameter == 1);

  deleteTree(root);
}

TEST_CASE("Single node tree") {
  TreeNode* root = new TreeNode(1);

  Solution solution;
  int diameter = solution.diameterOfBinaryTree(root);

  CHECK(diameter == 0);

  delete root;
}

TEST_CASE("Balanced tree with same depth in both subtrees") {
  // Create a balanced tree:
  //      1
  //     / \
    //    2   3
  //   / \ / \
    //  4  5 6  7
  std::vector<int> nodes = {1, 2, 3, 4, 5, 6, 7};
  TreeNode* root = createTreeFromArray(nodes);

  Solution solution;
  int diameter = solution.diameterOfBinaryTree(root);

  CHECK(diameter == 4);

  deleteTree(root);
}
