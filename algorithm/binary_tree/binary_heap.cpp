// 2025/4/19
// zhangzhong
// today，要做的就是先把几个数据库结构给补上，把所有的数据结构先复习一遍
// 然后直接看算法，就是解题的套路，然后直接刷题就行了
// 尽可能在比赛之前把题目给刷完，刷完一组题目，要去复习一下咱们的ACM的题目，就看一下就行了。
// 现在看的是速成的，比赛结束之后，根据这个目录，从前往后刷题就行了。
// 总共大概需要几个月的时间吧。
// ok

// use array
// 或许可以提供一个用数组保存二叉树的数据结构

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <chrono>
#include <doctest/doctest.h>
#include <queue>
#include <random>
#include <vector>

class ArrayBinaryTree {

public:
  using node_t = int;
  // 我们所有的值类型都是int

  // 一些比较重要的操作 包括获得child和parent
  // 这是有规律的
  // 我们使用一个新的类型表示节点的位置 其实就是索引
  // 我们用 node_t

  node_t root_node() const { return 1; }
  node_t parent_node(node_t child) const { return child / 2; }
  node_t left_child_node(node_t parent) const { return parent * 2; }
  node_t right_child_node(node_t parent) const {
    return this->left_child_node(parent) + 1;
  }

  // 我们可能删除元素，但是不应该重复分配内存
  // 而且这个逻辑最好由vector自己来维护

  // 但是对vecotr的操作 实际上只有 push_back 和 pop_back 而已啊
  // 还真是。。。
  // 那不如，直接获得data？
  // 还是咱们封装一下？还是封装一下
  // 因为咱们不用 index=0
  // 这个类就相当于一个vector的adaptor了，就像stack之于vector一样

  int& front() { return _data[1]; }
  int& back() { return _data.back(); }

  void push_back(int value) { _data.push_back(value); }

  void pop_back() { _data.pop_back(); }

  // 提供一个索引操作
  // operator[] 这个操作是比较重要的
  // 返回索引吧
  int& operator[](node_t index) { return _data[index]; }

  size_t size() const { return _data.size() - 1; }

  bool empty() const { return this->size() == 0; }

private:
  // 为了方便，我们内部使用vector
  // 下标从1开始
  std::vector<int> _data{0};
};

class BinaryHeap {
public:
  using node_t = ArrayBinaryTree::node_t;
  // 有两个操作比较重要
  // 一个是建堆 heapify 好像没有
  // 优先队列
  // 主要就是支持 push和 pop
  // 动态的维护最小元素 或者 最大元素

  // 当然所有的元素其实就是两个操作
  // fix down or fix up
  // 为什么？
  // 草，我都忘了二叉堆最重要的性质了
  // 你可以认为二叉堆是一种特殊的二叉树，这棵二叉树上的任意节点的值，都必须大于等于（或小于等于）其左右子树所有节点的值
  // 递归的，二叉堆的左右子树也是二叉堆
  //

  // 这是为了实现增加元素 push
  // 为什么要fixup？讲明白了，就ok了
  // fixup
  // 当我们向小顶堆中插入一个元素之后，放到最后一个位置上
  // 包含这个元素的最小子树可能不满足小顶堆的要求
  // 这个新的元素需要不断的上浮，直到满足小顶堆的要求
  // TODO: 找一下三本算法书上管这个方法到底叫什么
  void _fixup(node_t child) {
    // 如果有parent
    node_t parent = 0;
    while ((parent = _tree.parent_node(child)) != 0 &&
           _tree[child] < _tree[parent]) {
      // 交换child和parent，就是把这个节点上浮
      // 为什么这个上浮是合理的？
      // 因为在没有上浮之前，旧的parent和child是符合
      // 小顶堆的，他可能没有孩子，或者有一个孩子，这个孩子一定比parent大
      // 现在我们新的child比parent还小，所以它也一定比原来可能存在的孩子要小
      std::swap(_tree[child], _tree[parent]);
      // 更新child的index到parent的位置，继续上浮
      child = parent;
    }
  }

  // 而fixdown是为了支持删除元素 pop
  // 因为pop会删除堆顶的元素，也就是 node_index = 1
  // 这样就会剩下两个堆
  // 因为小顶堆的性质，任何堆的子堆都是小顶堆
  // 所以下一个最小的元素，一定是左右孩子中的一个
  // 我们把那个孩子提上来，但是下一个孩子就就没了顶，！递归！！！
  // 虽然是递归的，但是我们需要用迭代的方式来实现
  // 这相当于是一个空穴在向下移动
  // 这样的坏处是，这个空穴必须移动到最下面才行，也就是它不可能提前终止
  // 反之，我们把最后一个元素移动到空穴的位置，这样这个空穴会和左右孩子比较
  // 下沉到合适的位置，可能提前终止
  // 刚好和上面的操作相反！
  void _fixdown(node_t parent) {
    // 从parent开始，向下和孩子比较
    // 如果比孩子大，就要和孩子中的最小者交换。（为什么是最小者，因为要满足小顶堆的性质）
    // 除非我们已经是叶子了，或者我们比所有孩子都小，则停止

    while (_tree.left_child_node(parent) <= _tree.size()) {
      // not leaf

      // we at least have a left child

      // check if we have a right child

      node_t min_child = _tree.left_child_node(parent);
      // int min_value = _tree[left_child];

      node_t right_child = _tree.right_child_node(parent);
      if (right_child <= _tree.size() &&
          _tree[right_child] < _tree[min_child]) {
        // we have a right child
        // get the min child
        min_child = right_child;
        // min_value = _tree[right_child];
      }

      // compare the parent and the min child
      if (_tree[parent] < _tree[min_child]) {
        // we done
        break;
      }

      // we need to swap the child and the min child
      // and keep going
      std::swap(_tree[parent], _tree[min_child]);
      // update the parent to the min child
      parent = min_child;
    }
  }

  // 将一个元素放到对尾并返回其index
  void push(int value) {
    _tree.push_back(value);
    _fixup(_tree.size());
  }

  int& top() { return _tree.front(); }

  void pop() {
    // 把最后一个元素放到开头
    // 然后做fixdown
    _tree[_tree.root_node()] = _tree.back();
    _tree.pop_back();

    _fixdown(_tree.root_node());
  }

  // 我们有自己的size
  size_t size() const { return _tree.size(); }

private:
  ArrayBinaryTree _tree;
};

class PriorityQueue {

public:
  // logN
  void push(int x) {
    // 首先把元素push到对尾
    _heap.push(x);
  }

  // O1
  int top() { return _heap.top(); }

  // logN
  void pop() { _heap.pop(); }

  // O1
  size_t size() const { return _heap.size(); }

private:
  BinaryHeap _heap;
};

TEST_CASE("testing binary heap") {}

TEST_CASE("PriorityQueue validation against std::priority_queue") {
  SUBCASE("Basic operations") {
    PriorityQueue custom_pq;
    // std::priority_queue is max-heap by default, use greater<> to make it
    // min-heap
    std::priority_queue<int, std::vector<int>, std::greater<int>> std_pq;

    // Test empty queues
    CHECK(custom_pq.size() == 0);
    CHECK(std_pq.size() == 0);

    // Test push and top
    custom_pq.push(10);
    std_pq.push(10);
    CHECK(custom_pq.top() == std_pq.top());

    // Test with more elements
    std::vector<int> values = {5, 15, 3, 20, 8};
    for (int val : values) {
      custom_pq.push(val);
      std_pq.push(val);
    }

    CHECK(custom_pq.size() == std_pq.size());
    CHECK(custom_pq.top() == std_pq.top());

    // Test pop operations - elements should come out in same order
    while (!std_pq.empty()) {
      CHECK(custom_pq.top() == std_pq.top());
      custom_pq.pop();
      std_pq.pop();
    }

    CHECK(custom_pq.size() == 0);
  }

  SUBCASE("Random data test") {
    PriorityQueue custom_pq;
    std::priority_queue<int, std::vector<int>, std::greater<int>> std_pq;

    // Generate random data
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(-1000, 1000);

    const int NUM_ELEMENTS = 1000;
    for (int i = 0; i < NUM_ELEMENTS; i++) {
      int value = dist(gen);
      custom_pq.push(value);
      std_pq.push(value);
    }

    // Verify elements come out in same order
    while (!std_pq.empty()) {
      CHECK(custom_pq.top() == std_pq.top());
      custom_pq.pop();
      std_pq.pop();
    }
  }

  SUBCASE("Edge cases") {
    PriorityQueue custom_pq;
    std::priority_queue<int, std::vector<int>, std::greater<int>> std_pq;

    // Test with duplicates
    std::vector<int> values = {7, 7, 5, 5, 10, 10, 3, 3};
    for (int val : values) {
      custom_pq.push(val);
      std_pq.push(val);
    }

    while (!std_pq.empty()) {
      CHECK(custom_pq.top() == std_pq.top());
      custom_pq.pop();
      std_pq.pop();
    }

    // Test with extreme values
    values = {INT_MAX, INT_MIN, 0, 1000000, -1000000};
    for (int val : values) {
      custom_pq.push(val);
      std_pq.push(val);
    }

    while (!std_pq.empty()) {
      CHECK(custom_pq.top() == std_pq.top());
      custom_pq.pop();
      std_pq.pop();
    }
  }

  SUBCASE("Interleaved push/pop operations") {
    PriorityQueue custom_pq;
    std::priority_queue<int, std::vector<int>, std::greater<int>> std_pq;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> val_dist(-1000, 1000);
    std::uniform_int_distribution<> op_dist(0, 1); // 0 = push, 1 = pop

    // Start with some elements
    for (int i = 0; i < 50; i++) {
      int value = val_dist(gen);
      custom_pq.push(value);
      std_pq.push(value);
    }

    // Randomly push and pop
    for (int i = 0; i < 1000; i++) {
      if (op_dist(gen) == 0 || std_pq.empty()) {
        // Push operation
        int value = val_dist(gen);
        custom_pq.push(value);
        std_pq.push(value);
      } else {
        // Pop operation
        CHECK(custom_pq.top() == std_pq.top());
        custom_pq.pop();
        std_pq.pop();
      }

      // Verify sizes match
      CHECK(custom_pq.size() == std_pq.size());

      // If not empty, verify tops match
      if (!std_pq.empty()) {
        CHECK(custom_pq.top() == std_pq.top());
      }
    }
  }

  SUBCASE("Performance comparison") {
    // Generate random data
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(-100000, 100000);

    std::vector<int> values;
    const int NUM_ELEMENTS = 100000;
    for (int i = 0; i < NUM_ELEMENTS; i++) {
      values.push_back(dist(gen));
    }

    // Test std::priority_queue
    {
      auto start = std::chrono::high_resolution_clock::now();

      std::priority_queue<int, std::vector<int>, std::greater<int>> std_pq;
      for (int val : values) {
        std_pq.push(val);
      }

      while (!std_pq.empty()) {
        std_pq.pop();
      }

      auto end = std::chrono::high_resolution_clock::now();
      auto duration =
          std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
      MESSAGE("std::priority_queue time: " << duration.count() << "ms");
    }

    // Test custom PriorityQueue
    {
      auto start = std::chrono::high_resolution_clock::now();

      PriorityQueue custom_pq;
      for (int val : values) {
        custom_pq.push(val);
      }

      while (custom_pq.size() > 0) {
        custom_pq.pop();
      }

      auto end = std::chrono::high_resolution_clock::now();
      auto duration =
          std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
      MESSAGE("Custom PriorityQueue time: " << duration.count() << "ms");
    }
  }
}
