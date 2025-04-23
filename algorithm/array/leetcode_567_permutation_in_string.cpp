// 2025/4/23
// zhangzhong
// https://leetcode.cn/problems/permutation-in-string/description/

// 567. 字符串的排列
// 给你两个字符串 s1 和 s2 ，写一个函数来判断 s2 是否包含 s1 的
// 排列。如果是，返回 true ；否则，返回 false 。

// 换句话说，s1 的排列之一是 s2 的 子串 。

// 示例 1：

// 输入：s1 = "ab" s2 = "eidbaooo"
// 输出：true
// 解释：s2 包含 s1 的排列之一 ("ba").
// 示例 2：

// 输入：s1= "ab" s2 = "eidboaoo"
// 输出：false

// 提示：

// 1 <= s1.length, s2.length <= 104
// s1 和 s2 仅包含小写字母

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <unordered_map>
#include <vector>

class Solution {
public:
  void print_set(const std::unordered_map<char, int>& set) {
    std::cout << "{";
    for (const auto [ch, count] : set) {
      std::cout << ch << ": " << count << ", ";
    }
    std::cout << "}\n";
  }

  // 这道题目用模板也可以做，但是不直观
  // 这道题目就是用一个给定长度的窗口扫描整个序列而已
  // 模板还是更适用于滑动窗口会不规则的变化的
  bool checkInclusion(std::string s1, std::string s2) {
    // 这个题的做法应该和leetcode 76 是差不多的
    // 不对，这个题目比76简单多了
    // s2需要包含s1的排列，也就是s2的某个子串（窗口）和s1的的字符的集合是一样的
    // 而且这个窗口的滑动规则要简单的多，
    // end先向前滑动 s1.size() 个长度，然后begin和end一起移动就行了

    // invariant
    if (s2.size() < s1.size()) {
      return false;
    }

    // 排列，排列其实就是集合
    // 因为s1可能有重复额字符，所以需要map
    std::unordered_map<char, int> s1_set;
    for (char ch : s1) {
      s1_set[ch]++;
    }

    std::unordered_map<char, int> window;

    int begin = 0;
    int end = 0;

    // 先把s2的前s1.size()个元素放到window中
    for (end = 1; end <= s1.size(); end++) {
      char ch = s2[end - 1];
      window[ch]++;
    }
    // print_set(window);

    if (s1_set == window) {
      return true;
    }

    // 然后判断 window 和 s1_set需要完全一样才行

    // 不对，在这个循环一开始的时候，我们必须让[begin, end) 指向上一个区间
    // 我们需要操作这个区间的首位，然后跳到下一个区间

    // 不对，每个循环开始的时候，指向的是上一个完整的区间，而且已经判断过了
    // 这里必须是 < , 因为我们会在函数内部让end++, 并且访问！
    // 卧槽！BUG，这里必须让end--
    // 因为上面end多加了一次，到了end = s1.size() + 1
    // 这里希望 end = s1.size()
    // end--;
    end = s1.size(); // 这个也对
    while (end < s2.size()) {

      // 我们先把区间

      char begin_ch = s2[begin];

      window[begin_ch]--;
      if (window[begin_ch] == 0) {
        window.erase(begin_ch);
      }
      // 不对！我们只需要把首元素给删除就行了
      // window[end_ch]++;

      // 然后跳转到下一个区间
      begin++;
      end++;

      // 然后将end元素插入到window中
      char end_ch = s2[end - 1];
      window[end_ch]++;

      // 然后判断
      if (s1_set == window) {
        return true;
      }
      // print_set(window);
    }

    return false;
  }
};

TEST_CASE("Example 1: s1 = 'ab', s2 = 'eidbaooo'") {
  Solution solution;
  std::string s1 = "ab";
  std::string s2 = "eidbaooo";
  bool result = solution.checkInclusion(s1, s2);

  CHECK(result == true);
}

TEST_CASE("Example 2: s1 = 'ab', s2 = 'eidboaoo'") {
  Solution solution;
  std::string s1 = "ab";
  std::string s2 = "eidboaoo";
  bool result = solution.checkInclusion(s1, s2);

  CHECK(result == false);
}

TEST_CASE("s1 longer than s2") {
  Solution solution;
  std::string s1 = "hello";
  std::string s2 = "eid";
  bool result = solution.checkInclusion(s1, s2);

  CHECK(result == false);
}

TEST_CASE("s1 and s2 are identical") {
  Solution solution;
  std::string s1 = "abc";
  std::string s2 = "abc";
  bool result = solution.checkInclusion(s1, s2);

  CHECK(result == true);
}

TEST_CASE("s1 is a single character") {
  Solution solution;
  std::string s1 = "a";
  std::string s2 = "abc";
  bool result = solution.checkInclusion(s1, s2);

  CHECK(result == true);
}

TEST_CASE("s1 has repeated characters") {
  Solution solution;
  std::string s1 = "aab";
  std::string s2 = "eidbaaooo";
  bool result = solution.checkInclusion(s1, s2);

  CHECK(result == true);
}

TEST_CASE("s2 has the characters but not in right frequency") {
  Solution solution;
  std::string s1 = "aab";
  std::string s2 = "eidbaooo";
  bool result = solution.checkInclusion(s1, s2);

  CHECK(result == false);
}
