// 2025/4/22
// zhangzhong
// https://leetcode.cn/problems/reverse-string/description/

// 344. 反转字符串

// 编写一个函数，其作用是将输入的字符串反转过来。输入字符串以字符数组 s
// 的形式给出。

// 不要给另外的数组分配额外的空间，你必须原地修改输入数组、使用 O(1)
// 的额外空间解决这一问题。

// 示例 1：

// 输入：s = ["h","e","l","l","o"]
// 输出：["o","l","l","e","h"]
// 示例 2：

// 输入：s = ["H","a","n","n","a","h"]
// 输出：["h","a","n","n","a","H"]

// 提示：

// 1 <= s.length <= 105
// s[i] 都是 ASCII 码表中的可打印字符

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <vector>

class Solution {
public:
  void reverseString(std::vector<char>& s) {
    // 非常非常的简单，和判断回文串一样，就是双指针，从两侧向内遍历即可
    // 注意相会条件的处理
    int left = 0;
    int right = s.size() - 1;

    while (left < right) {
      std::swap(s[left], s[right]);
      left++;
      right--;
    }
  }
};

TEST_CASE("Example 1: s = ['h','e','l','l','o']") {
  Solution solution;
  std::vector<char> s = {'h', 'e', 'l', 'l', 'o'};
  std::vector<char> expected = {'o', 'l', 'l', 'e', 'h'};

  solution.reverseString(s);

  CHECK(s == expected);
}

TEST_CASE("Example 2: s = ['H','a','n','n','a','h']") {
  Solution solution;
  std::vector<char> s = {'H', 'a', 'n', 'n', 'a', 'h'};
  std::vector<char> expected = {'h', 'a', 'n', 'n', 'a', 'H'};

  solution.reverseString(s);

  CHECK(s == expected);
}

TEST_CASE("Single character string") {
  Solution solution;
  std::vector<char> s = {'a'};
  std::vector<char> expected = {'a'};

  solution.reverseString(s);

  CHECK(s == expected);
}

TEST_CASE("Palindrome string") {
  Solution solution;
  std::vector<char> s = {'r', 'a', 'c', 'e', 'c', 'a', 'r'};
  std::vector<char> expected = {'r', 'a', 'c', 'e', 'c', 'a', 'r'};

  solution.reverseString(s);

  CHECK(s == expected);
}

TEST_CASE("Even length string") {
  Solution solution;
  std::vector<char> s = {'a', 'b', 'c', 'd'};
  std::vector<char> expected = {'d', 'c', 'b', 'a'};

  solution.reverseString(s);

  CHECK(s == expected);
}

TEST_CASE("Odd length string") {
  Solution solution;
  std::vector<char> s = {'a', 'b', 'c'};
  std::vector<char> expected = {'c', 'b', 'a'};

  solution.reverseString(s);

  CHECK(s == expected);
}
