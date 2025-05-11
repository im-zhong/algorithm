// 2025/5/10
// zhangzhong
// https://leetcode.cn/problems/kth-smallest-element-in-a-sorted-matrix/description/

// 378. 有序矩阵中第k小的元素

// 给你一个 n x n 矩阵 matrix ，其中每行和每列元素均按升序排序，找到矩阵中第 k
// 小的元素。 请注意，它是 排序后 的第 k 小元素，而不是第 k 个 不同 的元素。

// 你必须找到一个内存复杂度优于 O(n2) 的解决方案。

// 示例 1：

// 输入：matrix = [[1,5,9],[10,11,13],[12,13,15]], k = 8
// 输出：13
// 解释：矩阵中的元素为 [1,5,9,10,11,12,13,13,15]，第 8 小元素是 13
// 示例 2：

// 输入：matrix = [[-5]], k = 1
// 输出：-5

// 提示：

// n == matrix.length
// n == matrix[i].length
// 1 <= n <= 300
// -109 <= matrix[i][j] <= 109
// 题目数据 保证 matrix 中的所有行和列都按 非递减顺序 排列
// 1 <= k <= n2

// 进阶：

// 你能否用一个恒定的内存(即 O(1) 内存复杂度)来解决这个问题?
// 你能在 O(n)
// 的时间复杂度下解决这个问题吗?这个方法对于面试来说可能太超前了，但是你会发现阅读这篇文章（
// this paper ）很有趣。

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <queue>
#include <vector>

class Solution {
public:
  struct MatrixIndex {
    int row;
    int col;
  };

  int kthSmallest(std::vector<std::vector<int>>& matrix, int k) {
    // 链表合并啊，太简单了兄弟
    // 每一行就是一个有序链表
    // 咱们用一个数组来保存最终的结果就行了
    // 根本用不到这个nums啊 我们只需要第k个而已
    // std::vector<int> nums;
    int kth_num = 0;
    int curr = 0;
    int n = matrix.size();
    // 1 <= k <= n^2 . 所以k一定是有效的

    // k个链表用优先队列来排序就行了，so easy
    // 默认是大顶堆，需要使用小顶堆 因为需要从小到大排序
    // 哦 这是一个方阵 n * n 的
    // 1 <= n <= 300
    // 所以每行至少有一个元素

    auto matrix_greater = [&matrix](const MatrixIndex& lhs,
                                    const MatrixIndex& rhs) -> bool {
      return matrix[lhs.row][lhs.col] > matrix[rhs.row][rhs.col];
    };
    std::priority_queue<MatrixIndex, std::vector<MatrixIndex>,
                        decltype(matrix_greater)>
        pq(matrix_greater);

    // 我们需要k个链表指针，用来指示链表中的位置
    // 也就是每一行一个指针被

    // 在最开始的时候，把所有行的第一个元素加入到pq中
    // vector的迭代器会失效，因为vector的迭代器就是指针，在重新分配内存之后就invalid了
    // 优先队列需要保存两个数值，一个是行数，一个是列数
    // row col
    for (int row = 0; row < n; row++) {
      int col = 0;
      pq.push(MatrixIndex{row, col});
    }

    while (curr < k && !pq.empty()) {
      MatrixIndex mi = pq.top();
      pq.pop();

      // nums.push_back(matrix[mi.row][mi.col]);
      kth_num = matrix[mi.row][mi.col];
      curr++;

      mi.col++;
      if (mi.col < n) {
        // 不能越界啊
        pq.push(mi);
      }
    }

    // assert nums.size() == k
    // return nums.back();
    return kth_num;
  }
};

TEST_CASE("Example 1: 3x3 matrix with k=8") {
  std::vector<std::vector<int>> matrix = {
      {1, 5, 9}, {10, 11, 13}, {12, 13, 15}};
  int k = 8;

  Solution solution;
  int result = solution.kthSmallest(matrix, k);

  CHECK(result == 13);
}

TEST_CASE("Example 2: 1x1 matrix with k=1") {
  std::vector<std::vector<int>> matrix = {{-5}};
  int k = 1;

  Solution solution;
  int result = solution.kthSmallest(matrix, k);

  CHECK(result == -5);
}

TEST_CASE("2x2 matrix with k=1") {
  std::vector<std::vector<int>> matrix = {{1, 2}, {3, 4}};
  int k = 1;

  Solution solution;
  int result = solution.kthSmallest(matrix, k);

  CHECK(result == 1);
}

TEST_CASE("2x2 matrix with k=4") {
  std::vector<std::vector<int>> matrix = {{1, 2}, {3, 4}};
  int k = 4;

  Solution solution;
  int result = solution.kthSmallest(matrix, k);

  CHECK(result == 4);
}

TEST_CASE("3x3 matrix with duplicate values") {
  std::vector<std::vector<int>> matrix = {{1, 3, 5}, {3, 5, 7}, {5, 7, 9}};
  int k = 5;

  Solution solution;
  int result = solution.kthSmallest(matrix, k);

  CHECK(result == 5);
}

TEST_CASE("3x3 matrix with negative values") {
  std::vector<std::vector<int>> matrix = {{-10, -5, 0}, {-3, 1, 5}, {4, 7, 10}};
  int k = 4;

  Solution solution;
  int result = solution.kthSmallest(matrix, k);

  CHECK(result == 0);
}
