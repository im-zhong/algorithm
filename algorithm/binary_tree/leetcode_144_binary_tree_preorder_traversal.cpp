// 2025/4/25
// zhangzhong
// https://leetcode.cn/problems/binary-tree-preorder-traversal/description/

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
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
  std::vector<int> preorder;

  std::vector<int> preorderTraversal(TreeNode* root) {

    // 这个太简单了
    // 不过也有两种写法
    // 一种就是遍历一遍🌲
    // 而这里是需要一个额外的辅助函数的，因为我们需要一个全局变量
    // 或者函数参数里面的一个可变引用
    // preorder.clear();
    // traverse(root);
    // return preorder;

    // 还有一种就是从子问题的角度来看待，也就是不准使用一个额外的辅助函数

    // return {};

    // 不过这样写慢了很多，毕竟涉及到很多的vector的操作，复制啥的，都有可能会让程序变慢
    if (!root) {
      return {};
    }

    // 前序遍历的节点
    // 就是根节点+ 左子树的结果 + 右子树的结果
    std::vector<int> result;
    result.push_back(root->val);
    for (auto val : preorderTraversal(root->left)) {
      result.push_back(val);
    }
    for (auto val : preorderTraversal(root->right)) {
      result.push_back(val);
    }
    return result;
  }

  void traverse(TreeNode* root) {
    if (!root) {
      return;
    }

    preorder.push_back(root->val);
    traverse(root->left);
    traverse(root->right);
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

TEST_CASE("Example 1: [1,null,2,3]") {
  // -1001 represents null
  std::vector<int> nodes = {1, -1001, 2, 3};
  TreeNode* root = createTreeFromArray(nodes);

  Solution solution;
  std::vector<int> result = solution.preorderTraversal(root);
  std::vector<int> expected = {1, 2, 3};

  CHECK(result == expected);

  deleteTree(root);
}

TEST_CASE("Empty tree") {
  TreeNode* root = nullptr;

  Solution solution;
  std::vector<int> result = solution.preorderTraversal(root);
  std::vector<int> expected = {};

  CHECK(result == expected);
}

TEST_CASE("Single node tree") {
  TreeNode* root = new TreeNode(1);

  Solution solution;
  std::vector<int> result = solution.preorderTraversal(root);
  std::vector<int> expected = {1};

  CHECK(result == expected);

  deleteTree(root);
}

TEST_CASE("Balanced tree") {
  std::vector<int> nodes = {1, 2, 3, 4, 5, 6, 7};
  TreeNode* root = createTreeFromArray(nodes);

  Solution solution;
  std::vector<int> result = solution.preorderTraversal(root);
  std::vector<int> expected = {1, 2, 4, 5, 3, 6, 7};

  CHECK(result == expected);

  deleteTree(root);
}

TEST_CASE("Left-skewed tree") {
  std::vector<int> nodes = {1, 2, -1001, 3, -1001, 4};
  TreeNode* root = createTreeFromArray(nodes);

  Solution solution;
  std::vector<int> result = solution.preorderTraversal(root);
  std::vector<int> expected = {1, 2, 3, 4};

  CHECK(result == expected);

  deleteTree(root);
}
