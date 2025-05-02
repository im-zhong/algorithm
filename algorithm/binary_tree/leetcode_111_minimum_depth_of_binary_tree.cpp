// 2025/4/24
// zhangzhong
// https://leetcode.cn/problems/minimum-depth-of-binary-tree/description/

// 给定一个二叉树，找出其最小深度。

// 最小深度是从根节点到最近叶子节点的最短路径上的节点数量。

// 说明：叶子节点是指没有子节点的节点。

// 示例 1：

// 输入：root = [3,9,20,null,null,15,7]
// 输出：2
// 示例 2：

// 输入：root = [2,null,3,null,4,null,5,null,6]
// 输出：5

// 提示：

// 树中节点数的范围在 [0, 105] 内
// -1000 <= Node.val <= 1000

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
  // 这道题目应该dfs和bfs都能做
  // 咱们都试一下

  // TIP：有的题目在递归调用处判断是否非空
  // 有的在函数内部最开始判断
  // 两种情况需要判断，灵活使用
  int minDepthDFS(TreeNode* root, int depth) {
    // 我们必须在调用处保证，root!=nullptr
    // if (!root) {
    //   return depth;
    // }

    // check if leaf
    if (!root->left && !root->right) {
      return depth + 1;
    }

    // 我们需要寻找的是最小的深度
    // 那么最小的深度当然就是左子树或者右子树的最小深度
    // 只有当我们是叶子节点的时候, 我们才是深度

    // 不对，这样写是不对的！什么时候才是一个有效的深度
    // 是一个没有子节点的节点，这样的话
    // 只要有一个没有分支没有，就会返回
    // 比如我们有一个根节点，他只有左节点
    // 那么在进入下面的函数之后，他会因为没有右孩子而返回1
    // 但是这并不是深度
    // 这里的错误来源于对递归结束条件的错误理解

    // 这里还是不对，想象一个实际上是右右孩子组成的链表的一棵树
    // 他永远没有左孩子
    // 所以左孩子的递归分支会直接返回
    // 返回的就是当前节点的深度，这是错误的！
    // 所以我们需要做一个判断，如果孩子不存在，就不应该递归了
    // 为了简化逻辑
    // 我们一开始为左右子树设置一个假的深度，就是一个超大的深度
    // 这样在之后的min 操作 就会被弃掉

    // 这里保证，一定有孩子
    int left_tree_depth = INT_MAX;
    int right_tree_depth = INT_MAX;
    if (root->left) {
      left_tree_depth = minDepthDFS(root->left, depth + 1);
    }
    if (root->right) {
      right_tree_depth = minDepthDFS(root->right, depth + 1);
    }
    return std::min(left_tree_depth, right_tree_depth);

    // return std::min(minDepthDFS(root->left, depth + 1),
    //                 minDepthDFS(root->right, depth + 1));
  }

  // dfs

  int minDepth(TreeNode* root) {
    // dfs
    // if (!root) {
    //   return 0;
    // }
    // // dfs要写递归
    // // 在参数里面要写上当前的深度
    // // 所以必须额外定义一个函数
    // return minDepthDFS(root, 0);

    // bfs
    return minDepthBFS(root);
  }

  // 而且这道题用bfs做的比dfs快得多！
  // 因为dfs无法提前停止，他必须遍历完所有的路径才知道最小深度
  // 但是bfs可以提前停止，在碰到第一个叶子的时候，就知道最小深度了！
  // 原来如此！

  // bfs 做一下
  // 感觉这道题直接套bfs的模板做就行 当然是带深度的
  int minDepthBFS(TreeNode* root) {
    // root invariant
    if (!root)
      return 0;

    // push root node into the queue
    std::queue<TreeNode*> q;
    q.push(root);
    // 最开始就让深度为1 因为我们知道一定有一个root
    int depth = 1;

    // level order traverse
    while (!q.empty()) {
      // get q size
      // this the current level's node size/
      // 必须在这里拿这一层的节点数量，因为在下面的循环中，我们会向队列中增加下一层的节点，就不对了
      int curr_level_node_size = q.size();

      // 只遍历当前层节点数量个
      while (curr_level_node_size-- > 0) {
        // get then pop
        TreeNode* node = q.front();
        q.pop();

        // 把所有孩子都放到队列里面
        // 判断自己是不是叶子节点
        // 这道题用bfs可以提前退出
        // 因为是层序遍历，所以碰到的第一个叶子节点一定是深度最小的叶子节点
        // int child_count = 0;
        // if (node->left) {
        //   child_count++;
        //   q.push(node->left);
        // }
        // if (node->right) {
        //   child_count++;
        //   q.push(node->right);
        // }
        // if (child_count == 0) {
        //   return depth;
        // }
        // 这里人家给的写法更好
        if (!node->left && !node->right) {
          return depth;
        }
        if (node->left) {
          q.push(node->left);
        }
        if (node->right) {
          q.push(node->right);
        }
      }

      // 遍历完一层之后。让深度++
      depth++;
    }

    return depth;
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
  int depth = solution.minDepth(root);

  CHECK(depth == 2);

  deleteTree(root);
}

TEST_CASE("Example 2: [2,null,3,null,4,null,5,null,6]") {
  // -1001 represents null
  std::vector<int> nodes = {2, -1001, 3, -1001, 4, -1001, 5, -1001, 6};
  TreeNode* root = createTreeFromArray(nodes);

  Solution solution;
  int depth = solution.minDepth(root);

  CHECK(depth == 5);

  deleteTree(root);
}

TEST_CASE("Empty tree") {
  TreeNode* root = nullptr;

  Solution solution;
  int depth = solution.minDepth(root);

  CHECK(depth == 0);
}

TEST_CASE("Single node tree") {
  TreeNode* root = new TreeNode(1);

  Solution solution;
  int depth = solution.minDepth(root);

  CHECK(depth == 1);

  deleteTree(root);
}

TEST_CASE("Balanced tree with same depth in all paths") {
  std::vector<int> nodes = {1, 2, 3, 4, 5, 6, 7};
  TreeNode* root = createTreeFromArray(nodes);

  Solution solution;
  int depth = solution.minDepth(root);

  CHECK(depth == 3);

  deleteTree(root);
}
