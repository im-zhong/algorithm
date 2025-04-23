// 2025/4/23
// zhangzhong
// https://leetcode.cn/problems/find-all-anagrams-in-a-string/description/

// 438. 找到字符串中所有字母异位词

// 给定两个字符串 s 和 p，找到 s 中所有 p 的 异位词
// 的子串，返回这些子串的起始索引。不考虑答案输出的顺序。

// 示例 1:

// 输入: s = "cbaebabacd", p = "abc"
// 输出: [0,6]
// 解释:
// 起始索引等于 0 的子串是 "cba", 它是 "abc" 的异位词。
// 起始索引等于 6 的子串是 "bac", 它是 "abc" 的异位词。
//  示例 2:

// 输入: s = "abab", p = "ab"
// 输出: [0,1,2]
// 解释:
// 起始索引等于 0 的子串是 "ab", 它是 "ab" 的异位词。
// 起始索引等于 1 的子串是 "ba", 它是 "ab" 的异位词。
// 起始索引等于 2 的子串是 "ab", 它是 "ab" 的异位词。

// 提示:

// 1 <= s.length, p.length <= 3 * 104
// s 和 p 仅包含小写字母

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

  // 字母异位词是通过重新排列不同单词或短语的字母而形成的单词或短语，并使用所有原字母一次。
  // 那tm这道题不就和leetcode 567 一样，只不过这里需要找到所有的符合条件的子串
  // 而567只需要找到一处就直接返回

  std::vector<int> findAnagrams(std::string s, std::string p) {

    // invariant
    if (s.size() < p.size()) {
      return {};
    }

    // std::vector<int> 里面就存放begin的位置就行
    std::vector<int> results;

    std::unordered_map<char, int> pset;
    for (char ch : p) {
      pset[ch]++;
    }

    std::unordered_map<char, int> window;
    int begin = 0;
    int end = 0;
    for (int end = 0; end < p.size(); end++) {
      char ch = s[end];
      window[ch]++;
    }

    if (window == pset) {
      results.push_back(begin);
    }
    // print_set(window);

    // 草，还是这里直接写上最安全了
    end = p.size();
    // 最终end = p.size()
    while (end < s.size()) {

      // 先删除旧的区间的第一个元素
      char begin_ch = s[begin];
      window[begin_ch]--;
      if (window[begin_ch] == 0) {
        window.erase(begin_ch);
      }

      begin++;
      end++;

      // 添加后一个元素
      char end_ch = s[end - 1];
      window[end_ch]++;

      if (window == pset) {
        results.push_back(begin);
      }
      // print_set(window);
    }

    return results;
  }
};

TEST_CASE("Example 1: s = 'cbaebabacd', p = 'abc'") {
  Solution solution;
  std::string s = "cbaebabacd";
  std::string p = "abc";
  std::vector<int> result = solution.findAnagrams(s, p);

  // Sort the result to ensure consistent comparison
  std::sort(result.begin(), result.end());
  std::vector<int> expected = {0, 6};

  CHECK(result == expected);
}

TEST_CASE("Example 2: s = 'abab', p = 'ab'") {
  Solution solution;
  std::string s = "abab";
  std::string p = "ab";
  std::vector<int> result = solution.findAnagrams(s, p);

  // Sort the result to ensure consistent comparison
  std::sort(result.begin(), result.end());
  std::vector<int> expected = {0, 1, 2};

  CHECK(result == expected);
}

TEST_CASE("No anagrams found") {
  Solution solution;
  std::string s = "hello";
  std::string p = "xyz";
  std::vector<int> result = solution.findAnagrams(s, p);

  CHECK(result.empty());
}

TEST_CASE("p is longer than s") {
  Solution solution;
  std::string s = "abc";
  std::string p = "abcdef";
  std::vector<int> result = solution.findAnagrams(s, p);

  CHECK(result.empty());
}

TEST_CASE("Single character pattern") {
  Solution solution;
  std::string s = "aaaa";
  std::string p = "a";
  std::vector<int> result = solution.findAnagrams(s, p);

  std::vector<int> expected = {0, 1, 2, 3};

  CHECK(result == expected);
}

TEST_CASE("Pattern with repeated characters") {
  Solution solution;
  std::string s = "aababba";
  std::string p = "aab";
  std::vector<int> result = solution.findAnagrams(s, p);

  // Sort the result to ensure consistent comparison
  std::sort(result.begin(), result.end());
  std::vector<int> expected = {0, 1};

  CHECK(result == expected);
}
