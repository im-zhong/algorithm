// 2025/5/3
// zhangzhong
// https://leetcode.cn/problems/lru-cache/description/
// LRU 算法的淘汰策略是 Least Recently
// Used，也就是每次淘汰那些最久没被使用的数据
// TIP: 目前的这个实现效率很低，但是非常简单。

// 146. LRU缓存
// 请你设计并实现一个满足  LRU (最近最少使用) 缓存 约束的数据结构。
// 实现 LRUCache 类：
// LRUCache(int capacity) 以 正整数 作为容量 capacity 初始化 LRU 缓存
// int get(int key) 如果关键字 key 存在于缓存中，则返回关键字的值，否则返回 -1
// 。 void put(int key, int value) 如果关键字 key 已经存在，则变更其数据值 value
// ；如果不存在，则向缓存中插入该组 key-value 。如果插入操作导致关键字数量超过
// capacity ，则应该 逐出 最久未使用的关键字。 函数 get 和 put 必须以 O(1)
// 的平均时间复杂度运行。

// 示例：

// 输入
// ["LRUCache", "put", "put", "get", "put", "get", "put", "get", "get", "get"]
// [[2], [1, 1], [2, 2], [1], [3, 3], [2], [4, 4], [1], [3], [4]]
// 输出
// [null, null, null, 1, null, -1, null, -1, 3, 4]

// 解释
// LRUCache lRUCache = new LRUCache(2);
// lRUCache.put(1, 1); // 缓存是 {1=1}
// lRUCache.put(2, 2); // 缓存是 {1=1, 2=2}
// lRUCache.get(1);    // 返回 1
// lRUCache.put(3, 3); // 该操作会使得关键字 2 作废，缓存是 {1=1, 3=3}
// lRUCache.get(2);    // 返回 -1 (未找到)
// lRUCache.put(4, 4); // 该操作会使得关键字 1 作废，缓存是 {4=4, 3=3}
// lRUCache.get(1);    // 返回 -1 (未找到)
// lRUCache.get(3);    // 返回 3
// lRUCache.get(4);    // 返回 4

// 提示：

// 1 <= capacity <= 3000
// 0 <= key <= 10000
// 0 <= value <= 105
// 最多调用 2 * 105 次 get 和 put

// I need another linked hash map

// 因为我们需要一个根据当前的链表节点从链表中删除该节点的能力，这个能力只有双链表才有
// 单链表必须拿着前一个节点才能删除下一个节点，而且也没有办法简单的在链表尾部添加元素
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <cassert>
#include <doctest/doctest.h>
#include <list>
#include <unordered_map>

struct Pair {
  int key;
  int value;
};

class LinkedHashMap {
public:
  using list_type = std::list<Pair>;
  using map_type = std::unordered_map<int, list_type::iterator>;

  size_t size() const noexcept { return _map.size(); }

  bool empty() const noexcept { return _map.empty(); }

  // 这个东西总归是一个map，map就有insert
  // erase，或者说，我们可以遵循unorderd_map的接口来设计

  // BUG! should return key, not value!
  Pair front() { return *_list.begin(); }

  void insert(int key, int value) {
    if (!contains(key)) {
      auto iter = _list.insert(_list.end(), Pair{key, value});
      _map[key] = iter;
    } else {
      // update key
      _map[key]->value = value;
    }
  }

  void erase(int key) {
    if (!contains(key)) {
      return;
    }

    auto iter = _map[key];
    _list.erase(iter);
    _map.erase(key);
  }

  // 还有索引操作
  bool contains(int key) { return _map.count(key) > 0; }

  // get, find, operator[], at
  int at(int key) { return _map.at(key)->value; }

private:
  list_type _list;
  map_type _map;
};

class LRUCache {
public:
  LRUCache(int capacity) : _capacity(capacity) {}

  // int get(int key) 如果关键字 key 存在于缓存中，则返回关键字的值，否则返回 -1
  int get(int key) { return move_key_to_head(key); }

  int move_key_to_head(int key) {
    if (!_map.contains(key)) {
      return -1;
    }

    // the key is in the list, we should move it to the end of the list
    // the eaist way is to erase from the linked hash map and then add it back
    // but this way is slow actually. so the most proper way is to impl a list
    // ourself.

    int value = _map.at(key);
    _map.erase(key);
    _map.insert(key, value);
    return value;
  }

  void put(int key, int value) {
    if (!_map.contains(key) && _map.size() == _capacity) {
      _map.erase(_map.front().key);
      // just insert into the map
      // oh, first check the capacity
      //   if (_map.size() == _capacity) {
      //     // we need to erase the front of the list
      //     // erase the element on the tail
      //     _map.erase(_map.front());
      //   }

      //   // ok, now we get the sufficient space to store the new kv pair
      assert(_map.size() < _capacity);

      //   _map.insert(key, value);
    }

    // else {
    //   // if we already have the key, just update the key
    //   // and move it to the head
    //   _map.insert(key, value);
    //   // then move this value to head
    //   move_key_to_head(key);
    // }

    // ok, now we get the sufficient space to store the new kv pair
    // 哦，我这里过度简化了代码，实际上是不对的。。。
    // 这里的assert实际上只有在上面的分支里才有效
    // 在这里应该是 <=
    assert(_map.size() <= _capacity);

    _map.insert(key, value);

    // then move this value to head
    move_key_to_head(key);
  }

private:
  int _capacity;
  LinkedHashMap _map;
};

/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache* obj = new LRUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */
TEST_CASE("LRUCache example from problem description") {
  LRUCache lRUCache(2);

  lRUCache.put(1, 1);
  lRUCache.put(2, 2);
  CHECK(lRUCache.get(1) == 1);

  lRUCache.put(3, 3);           // This should evict key 2
  CHECK(lRUCache.get(2) == -1); // Not found

  lRUCache.put(4, 4);           // This should evict key 1
  CHECK(lRUCache.get(1) == -1); // Not found
  CHECK(lRUCache.get(3) == 3);
  CHECK(lRUCache.get(4) == 4);
}

TEST_CASE("LRUCache basic operations") {
  LRUCache cache(3);

  // Test put and get
  cache.put(1, 100);
  cache.put(2, 200);
  cache.put(3, 300);

  CHECK(cache.get(1) == 100);
  CHECK(cache.get(2) == 200);
  CHECK(cache.get(3) == 300);

  // Test updating existing values
  cache.put(2, 250);
  CHECK(cache.get(2) == 250);

  // Test LRU eviction - 4 should replace 1 since 1 is least recently used
  cache.put(4, 400);
  CHECK(cache.get(1) == -1); // 1 should be evicted
  CHECK(cache.get(2) == 250);
  CHECK(cache.get(3) == 300);
  CHECK(cache.get(4) == 400);

  // Test that get() updates the LRU order
  // After getting 2, the order is now: 3,4,2
  CHECK(cache.get(2) == 250);

  // Adding 5 should evict 3 now
  cache.put(5, 500);
  CHECK(cache.get(3) == -1); // 3 should be evicted
  CHECK(cache.get(2) == 250);
  CHECK(cache.get(4) == 400);
  CHECK(cache.get(5) == 500);
}

TEST_CASE("LRUCache with capacity 1") {
  LRUCache cache(1);

  cache.put(1, 10);
  CHECK(cache.get(1) == 10);

  cache.put(2, 20);
  CHECK(cache.get(1) == -1); // Evicted
  CHECK(cache.get(2) == 20);

  // Test that get affects LRU order
  CHECK(cache.get(2) == 20);

  cache.put(3, 30);
  CHECK(cache.get(2) == -1); // Evicted
  CHECK(cache.get(3) == 30);

  // Update existing key
  cache.put(3, 35);
  CHECK(cache.get(3) == 35);
}

TEST_CASE("LRUCache get updates LRU order") {
  LRUCache cache(2);

  cache.put(1, 100);
  cache.put(2, 200);

  // Access 1, making 2 the least recently used
  CHECK(cache.get(1) == 100);

  // This should evict 2 instead of 1
  cache.put(3, 300);
  CHECK(cache.get(1) == 100);
  CHECK(cache.get(2) == -1); // Evicted
  CHECK(cache.get(3) == 300);
}
