// 2025/4/24
// zhangzhong
// https://leetcode.cn/problems/maximum-depth-of-binary-tree/description/

// 104. 二叉树的最大深度

// 给定一个二叉树 root ，返回其最大深度。

// 二叉树的 最大深度 是指从根节点到最远叶子节点的最长路径上的节点数。

// 示例 1：

// 输入：root = [3,9,20,null,null,15,7]
// 输出：3
// 示例 2：

// 输入：root = [1,null,2]
// 输出：2

// 提示：

// 树中节点的数量在 [0, 104] 区间内。
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

// 这道题目虽然简单，但是可以用两种思路来解决
// 熟悉一下应该是可以的，在比赛之前把二叉树的这些东西给学完就行了
// 感觉实在是没有时间了
// 第一种思路，就是遍历一遍二叉树，在遍历的过程中，修改全局变量，来解决问题
// 第二种思路，是把问题分解成子问题，这个时候，递归函数是通过返回值来沟通的，不使用全局变量
// 其实我这里的思路有点类似于第二种思路，dfs

class Solution {
public:
  // 咱们尝试写一下第一种思路
  // 就是单纯的遍历

  int depth = 0;
  int max_depth = 0;

  void traverse(TreeNode* root) {
    if (!root) {
      return;
    }

    // 是否碰到了叶子节点
    // 这里是前序的位置，也就是进入一个节点之前
    // 深度应该增加
    // 因为我们一定会进入
    depth++;

    // 还是在这里，我们应该判断是否为叶子
    if (!root->left && !root->right) {
      // update max depth
      max_depth = std::max(max_depth, depth);
    }

    traverse(root->left);
    traverse(root->right);

    // 这里是后序的位置，也就是离开节点的时候
    // 那么深度应该减少
    depth--;
  }

  //  这道题用dfs就挺快的
  // 保证root != nullptr
  int maxDepthDFS(TreeNode* root, int depth) {
    // 不对，不用试了，这肯定是在函数调用处判断
    // 因为我们的递归结束条件是叶子节点

    if (!root->left && !root->right) {
      return depth + 1;
    }

    int max_depth = 0;
    if (root->left) {
      max_depth = maxDepthDFS(root->left, depth + 1);
    }
    if (root->right) {
      max_depth = std::max(max_depth, maxDepthDFS(root->right, depth + 1));
    }
    return max_depth;
  }

  // 这道题 bfs竟然真的比dfs慢哎
  // 应该用dfs来做吧
  // 不过，dfs和bfs都能做，咱们还是都写一下吧
  int maxDepth(TreeNode* root) {
    // dfs，子问题的视角
    // 同样，dfs需要一个额外的函数来实现递归
    // if (!root) {
    //   return 0;
    // }

    // return maxDepthDFS(root, 0);

    // bfs
    // return maxDepthBFS(root);

    // dfs, 遍历的视角
    // 需要更新全局的状态
    depth = 0;
    max_depth = 0;
    traverse(root);
    return max_depth;
  }

  // 不过，咱们用bfs试一下呢？
  int maxDepthBFS(TreeNode* root) {

    // nullptr invariant
    if (!root) {
      return 0;
    }

    // put root in the q
    std::queue<TreeNode*> q;
    q.push(root);
    int depth = 1;

    int max_depth = 0;
    // level order traverse
    while (!q.empty()) {

      // get current depth node count
      int curr_depth_node_count = q.size();
      while (curr_depth_node_count-- > 0) {

        // get the pop
        TreeNode* node = q.front();
        q.pop();

        // check if leaf
        if (!node->left && !node->right) {
          // 虽然但是，我们这里不能提前退出
          // 必须找到所有的leaf
          max_depth = std::max(max_depth, depth);
        }
        if (node->left) {
          q.push(node->left);
        }
        if (node->right) {
          q.push(node->right);
        }
      }
      // 在遍历完一层之后，depth++
      depth++;
    }

    return max_depth;
  }

  // 人家这里的分解子问题的写法更好，更简洁
  // 可以看到，整体还是遍历的模板，只不过加了返回值
  // 分解问题，往往是在后序的时候解决，这个时候子问题是什么就很关键了
  // 这里的子问题是：子树的最大深度。子问题就是抽取出一颗子树来，只在这个上面操作，你能得到什么？
  // 或者说，只从子树的视角考虑问题，他和整个问题的关系是什么？问题，子问题，树，子树。从这样的角度来思考
  // 定义：输入一个节点，返回以该节点为根的二叉树的最大深度
  int maxDepthDFS2(TreeNode* root) {
    if (root == nullptr) {
      return 0;
    }
    // 利用定义，计算左右子树的最大深度
    int leftMax = maxDepth(root->left);
    int rightMax = maxDepth(root->right);

    // 根据左右子树的最大深度推出原二叉树的最大深度
    // 整棵树的最大深度等于左右子树的最大深度取最大值，
    // 然后再加上根节点自己
    return 1 + std::max(leftMax, rightMax);
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

TEST_CASE("Example 1: [3,9,20,null,null,15,7]") {
  // -1001 represents null
  std::vector<int> nodes = {3, 9, 20, -1001, -1001, 15, 7};
  TreeNode* root = createTreeFromArray(nodes);

  Solution solution;
  int depth = solution.maxDepth(root);

  CHECK(depth == 3);

  deleteTree(root);
}

TEST_CASE("Example 2: [1,null,2]") {
  // -1001 represents null
  std::vector<int> nodes = {1, -1001, 2};
  TreeNode* root = createTreeFromArray(nodes);

  Solution solution;
  int depth = solution.maxDepth(root);

  CHECK(depth == 2);

  deleteTree(root);
}

TEST_CASE("Empty tree") {
  TreeNode* root = nullptr;

  Solution solution;
  int depth = solution.maxDepth(root);

  CHECK(depth == 0);
}

TEST_CASE("Single node tree") {
  TreeNode* root = new TreeNode(1);

  Solution solution;
  int depth = solution.maxDepth(root);

  CHECK(depth == 1);

  deleteTree(root);
}

TEST_CASE("Unbalanced tree") {
  std::vector<int> nodes = {1, 2, -1001, 3, -1001, 4, -1001, 5};
  TreeNode* root = createTreeFromArray(nodes);

  Solution solution;
  int depth = solution.maxDepth(root);

  CHECK(depth == 5);

  deleteTree(root);
}
