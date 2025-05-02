// 2025/4/25
// zhangzhong
// https://labuladong.online/algo/essential-technique/binary-tree-summary
// 所有不是刷题的题目，但是我觉得有必要写下来的例子，都会放到这个文件里面

// https://labuladong.online/algo/essential-technique/binary-tree-summary/#%E5%90%8E%E5%BA%8F%E4%BD%8D%E7%BD%AE%E7%9A%84%E7%89%B9%E6%AE%8A%E4%B9%8B%E5%A4%84
// 这里的观察真的很有趣，理解的很好

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <algorithm>
#include <doctest/doctest.h>
#include <queue>
#include <sstream>
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

// 两个例子的代码
// 1. 如果把根节点看做第 1 层，如何打印出每一个节点所在的层数？
// 这个问题非常简单，而且有超多种做法，不过dfs应该是最快也是最简单的
void printNodeDepthImpl(TreeNode* root, int level) {
  if (!root) {
    return;
  }

  // 在前序位置打印即可
  std::cout << root->val << ": " << level << "\n";

  // 向下的时候，level增加
  printNodeDepthImpl(root->left, level + 1);
  printNodeDepthImpl(root->right, level + 1);
}

// calling
void printNodeDepthImpl(TreeNode* root) { printNodeDepthImpl(root, 1); }

// 还有一种写法，之前写过，需要一个全局的变量 depth，在前序的位置 + 1
// 在后序的位置 -1
// 也就是在进入一个节点的时候 + 1， 在离开一个节点的时候 -
int depth = 0;
void printNodeDepthImpl2(TreeNode* root) {
  if (!root) {
    return;
  }

  // 进入节点，深度+1
  depth++;

  // 打印节点
  std::cout << root->val << ": " << depth << "\n";

  printNodeDepthImpl2(root->left);
  printNodeDepthImpl2(root->right);

  // 后序位置
  // 离开节点，深度-1
  depth--;
}

TEST_CASE("Testing binary tree depth functions") {
  // Create a simple binary tree
  //       1
  //      / \
    //     2   3
  //    / \
    //   4   5
  TreeNode* root = new TreeNode(1);
  root->left = new TreeNode(2);
  root->right = new TreeNode(3);
  root->left->left = new TreeNode(4);
  root->left->right = new TreeNode(5);

  // Redirect cout to capture output
  std::stringstream buffer;
  std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

  SUBCASE("Test printNodeDepthImpl") {
    printNodeDepthImpl(root);
    std::string output = buffer.str();

    // Clear buffer for next test
    buffer.str("");

    // Check output contains expected depth information
    // Note: This is a simple check, actual format might differ
    CHECK(output.find("1: 1") != std::string::npos);
    CHECK(output.find("2: 2") != std::string::npos);
    CHECK(output.find("3: 2") != std::string::npos);
    CHECK(output.find("4: 3") != std::string::npos);
    CHECK(output.find("5: 3") != std::string::npos);
  }

  SUBCASE("Test printNodeDepthImpl2") {
    // Reset global depth before testing
    depth = 0;

    printNodeDepthImpl2(root);
    std::string output = buffer.str();

    // Check output contains expected depth information
    CHECK(output.find("1: 1") != std::string::npos);
    CHECK(output.find("2: 2") != std::string::npos);
    CHECK(output.find("3: 2") != std::string::npos);
    CHECK(output.find("4: 3") != std::string::npos);
    CHECK(output.find("5: 3") != std::string::npos);
  }

  // Restore cout
  std::cout.rdbuf(old);

  // Clean up the tree
  delete root->left->right;
  delete root->left->left;
  delete root->right;
  delete root->left;
  delete root;
}

// 第二个问题，输入一颗二叉树，返回这颗二叉树的节点总数
// 这个问题的解依赖于子树，所以很明显要在后序的位置解决
int countTreeNode(TreeNode* root) {
  if (!root) {
    return 0;
  }

  // 总的数量就是左子树的数量加上右子树的数量加上自己
  return 1 + countTreeNode(root->left) + countTreeNode(root->right);
}

TEST_CASE("Testing countTreeNode function") {
  // Create a simple binary tree
  //       1
  //      / \
    //     2   3
  //    / \
    //   4   5
  TreeNode* root = new TreeNode(1);
  root->left = new TreeNode(2);
  root->right = new TreeNode(3);
  root->left->left = new TreeNode(4);
  root->left->right = new TreeNode(5);

  // Test the count function
  int nodeCount = countTreeNode(root);
  CHECK(nodeCount == 5);

  // Test with empty tree
  CHECK(countTreeNode(nullptr) == 0);

  // Test with single node tree
  TreeNode* singleNode = new TreeNode(42);
  CHECK(countTreeNode(singleNode) == 1);
  delete singleNode;

  // Clean up the tree
  delete root->left->right;
  delete root->left->left;
  delete root->right;
  delete root->left;
  delete root;
}
