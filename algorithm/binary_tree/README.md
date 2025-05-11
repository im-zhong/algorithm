# 二叉树

1. DFS 算法常用来穷举所有路径，BFS 算法常用来寻找最短路径。 我的理解，dfs是一条路走到黑，而bfs是一圈一圈的往外找。
2. 递归遍历
3. // 二叉树的遍历框架 我觉得这里总结的非常好，简单的遍历，都是在函数的开头判空，因为这样写非常简单，或许这应该作为我们的默认写法，当发现不行的时候，再修改。
4. 着有点像 condition guard，也挺符合最佳实践的，从某种角度上
void traverse(TreeNode* root) {
    if (root == nullptr) {
        return;
    }
    // 前序位置
    traverse(root->left);
    // 中序位置
    traverse(root->right);
    // 后序位置
}

// N 叉树的遍历框架
void traverse(Node*root) {
    if (root == nullptr) {
        return;
    }
    // 前序位置
    for (Node* child : root->children) {
        traverse(child);
    }
    // 后序位置
}
5. 层序遍历
void levelOrderTraverse(Node*root) {
    if (root == nullptr) {
        return;
    }
    std::queue<Node*> q;
    q.push(root);
    // 记录当前遍历到的层数（根节点视为第 1 层）
    int depth = 1;

    while (!q.empty()) {
        int sz = q.size();
        for (int i = 0; i < sz; i++) {
            Node* cur = q.front();
            q.pop();
            // 访问 cur 节点，同时知道它所在的层数
            std::cout << "depth = " << depth << ", val = " << cur->val << std::endl;

            // !!! ⚠️注意到：其实只有遍历孩子节点的方式不一样，因为是多叉树，所以有多个孩子，其他的没有任何区别。
            for (Node* child : cur->children) {
                q.push(child);
            }
        }
        depth++;
    }
}
6. 二叉树的思维方式
    1、是否可以通过遍历一遍二叉树得到答案？如果可以，用一个 traverse 函数配合外部变量来实现，这叫「遍历」的思维模式。
    2、是否可以定义一个递归函数，通过子问题（子树）的答案推导出原问题的答案？如果可以，写出这个递归函数的定义，并充分利用这个函数的返回值，这叫「分解问题」的思维模式。
7. 思考的方式：如果单独抽出一个二叉树节点，它需要做什么事情？需要在什么时候（前/中/后序位置）做？确实，用递归的，整体的思想去理解二叉树，是更容易的。
8. 如果你告诉我，快速排序就是个二叉树的前序遍历，归并排序就是个二叉树的后序遍历，那么我就知道你是个算法高手了。
    1. 对于归并排序，他是把两个数组都排得有序了之后，再合并。可以理解成两个子问题都解决了，最后再解决合并的问题，有点类似于后序遍历的时候，先遍历左子树和右子树，在遍历根
    2. 对于快排，他是先找到一个pivot，这个pivot会把整个问题分解成两个子问题，不断的做这个事情。有点类似于先序遍历的时候，先遍历根节点，再遍历左子树和右子树。
9. 你也注意到了，只要是递归形式的遍历，都可以有前序位置和后序位置，分别在递归之前和递归之后。所谓前序位置，就是刚进入一个节点（元素）的时候，后序位置就是即将离开一个节点（元素）的时候，那么进一步，你把代码写在不同位置，代码执行的时机也不同：

10. 这个观察真的很有意思：仔细观察，前中后序位置的代码，能力依次增强。

前序位置的代码只能从函数参数中获取父节点传递来的数据。

中序位置的代码不仅可以获取参数数据，还可以获取到左子树通过函数返回值传递回来的数据。

后序位置的代码最强，不仅可以获取参数数据，还可以同时获取到左右子树通过函数返回值传递回来的数据。

所以，某些情况下把代码移到后序位置效率最高；有些事情，只有后序位置的代码能做。
只有后序位置才能通过返回值获取子树的信息。

那么换句话说，一旦你发现题目和子树有关，那大概率要给函数设置合理的定义和返回值，在后序位置写代码了。

11. 兄弟，你的理解真的很到胃！！！
// 回溯算法框架
void backtrack(...) {
    // base case
    if (...) return;

    for (int i = 0; i < ...; i++) {
        // 做选择
        ...
        // 这里其实就是前序，是进入下一层决策树的地方
        // 类似于 depth++

        // 进入下一层决策树
        backtrack(...);

        // 撤销刚才做的选择
        ...
        // 这里就是后序，是离开当前的决策树的地方
        // 类似于 depth--
    }
}

12. dfs与回溯
// DFS 算法把「做选择」「撤销选择」的逻辑放在 for 循环外面
void dfs(Node*root) {
    if (!root) return;
    // 做选择
    printf("enter node %s\n", root->val.c_str());
    for (Node* child : root->children) {
        dfs(child);
    }
    // 撤销选择
    printf("leave node %s\n", root->val.c_str());
}

// 回溯算法把「做选择」「撤销选择」的逻辑放在 for 循环里面
void backtrack(Node*root) {
    if (!root) return;
    for (Node* child : root->children) {
        // 做选择
        printf("I'm on the branch from %s to %s\n", root->val.c_str(), child->val.c_str());
        backtrack(child);
        // 撤销选择
        printf("I'll leave the branch from %s to %s\n", child->val.c_str(), root->val.c_str());
    }
}

13. 多叉树我也放到这里了，包括各种树的变种，trie树等等都放到这里，
