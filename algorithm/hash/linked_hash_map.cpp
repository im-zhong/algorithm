// 2025/4/29
// zhangzhong
// hashed linked list
// https://labuladong.online/algo/data-structure-basic/hashtable-with-linked-list/
// https://labuladong.online/algo/data-structure/lru-cache/#%E4%B8%89%E3%80%81%E4%BB%A3%E7%A0%81%E5%AE%9E%E7%8E%B0

// 我觉得我应该深入了
// 我要深入理解STL的实现细节！精通模板！
// 咱们这样来吧，在刷题这块，不用模板来实现，只关注于算法和数据结构方面的要点
// 但是在mtd里面，实现的东西都是基于模板的，这样两方面不冲突，我认为是比较合适的学习方式！

// 简单来说，就是在一个hash map里面，key不变，value变成list的节点即可

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <algorithm>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>

using key_type = std::string;
using value_type = int;

struct kv_pair {
  key_type key;
  value_type value;
};

class LinkedHashMap {

public:
  // api is what?
  // 比较合适的方式是拿list的迭代器
  // 因为std::list 可以直接用迭代器作为参数来操作list

  using list_type = std::list<kv_pair>;

  // 看一下有哪些API

  value_type get(key_type key) {
    // 不是，他为什么要在没有这个key的时候返回value的默认值啊？
    return this->_map.at(key)->value;
  }

  void put(key_type key, value_type value) {

    // BUG： 草，忘了考虑是否是已经存在的key了，！！！
    if (contains(key)) {
      // replace it with new value
      // 我们保存的是mutable iterator，所以可以通过iterator修改其内容
      _map[key]->value = value;
      return;
    }

    // we should insert at the end the get the inserted iterator
    // Inserts value before pos, possibly using move semantics.
    // return  Iterator pointing to the first element inserted
    auto iter = _list.insert(_list.end(), kv_pair{key, value});
    // 最关键的一点，删除，增加链表中的元素，其他已经存在的元素不受影响！
    // file:///Users/zhangzhong/Downloads/html-book-20250209/reference/en/cpp/container/list/erase.html：
    // References and iterators to the erased elements are invalidated. Other
    //  references and iterators are not affected.
    // insert: No iterators or references are invalidated.
    // 所以，我合理怀疑，list的迭代器应该就是原生指针，vecotr的迭代器就是原生指针。

    _map.insert({key, iter});
  }

  void remove(key_type key) {
    if (!contains(key)) {
      return;
    }

    // first get the iter
    auto iter = _map[key];
    // erase this iter: Removes the element at pos.
    _list.erase(iter);
    _map.erase(key);
  }

  bool contains(key_type key) { return _map.count(key) > 0; }

  std::vector<key_type> keys() {
    // 这个应该就是按照插入顺序返回key
    // how? 哎！我们是不是要在链表的节点中同时保存key和value才行？
    // 果然！只有这样我们才能在遍历链表，按照插入序返回所有keys

    // traditional version:
    // std::vector<key_type> _keys;
    // for (const kv_pair& kv : _list) {
    //   _keys.push_back(kv.key);
    // }
    // return _keys;

    // better version:
    std::vector<key_type> _keys;
    // pre allocate for better performance
    _keys.reserve(_map.size());

    // use transform and back_insert
    std::transform(_list.begin(), _list.end(), std::back_inserter(_keys),
                   [](const kv_pair& kv) { return kv.key; });
    return _keys;
  }

private:
  // we should also use a list to store all the values by the insert order
  list_type _list;
  std::unordered_map<key_type, list_type::iterator> _map;
};

TEST_CASE("LinkedHashMap basic operations") {
  LinkedHashMap map;

  // Test empty map
  CHECK(map.contains("key1") == false);
  CHECK_THROWS_AS(map.get("key1"), std::out_of_range);
  CHECK(map.keys().empty());

  // Test put and get
  map.put("key1", 100);
  CHECK(map.contains("key1") == true);
  CHECK(map.get("key1") == 100);

  map.put("key2", 200);
  CHECK(map.get("key2") == 200);

  // Test update existing key
  map.put("key1", 150);
  CHECK(map.get("key1") == 150);

  // Test keys in insertion order
  std::vector<key_type> expected_keys = {"key1", "key2"};
  CHECK(map.keys() == expected_keys);

  // Test remove
  map.remove("key1");
  CHECK(map.contains("key1") == false);
  CHECK_THROWS_AS(map.get("key1"), std::out_of_range);
  CHECK(map.keys() == std::vector<key_type>{"key2"});

  // Test insertion order is preserved
  map.put("key3", 300);
  map.put("key4", 400);
  map.put("key5", 500);

  expected_keys = {"key2", "key3", "key4", "key5"};
  CHECK(map.keys() == expected_keys);

  // Test removing and re-adding
  map.remove("key3");
  map.put("key3", 350);
  expected_keys = {"key2", "key4", "key5", "key3"};
  CHECK(map.keys() == expected_keys);

  // Test removing non-existent key
  map.remove("non_existent");
  CHECK(map.keys() == expected_keys);
}

TEST_CASE("LinkedHashMap with many elements") {
  LinkedHashMap map;

  // Add 100 elements
  for (int i = 0; i < 100; i++) {
    map.put("key" + std::to_string(i), i);
  }

  // Check all elements exist
  for (int i = 0; i < 100; i++) {
    CHECK(map.contains("key" + std::to_string(i)));
    CHECK(map.get("key" + std::to_string(i)) == i);
  }

  // Check keys are in correct order
  std::vector<key_type> expected_keys;
  for (int i = 0; i < 100; i++) {
    expected_keys.push_back("key" + std::to_string(i));
  }
  CHECK(map.keys() == expected_keys);

  // Remove every other element
  for (int i = 0; i < 100; i += 2) {
    map.remove("key" + std::to_string(i));
  }

  // Check remaining elements
  for (int i = 1; i < 100; i += 2) {
    CHECK(map.contains("key" + std::to_string(i)));
    CHECK(!map.contains("key" + std::to_string(i - 1)));
  }

  // Check remaining keys in correct order
  expected_keys.clear();
  for (int i = 1; i < 100; i += 2) {
    expected_keys.push_back("key" + std::to_string(i));
  }
  CHECK(map.keys() == expected_keys);
}
