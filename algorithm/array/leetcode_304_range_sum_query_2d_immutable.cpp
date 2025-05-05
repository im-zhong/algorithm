// 2025/5/5
// zhangzhong
// 304. 二维区域和检索-矩阵不可变

// 给定一个二维矩阵 matrix，以下类型的多个请求：

// 计算其子矩形范围内元素的总和，该子矩阵的 左上角 为 (row1, col1) ，右下角 为
// (row2, col2) 。 实现 NumMatrix 类：

// NumMatrix(int[][] matrix) 给定整数矩阵 matrix 进行初始化
// int sumRegion(int row1, int col1, int row2, int col2) 返回 左上角 (row1,
// col1) 、右下角 (row2, col2) 所描述的子矩阵的元素 总和 。

// 示例 1：

// 输入:
// ["NumMatrix","sumRegion","sumRegion","sumRegion"]
// [[[[3,0,1,4,2],[5,6,3,2,1],[1,2,0,1,5],[4,1,0,1,7],[1,0,3,0,5]]],[2,1,4,3],[1,1,2,2],[1,2,2,4]]
// 输出:
// [null, 8, 11, 12]

// 解释:
// NumMatrix numMatrix = new
// NumMatrix([[3,0,1,4,2],[5,6,3,2,1],[1,2,0,1,5],[4,1,0,1,7],[1,0,3,0,5]]);
// numMatrix.sumRegion(2, 1, 4, 3); // return 8 (红色矩形框的元素总和)
// numMatrix.sumRegion(1, 1, 2, 2); // return 11 (绿色矩形框的元素总和)
// numMatrix.sumRegion(1, 2, 2, 4); // return 12 (蓝色矩形框的元素总和)

// 提示：

// m == matrix.length
// n == matrix[i].length
// 1 <= m, n <= 200
// -105 <= matrix[i][j] <= 105
// 0 <= row1 <= row2 < m
// 0 <= col1 <= col2 < n
// 最多调用 104 次 sumRegion 方法

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <vector>

// ok, 这道题目也非常简单
// 相较于一维的前缀和，我们这里的前缀和二维数组的每个位置都表示 [0, 0] - [row,
// col] 对应的矩阵的和即可
//

class NumMatrix {
public:
  // TIP: 底层只开一个数组，来模拟二维数组的话，更快
  NumMatrix(std::vector<std::vector<int>>& matrix)
      : prefix_matrix(matrix.size() + 1,
                      std::vector<int>(matrix[0].size() + 1, 0)) {
    // 对比一维的前缀和构造方式，仍然有两种方法
    // 第一种，使用一个变量一直累加和，这种方式是最简单的，只要按照从左到右，从上到下的方式遍历，即可
    int sum = 0;
    // 同样和一维的一样，数组的维度是size+1
    for (int row = 0; row < matrix.size(); row++) {
      for (int col = 0; col < matrix[0].size(); col++) {
        // 不对！这种累加方式是错误的
        // 只能使用第二种方式
        // sum += matrix[row][col];
        // 必须使用其上方，左方，左上方的和matrix在此位置的元素，得到前缀和
        //
        // 为了防止混淆，prefix matirx的row col和matirx的 row
        // col是错开一行一列的
        int prow = row + 1;
        int pcol = col + 1;
        prefix_matrix[prow][pcol] =
            prefix_matrix[prow][pcol - 1] + prefix_matrix[prow - 1][pcol] -
            prefix_matrix[prow - 1][pcol - 1] + matrix[row][col];
      }
    }
  }

  int sumRegion(int row1, int col1, int row2, int col2) {

    // 把right变成end
    // 就是把右闭区间改成右开区间
    row2++;
    col2++;

    return prefix_matrix[row2][col2] - prefix_matrix[row1][col2] -
           prefix_matrix[row2][col1] + prefix_matrix[row1][col1];
  }

private:
  std::vector<std::vector<int>> prefix_matrix;
};

/**
 * Your NumMatrix object will be instantiated and called as such:
 * NumMatrix* obj = new NumMatrix(matrix);
 * int param_1 = obj->sumRegion(row1,col1,row2,col2);
 */
