// 2025/5/8
// zhangzhong
// trie tree
// https://labuladong.online/algo/data-structure-basic/trie-map-basic/
// https://labuladong.online/algo/data-structure/trie-implement/
// 我记得算法4 那本书里也有这种数据结构的介绍

// Trie树就是 多叉树结构 的延伸，是一种针对字符串进行特殊优化的数据结构。
// Trie树在处理字符串相关操作时有诸多优势，比如节省公共字符串前缀的内存空间、方便处理前缀操作、支持通配符匹配等。

// 前缀，keys with prefix 适合用来实现自动补全功能

// Trie树的主要应用场景：
// 1. 节约存储空间
// 2. 方便处理前缀操作
// 3. 可以使用通配符
// 4. 可以按照字典序遍历键

// TODO:
// 我希望这次做trie的题目就可以应用上我的模板，所以我需要先去继续学习STL，然后回来把trie
// tree一次性写好就行了，很多东西都应该一次性写好，然后就不用再写了，然后写好注释，最好是动画做的注释
// 这样以后再来复习的时候，也不用再重新学了。
// 这个东西需要的时间很长，感觉一时半会不应该做，还是先多刷基础的题目再说吧。

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <array>
#include <doctest/doctest.h>
#include <string>
#include <unordered_map>
#include <vector>

using value_type = int;

struct TrieTreeNode {
  value_type value;
  // ASCII, 'a' = 97
  std::array<TrieTreeNode*, 256> children;
  // or if only use small letters, you could only use 26, and use offset, xx -
  // 'a' as the index
  // or if the letter is not consecutive, you coude just use
  // the hashmap: char -> int
  // it depends.

  // api

  // 在 Map 中添加 key
  void put(const std::string& key, value_type value) {}

  // 删除键 key 以及对应的值
  void remove(std::string key);

  // 搜索 key 对应的值，不存在则返回 null
  // get("the") -> 4
  // get("tha") -> null
  value_type get(std::string key);

  // 判断 key 是否存在在 Map 中
  // containsKey("tea") -> false
  // containsKey("team") -> true
  bool containsKey(std::string key);

  // 在 Map 的所有键中搜索 query 的最短前缀
  // shortestPrefixOf("themxyz") -> "the"
  std::string shortestPrefixOf(std::string query);

  // 在 Map 的所有键中搜索 query 的最长前缀
  // longestPrefixOf("themxyz") -> "them"
  std::string longestPrefixOf(std::string query);

  // 搜索所有前缀为 prefix 的键
  // keysWithPrefix("th") -> ["that", "the", "them"]
  std::vector<std::string> keysWithPrefix(std::string prefix);

  // 判断是和否存在前缀为 prefix 的键
  // hasKeyWithPrefix("tha") -> true
  // hasKeyWithPrefix("apple") -> false
  bool hasKeyWithPrefix(std::string prefix);

  // 通配符 . 匹配任意字符，搜索所有匹配的键
  // keysWithPattern("t.a.") -> ["team", "that"]
  std::vector<std::string> keysWithPattern(std::string pattern);

  // 通配符 . 匹配任意字符，判断是否存在匹配的键
  // hasKeyWithPattern(".ip") -> true
  // hasKeyWithPattern(".i") -> false
  bool hasKeyWithPattern(std::string pattern);

  // 返回 Map 中键值对的数量
  int size();
};

// 卧槽！我突然意识到我学习STL的底层实现是有意义的，我要创造一个刷题的STL库！很多底层的数据结构，用模板写好了之后，直接复制过去使用就行了呀！
// 不只是面试的时候用得到，我们以后刷codeforce也可能用到的！nice！一箭双雕
