// 2025/4/22
// zhangzhong
// https://leetcode.cn/problems/longest-palindromic-substring/description/

// 5. 最长回文子串

// 给你一个字符串 s，找到 s 中最长的 回文 子串。

// 示例 1：

// 输入：s = "babad"
// 输出："bab"
// 解释："aba" 同样是符合题意的答案。
// 示例 2：

// 输入：s = "cbbd"
// 输出："bb"

// 提示：

// 1 <= s.length <= 1000
// s 仅由数字和英文字母组成

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <algorithm>
#include <doctest/doctest.h>
#include <string>
#include <vector>

struct Span {
  int begin;
  int length;
};

class Solution {
public:
  // 这个函数可以同时判断奇数和偶数的回文串！
  Span get_palindrome_length(const std::string& s, int l, int r) {
    // l 和 r 从
    while (l >= 0 && r < s.size() && s[l] == s[r]) {
      l--;
      r++;
    }
    // 如果在一开始，l就不等于r呢？
    // 不对，l和r会越过回文串，出去一格 所以需要往回拉一下
    return Span{l + 1, r - l - 1};
  }

  std::string longestPalindrome(std::string s) {

    // 我想到一个方法
    // 从任意一个点开始，向两侧寻找回文串
    // 然后返回最长的那个
    // 不过这个是遍历的方法
    // 果然是这样！
    // 这样双指针的技巧就齐了
    // 一起向前的，或快或慢，或同步，或倍速
    // 从两侧向中间的，从中间向两侧的！

    // 回文串在处理的时候有一个点需要注意！
    // 那就是回文串存在奇数长度和偶数长度
    // 这个需要考虑到

    // 简单的讲，就是遍历！
    int len = 0;
    // ! BUG！ 下面的处理方法，在数组长度只有1的时候，是无法处理的
    // 循环会被直接跳过
    // 所以我们的初始值，应该就是答案，也就是当只有一格字符的时候，回文串就是他自己
    Span strspan{0, 1};
    // 注意到，这里使用的是 size() -1
    // 因为最后一个字符，一定不存在偶数个的回文串
    // 然后最后一个字符 奇数的回文串的长度一定是 1 他一定不是最长的那个
    // 所以跳过了 因为不跳过的话，get length会越界！
    for (int i = 0; i < s.size() - 1; i++) {

      // 存在以 i 为中心的 奇数长度的 回文
      // 存在以 i, i+1 为中心的 偶数长度的回文串
      // 我们写一个函数来返回回文串的长度即可
      // 保存回文串长度的最大值

      // 哦，不对，这里需要返回的是字符串
      // 那，我们返回一个span吧
      Span odd{get_palindrome_length(s, i, i)};
      if (odd.length > len) {
        len = odd.length;
        strspan = odd;
      }
      Span even{get_palindrome_length(s, i, i + 1)};
      if (even.length > len) {
        len = even.length;
        strspan = even;
      }
    }

    return s.substr(strspan.begin, strspan.length);
  }
};

TEST_CASE("Example 1: s = 'babad'") {
  Solution solution;
  std::string s = "babad";
  std::string result = solution.longestPalindrome(s);

  // Both "bab" and "aba" are valid answers
  CHECK((result == "bab" || result == "aba"));
  CHECK(result.length() == 3);
}

TEST_CASE("Example 2: s = 'cbbd'") {
  Solution solution;
  std::string s = "cbbd";
  std::string result = solution.longestPalindrome(s);

  CHECK(result == "bb");
  CHECK(result.length() == 2);
}

TEST_CASE("Single character") {
  Solution solution;
  std::string s = "a";
  std::string result = solution.longestPalindrome(s);

  CHECK(result == "a");
  CHECK(result.length() == 1);
}

TEST_CASE("All same characters") {
  Solution solution;
  std::string s = "aaaa";
  std::string result = solution.longestPalindrome(s);

  CHECK(result == "aaaa");
  CHECK(result.length() == 4);
}

TEST_CASE("Palindrome at beginning") {
  Solution solution;
  std::string s = "abbcd";
  std::string result = solution.longestPalindrome(s);

  CHECK(result == "bb");
  CHECK(result.length() == 2);
}

TEST_CASE("Palindrome at end") {
  Solution solution;
  std::string s = "cdabb";
  std::string result = solution.longestPalindrome(s);

  CHECK(result == "bb");
  CHECK(result.length() == 2);
}

TEST_CASE("Longer palindrome") {
  Solution solution;
  std::string s = "cababac";
  std::string result = solution.longestPalindrome(s);

  CHECK(result == "cababac");
  CHECK(result.length() == 7);
}
