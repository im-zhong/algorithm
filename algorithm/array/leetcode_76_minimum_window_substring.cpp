// 2025/4/23
// zhangzhong
// https://leetcode.cn/problems/minimum-window-substring/description/

// 76. 最小覆盖子串

// 给你一个字符串 s 、一个字符串 t 。返回 s 中涵盖 t 所有字符的最小子串。如果 s
// 中不存在涵盖 t 所有字符的子串，则返回空字符串 "" 。

// 注意：

// 对于 t 中重复字符，我们寻找的子字符串中该字符数量必须不少于 t 中该字符数量。
// 如果 s 中存在这样的子串，我们保证它是唯一的答案。

// 示例 1：

// 输入：s = "ADOBECODEBANC", t = "ABC"
// 输出："BANC"
// 解释：最小覆盖子串 "BANC" 包含来自字符串 t 的 'A'、'B' 和 'C'。
// 示例 2：

// 输入：s = "a", t = "a"
// 输出："a"
// 解释：整个字符串 s 是最小覆盖子串。
// 示例 3:

// 输入: s = "a", t = "aa"
// 输出: ""
// 解释: t 中两个字符 'a' 均应包含在 s 的子串中，
// 因此没有符合条件的子字符串，返回空字符串。

// 提示：

// m == s.length
// n == t.length
// 1 <= m, n <= 105
// s 和 t 由英文字母组成

// 进阶：你能设计一个在 o(m+n) 时间内解决此问题的算法吗？

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <unordered_map>
#include <vector>

class Solution {
public:
  // 果然我想的是没错的

  // 滑动窗口算法的思路是这样：

  // 1、我们在字符串 S 中使用双指针中的左右指针技巧，初始化 left = right =
  // 0，把索引左闭右开区间 [left, right) 称为一个「窗口」。

  // 为什么要「左闭右开」区间

  // 理论上你可以设计两端都开或者两端都闭的区间，但设计为左闭右开区间是最方便处理的。

  // 因为这样初始化 left = right = 0 时区间 [0, 0) 中没有元素，但只要让 right
  // 向右移动（扩大）一位，区间 [0, 1) 就包含一个元素 0 了。

  // 如果你设置为两端都开的区间，那么让 right 向右移动一位后开区间 (0, 1)
  // 仍然没有元素；如果你设置为两端都闭的区间，那么初始区间 [0, 0]
  // 就包含了一个元素。这两种情况都会给边界处理带来不必要的麻烦。

  // 2、我们先不断地增加 right 指针扩大窗口 [left,
  // right)，直到窗口中的字符串符合要求（包含了 T 中的所有字符）。

  // 3、此时，我们停止增加 right，转而不断增加 left 指针缩小窗口 [left,
  // right)，直到窗口中的字符串不再符合要求（不包含 T
  // 中的所有字符了）。同时，每次增加 left，我们都要更新一轮结果。

  // 4、重复第 2 和第 3 步，直到 right 到达字符串 S 的尽头。

  // 这个思路其实也不难，第 2 步相当于在寻找一个「可行解」，然后第 3
  // 步在优化这个「可行解」，最终找到最优解，也就是最短的覆盖子串。左右指针轮流前进，窗口大小增增减减，就好像一条毛毛虫，一伸一缩，不断向右滑动，这就是「滑动窗口」这个名字的来历。

  bool isInclude(const std::unordered_map<char, int>& need,
                 std::unordered_map<char, int>& window) {
    for (const auto [ch, count] : need) {
      // operator[] 会修改window容器，所以把constant去掉了
      if (window[ch] < count) {
        return false;
      }
    }
    return true;
  }

  std::string minWindowSlow(std::string s, std::string t) {
    // 滑动窗口问题，如果是从模板的角度来思考的话，只需要考虑两个点
    // 一个是在何时缩小窗口，扩大窗口不需要思考，因为每次的循环一次，就会扩大窗口
    // 另外一个就是窗口里面的元素要如何表示，在扩大和缩小窗口的时候要如何调整这个表示

    // 这道题，窗口里面的元素是没有顺序的，所以本质上是字符的集合
    // 那么我们在扩大和缩小窗口的时候，需要的操作就显而易见了，就是，扩大的时候，向集合中增加元素
    // 减少的时候，从集合中删除元素

    // 关键是在何时缩小窗口？比如在我们的窗口中的元素包含t的所有字符的时候，
    // 开始尝试缩小窗口，缩小到不能再缩的时候，就找到了right位置的一个解
    // 然后继续扩大窗口，找下一个解
    // 最终返回最短的解

    // 滑动窗口也采用左必右开的方式，binary_search, lower_bound,
    // upper_bound也都采用这种方式 快排，双指针向内的，使用左必右必的比较方便
    // 为什么？本质上是他们表示的东西不同，滑动窗口，和二分查找，本质上左右指针表示的是一个区间，表示区间最方便的方式就是左开右必，可以省掉一些额外的边界判断，
    // 但是双指针向内，向外，他们表示的就是指针指向的元素！这个时候必须是必的，因为区间的end，不一定指向元素
    // 或许从这种思考方式去理解，也更容易判断应该使用那种做法

    // 首先需要将t转换成哈希表
    // t -> need
    // window

    std::unordered_map<char, int> need;
    for (char ch : t) {
      need[ch]++;
    }

    // 我们可以通过固定的变量名来表示这两种区间
    // [begin, end)
    // [left, right]
    int begin = 0;
    int end = 0;

    std::unordered_map<char, int> window;

    // 我们让span最开始，等于一个题目中不可能出现的最长的字符串
    // 然后在后面判断是否这个东西没有被修改，那么就说明我们没有找到解
    // 就返回空串
    // TIP：这里不对，我总是习惯的去记录区间
    // 我应该像标准库里面那样，记录begin和length
    std::pair<int, int> span{0, 0};

    for (int end = 1; end <= s.size(); end++) {

      // 将end前面的元素加入window中
      window[s[end - 1]]++;

      // 接下来就需要判断，是否需要缩小窗口
      // 就是当window包含need的所有元素的时候
      // 怎么判断呢？
      // 我们应该遍历need
      // 然后看看window里面对应得字符，的count是不是比need的大？
      // 这个遍历应该是快的
      // 我们把这个写成一个函数吧
      // 这里应该是一个循环！
      bool foundSolution = false;
      // TIP：我的实现慢的原因就是这里的isInclude每次都需要遍历need，非常的慢
      // 因为我们每次都只会修改一个元素，所以直接查看这个操作对结果有效性的影响，而不是每次都
      // 重新遍历，应该会快很多
      while (isInclude(need, window)) {
        foundSolution = true;
        // 把窗口缩小
        // 也就是把begin指向的元素从window中去掉
        // 这个操作一定是安全de
        window[s[begin]]--;
        begin++;
      }
      // 如果进入了这个循环，然后跳出了这个循环
      // 说明我们找到了一个解
      // 但是我们也有可能直接跳过了这个循环
      // 所以程序到达这里的时候，无法判断，需要一个额外的flag
      if (foundSolution) {
        // 记录一下区间，用span来记录
        // 算了，用pair吧
        // 因为我们要找最小的
        // 因为begin跳过了当前的解的窗口，才能退出上面的循环
        std::pair<int, int> new_sol{begin - 1, end};

        // 或者，可以在这里判断 是否是 00
        if (((span.first == 0) && (span.second == 0)) ||
            (new_sol.second - new_sol.first) < (span.second - span.first)) {
          span = new_sol;
        }
      }
    }

    return s.substr(span.first, span.second - span.first);
  }

  // 暴力解法
  //   for (int i = 0; i < s.length(); i++)
  //     for (int j = i + 1; j < s.length(); j++)
  //         if s[i:j] 包含 t 的所有字母:
  //             更新答案
  // 其中： if s[i:j] 包含 t 的所有字母: 这个操作要如何做？
  // 我能想到的一个思路是使用hashmap，如果字符有范围的话，也可以使用数组
  // key: char, value: int(字符的重复次数)
  // 然后我们遍历s，每出现一个字符，就减去hashmp中的字符重复次数
  // 最终，字符全部被删去，就说明s[i:j] 包含 t 的所有字母
  //
  // 当然，如果我们直接维护的就是这个hashmap，那么应该直接比较两者是否相等
  // 这样每次的比较是一个常数哎，因为ascii的个数是常数

  std::string minWindow(std::string s, std::string t) {
    std::unordered_map<char, int> need;
    for (char ch : t) {
      need[ch]++;
    }

    int begin = 0;
    int end = 0;
    std::unordered_map<char, int> window;
    std::pair<int, int> span{0, INT_MAX};

    int valid_count = 0;
    for (int end = 1; end <= s.size(); end++) {

      char ch = s[end - 1];
      // 其实也可以不放
      window[ch]++;

      // 在这里统计某个字符是否达标
      // C++ 版的contains
      if (need.count(ch) > 0) {

        // 不对，如果是>= 的话，会重复统计
        // 只需要在= 的那一刻统计一下就行了
        if (window[ch] == need[ch]) {
          valid_count++;
        }
      }

      // 那这里就是判断是否需要缩小窗口的时候了
      // 条件就是valid_count == need.size()
      // 就是全部的都达标了
      while (valid_count == need.size()) {

        // ！在这里更新答案
        // 这是对的，因为在这里的时候，区间肯定是有效的
        // 而且越更新越短！
        if (end - begin < span.second) {
          // update spa
          span.first = begin;
          span.second = end - begin;
        }

        // 将begin从window中踢出去
        // 并且需要在何时的时机减少valid count
        char ch = s[begin];

        // 在减之前就要判断，
        if (need.count(ch) > 0) {
          if (window[ch] == need[ch]) {
            valid_count--;
          }
        }
        window[ch]--;
        // 减小窗口
        begin++;
      }
    }

    if (span.second == INT_MAX) {
      return "";
    } else {
      return s.substr(span.first, span.second);
    }
  }
};

TEST_CASE("Example 1: s = 'ADOBECODEBANC', t = 'ABC'") {
  Solution solution;
  std::string s = "ADOBECODEBANC";
  std::string t = "ABC";
  std::string result = solution.minWindow(s, t);

  CHECK(result == "BANC");
}

TEST_CASE("Example 2: s = 'a', t = 'a'") {
  Solution solution;
  std::string s = "a";
  std::string t = "a";
  std::string result = solution.minWindow(s, t);

  CHECK(result == "a");
}

TEST_CASE("Example 3: s = 'a', t = 'aa'") {
  Solution solution;
  std::string s = "a";
  std::string t = "aa";
  std::string result = solution.minWindow(s, t);

  CHECK(result == "");
}

TEST_CASE("Window at beginning of string") {
  Solution solution;
  std::string s = "ABCDEFG";
  std::string t = "ABC";
  std::string result = solution.minWindow(s, t);

  CHECK(result == "ABC");
}

TEST_CASE("Window at end of string") {
  Solution solution;
  std::string s = "DEFGABC";
  std::string t = "ABC";
  std::string result = solution.minWindow(s, t);

  CHECK(result == "ABC");
}

TEST_CASE("Window with repeated characters") {
  Solution solution;
  std::string s = "ADOBECODEBANC";
  std::string t = "AABC";
  std::string result = solution.minWindow(s, t);

  CHECK(result == "ADOBECODEBA");
}

TEST_CASE("Entire string is the window") {
  Solution solution;
  std::string s = "ABCDEF";
  std::string t = "ABCDEF";
  std::string result = solution.minWindow(s, t);

  CHECK(result == "ABCDEF");
}
