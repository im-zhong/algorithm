// 2025/4/23
// zhangzhong
// https://leetcode.cn/problems/longest-substring-without-repeating-characters/

// 给定一个字符串 s ，请你找出其中不含有重复字符的 最长 子串 的长度。

// 示例 1:

// 输入: s = "abcabcbb"
// 输出: 3
// 解释: 因为无重复字符的最长子串是 "abc"，所以其长度为 3。
// 示例 2:

// 输入: s = "bbbbb"
// 输出: 1
// 解释: 因为无重复字符的最长子串是 "b"，所以其长度为 1。
// 示例 3:

// 输入: s = "pwwkew"
// 输出: 3
// 解释: 因为无重复字符的最长子串是 "wke"，所以其长度为 3。
//      请注意，你的答案必须是 子串 的长度，"pwke" 是一个子序列，不是子串。

// 提示：

// 0 <= s.length <= 5 * 104
// s 由英文字母、数字、符号和空格组成

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <algorithm>
#include <doctest/doctest.h>
#include <string>
#include <vector>

// 这道题目使用滑动窗口也是可解的
// 在向前滑动的过程中，记录窗口中出现的字符
// 只要没有出现重复，就一直向前，并且一直更新最长的字符串的解
// 如果出现了重复，就收缩窗口，知道不在重复，持续下去
//
// 收缩窗口时可以进行优化，

class Solution {
public:
  // 可以写构造函数吗？
  Solution() {
    // last记录的是某个字符串s
    // 在i的位置上出现的字符，在前面出现的最靠后的位置
    // 也就是前一个相同字符的位置，如果没有就是-1
    // 这个last数组的初始化
    last.resize(50001);
  }

  int lengthOfLongestSubstring(std::string s) {
    // hashmap可以用一个ascii的数组来实现
    // 128的就够了

    // 首先重置hashmap的状态
    // 我们用-1表示字母不存在

    // chars 是一个哈希表，key: char, value: int
    // 表示字符串中某个char出现的最后的位置
    for (int i = 0; i < 256; i++) {
      chars[i] = -1;
    }

    // 只需遍历一遍字符串，就可以构造出 last[i]
    // 然后遍历字符串
    for (int i = 0; i < s.size(); i++) {
      // 哎！默认值就是-1的话 这里好像可以直接进行一个赋值啊 代码更漂亮了
      // if (characters[s[i]] == -1) {
      //     // 当前i指向的char在前面没有重复
      //     last[i] = -1;
      // } else {
      //     // 否则就是出现过
      //     last[i] = characters[s[i]];
      // }
      // 那么这个初始化就非常好理解了，就是让字符串s中当前位置i的字符的last指向s中该字符出现的最后位置
      // 实际上是倒数第二个位置，因为接下来下一条语句马上就更新了chars中对应字符si的最后位置
      last[i] = chars[s[i]];
      // 然后更新hashmap，让其中的元素始终指向字符出现的最后位置
      chars[s[i]] = i;
    }

    // 然后就是遍历last数组
    // 这里使用了左闭右闭区间
    int left = 0;
    int right = 0;
    int max_length = 0;
    while (right < s.size()) {

      // 关键是，如何使用这个last数组来判断是否在区间[left,
      // right]是否有重复元素呢
      // 非常简单，每当right++的时候，我们都会进来一个新的元素
      // 我们只需要查看last数组，找到他的前一个重复元素 last[right]
      // 是否在区间[left, right] 中 也就是 last[right] < left,
      // 就不在区间里面，可以继续扩大滑动窗口

      // 首先不断的向右扩展right，直到碰到重复元素出现在[left, right]之间
      while (right < s.size() && last[right] < left) {
        right++;
      }

      // 此时 last[right] >= left
      // 说明在区间[left, ... last[right], ... right]
      // 中有重复的元素
      // 那么[left, right-1] 这个区间里面是没有重复的！
      // 此时，right刚好可以视作左闭右开区间的end
      // 所以计算区间长度就是 right(end) - left(begin)

      // last[right] >= left
      // 即在[left, right]之间出现了重复元素
      // 此时计算当前的序列长度
      max_length = std::max(max_length, right - left);

      // 然后让left跳过出现重复元素的序列，也就是跳到 last[right]
      // 指向的位置的下一个位置 因为此时 sequence[right] ==
      // sequence[last[right]]
      // [left, ... last[right], ... right]， 其中 last[right] 和
      // right的位置的元素重复 所以left直接跳过
      left = last[right] + 1;
    }

    return max_length;
  }

  // 不知道我可不可以在这里写成员变量呢？
  std::vector<int> last;
  int chars[256];
};

TEST_CASE("Example 1: s = 'abcabcbb'") {
  Solution solution;
  std::string s = "abcabcbb";
  int result = solution.lengthOfLongestSubstring(s);

  CHECK(result == 3);
}

TEST_CASE("Example 2: s = 'bbbbb'") {
  Solution solution;
  std::string s = "bbbbb";
  int result = solution.lengthOfLongestSubstring(s);

  CHECK(result == 1);
}

TEST_CASE("Example 3: s = 'pwwkew'") {
  Solution solution;
  std::string s = "pwwkew";
  int result = solution.lengthOfLongestSubstring(s);

  CHECK(result == 3);
}

TEST_CASE("Empty string") {
  Solution solution;
  std::string s = "";
  int result = solution.lengthOfLongestSubstring(s);

  CHECK(result == 0);
}

TEST_CASE("Single character") {
  Solution solution;
  std::string s = "a";
  int result = solution.lengthOfLongestSubstring(s);

  CHECK(result == 1);
}

TEST_CASE("All distinct characters") {
  Solution solution;
  std::string s = "abcdefg";
  int result = solution.lengthOfLongestSubstring(s);

  CHECK(result == 7);
}

TEST_CASE("Repeated characters spaced out") {
  Solution solution;
  std::string s = "abcdefgabcdefg";
  int result = solution.lengthOfLongestSubstring(s);

  CHECK(result == 7);
}

// TEST_CASE("String with spaces and special characters") {
//   Solution solution;
//   std::string s = "ab c!def";
//   int result = solution.lengthOfLongestSubstring(s);

//   CHECK(result == 8);
// }
