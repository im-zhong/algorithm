// 2025/5/3
// zhangzhong
// https://leetcode.cn/problems/lfu-cache/description/
// LFU 算法的淘汰策略是 Least Frequently
// Used，也就是每次淘汰那些使用次数最少的数据。

// 460. LFU缓存

// 请你为 最不经常使用（LFU）缓存算法设计并实现数据结构。

// 实现 LFUCache 类：

// LFUCache(int capacity) - 用数据结构的容量 capacity 初始化对象
// int get(int key) - 如果键 key 存在于缓存中，则获取键的值，否则返回 -1 。
// void put(int key, int value) - 如果键 key
// 已存在，则变更其值；如果键不存在，请插入键值对。当缓存达到其容量 capacity
// 时，则应该在插入新项之前，移除最不经常使用的项。在此问题中，当存在平局（即两个或更多个键具有相同使用频率）时，应该去除
// 最久未使用 的键。 为了确定最不常使用的键，可以为缓存中的每个键维护一个
// 使用计数器 。使用计数最小的键是最久未使用的键。heap?

// 当一个键首次插入到缓存中时，它的使用计数器被设置为 1 (由于 put
// 操作)。对缓存中的键执行 get 或 put 操作，使用计数器的值将会递增。

// 函数 get 和 put 必须以 O(1) 的平均时间复杂度运行。 heap + linked hash map ?

// 示例：

// 输入：
// ["LFUCache", "put", "put", "get", "put", "get", "get", "put", "get", "get",
// "get"]
// [[2], [1, 1], [2, 2], [1], [3, 3], [2], [3], [4, 4], [1], [3], [4]]
// 输出：
// [null, null, null, 1, null, -1, 3, null, -1, 3, 4]

// 解释：
// // cnt(x) = 键 x 的使用计数
// // cache=[] 将显示最后一次使用的顺序（最左边的元素是最近的）
// LFUCache lfu = new LFUCache(2);
// lfu.put(1, 1);   // cache=[1,_], cnt(1)=1
// lfu.put(2, 2);   // cache=[2,1], cnt(2)=1, cnt(1)=1
// lfu.get(1);      // 返回 1
//                  // cache=[1,2], cnt(2)=1, cnt(1)=2
// lfu.put(3, 3);   // 去除键 2 ，因为 cnt(2)=1 ，使用计数最小
//                  // cache=[3,1], cnt(3)=1, cnt(1)=2
// lfu.get(2);      // 返回 -1（未找到）
// lfu.get(3);      // 返回 3
//                  // cache=[3,1], cnt(3)=2, cnt(1)=2
// lfu.put(4, 4);   // 去除键 1 ，1 和 3 的 cnt 相同，但 1 最久未使用
//                  // cache=[4,3], cnt(4)=1, cnt(3)=2
// lfu.get(1);      // 返回 -1（未找到）
// lfu.get(3);      // 返回 3
//                  // cache=[3,4], cnt(4)=1, cnt(3)=3
// lfu.get(4);      // 返回 4
//                  // cache=[3,4], cnt(4)=2, cnt(3)=3

// 提示：

// 1 <= capacity <= 104
// 0 <= key <= 105
// 0 <= value <= 109
// 最多调用 2 * 105 次 get 和 put 方法

// 因为咱们的选择的就是简单的策略，所以实现上，还是选择简单的方式
// 先实现一个简单的 linked_hash_set 用来实现LRUCache only with keys

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
  Pair front() const { return *_list.begin(); }

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

// 不对！我们这里需要的是一个没有capacity的LRU
class LRUCache {
public:
  // LRUCache() {}

  int front_key() const { return _map.front().key; }

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

  bool empty() const noexcept { return _map.empty(); }

  void put(int key, int value) {
    // 没有没有capacity，就永远不会出发eject！
    // if (!_map.contains(key) && _map.size() == _capacity) {
    //   _map.erase(_map.front().key);
    //   // just insert into the map
    //   // oh, first check the capacity
    //   //   if (_map.size() == _capacity) {
    //   //     // we need to erase the front of the list
    //   //     // erase the element on the tail
    //   //     _map.erase(_map.front());
    //   //   }

    //   //   // ok, now we get the sufficient space to store the new kv pair
    //   // assert(_map.size() < _capacity);

    //   //   _map.insert(key, value);
    // }

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
    // assert(_map.size() <= _capacity);

    _map.insert(key, value);

    // then move this value to head
    move_key_to_head(key);
  }

  void erase(int key) { _map.erase(key); }

private:
  // int _capacity;
  LinkedHashMap _map;
};

class LFUCache {
public:
  LFUCache(int capacity) : capacity(capacity) {}

  int get(int key) {
    if (!contains(key)) {
      return -1;
    }

    // 对缓存中的键执行 get 或 put 操作，使用计数器的值将会递增。
    // 懂了！这里存在bug，remove key会把 key2freq[key]给删掉！
    int original_freq = key2freq[key];
    int value = remove_key(key);

    // 所以这里的++ 实际上会先插入一个0 然后++ 结果就是1
    // 所以我们需要提前保存，origianl freq
    // 然后在put to freq 中更新 key2frea
    // key2freq[key]++;
    // find next freq
    put_to_freq(original_freq + 1, key, value);

    return value;
  }

  // TODO
  // 那就还需要一个函数，就是单纯的查看key对应的value，不做任何的修改
  //   int peek(int key) {
  //     if (!contains(key)) {
  //       return -1;
  //     }

  //     // 竟然如此的困难！！！
  //     // 或许LRUCache仅保存key，然后key2{freq, value} 这样的结构会更好写？
  //   }

  //   // 应该抽象出一个函数，update value
  //   int update(int key, int value) {
  //     int original_freq = key2freq[key];
  //     remove_key(key);

  //     // 所以这里的++ 实际上会先插入一个0 然后++ 结果就是1
  //     // 所以我们需要提前保存，origianl freq
  //     // 然后在put to freq 中更新 key2frea
  //     // key2freq[key]++;
  //     // find next freq
  //     put_to_freq(original_freq + 1, key, value);
  //   }

  int remove_key(int key) {

    if (!contains(key)) {
      return -1;
    }

    int original_freq = key2freq[key];
    // BUG: 忘了删除key2freq里面了
    key2freq.erase(key);

    // from original_freq list, erase the key, and move to next freq list
    auto& lru = freq2lru[original_freq];
    int value = lru.get(key);
    // 卧槽，lru没法直接删除一个key吗
    lru.erase(key);
    // 同时，别忘了处理minfreq
    // 如果此时lru已经是空的，那么需要删除
    if (lru.empty()) {
      // 如果我们的min freq 就是要被删除的freq，那么freq应该+1
      if (minfreq == original_freq) {
        minfreq++;
      }
      // 最后删除相应的lru cache
      freq2lru.erase(original_freq);
    }

    return value;
  }

  void put_to_freq(int freq, int key, int value) {
    // update key2freq
    key2freq[key] = freq;
    // check if there is such freq list
    if (freq2lru.count(freq) <= 0) {
      freq2lru[freq] = LRUCache{};
    }
    auto& lru = freq2lru[freq];
    lru.put(key, value);
  }

  void put(int key, int value) {
    if (!contains(key)) {
      // 如果key不存在，那么我们需要插入新的key

      // 如果已经满了，那么需要先删除一个LFU
      if (size() == capacity) {
        remove_lfu();
      }

      assert(size() < capacity);

      // 新插入的key，minfreq = 1
      minfreq = 1;
      // key2freq[key] = 1;
      put_to_freq(1, key, value);
      // 这一段代码重复了
      //   int freq = minfreq;
      //   if (freq2lru.count(freq) <= 0) {
      //     freq2lru[freq] = LRUCache{};
      //   }
      //   auto& next_lru = freq2lru[freq];
      //   next_lru.put(key, value);
      // 草，忘了更新 key2freq

      // 这一段代码重复了
    } else {

      // 若key已经存在，更新他的值
      // 然后就像调用get key那样即可
      // 先删除，再增加！
      // 把get拆成两个函数！
      int original_freq = key2freq[key];
      remove_key(key);
      // BUG! 忘了，这里也要改！
      // key2freq[key]++;
      put_to_freq(original_freq + 1, key, value);
    }
  }

  void remove_lfu() {
    // 这里最关键的就是找到符合lfu的元素
    // 肯定在minfreq指向的lru中
    auto& lru = freq2lru[minfreq];
    // 最应该删除的元素肯定是链表头的元素
    int lfu_key = lru.front_key();
    remove_key(lfu_key);
  }

  bool contains(int key) { return key2freq.count(key) > 0; }
  size_t size() const noexcept { return key2freq.size(); }

private:
  int capacity;
  std::unordered_map<int, int> key2freq;
  std::unordered_map<int, LRUCache> freq2lru;
  int minfreq;
};

/**
 * Your LFUCache object will be instantiated and called as such:
 * LFUCache* obj = new LFUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */

TEST_CASE("LFUCache example from problem description") {
  LFUCache lfu(2);

  lfu.put(1, 1);          // cache=[1,_], cnt(1)=1
  lfu.put(2, 2);          // cache=[2,1], cnt(2)=1, cnt(1)=1
  CHECK(lfu.get(1) == 1); // cache=[1,2], cnt(2)=1, cnt(1)=2

  lfu.put(3, 3);           // evicts key 2, cache=[3,1], cnt(3)=1, cnt(1)=2
  CHECK(lfu.get(2) == -1); // returns -1 (not found)
  CHECK(lfu.get(3) == 3);  // cache=[3,1], cnt(3)=2, cnt(1)=2

  lfu.put(4, 4);           // evicts key 1, cache=[4,3], cnt(4)=1, cnt(3)=2
  CHECK(lfu.get(1) == -1); // returns -1 (not found)
  CHECK(lfu.get(3) == 3);  // cache=[3,4], cnt(4)=1, cnt(3)=3
  CHECK(lfu.get(4) == 4);  // cache=[3,4], cnt(4)=2, cnt(3)=3
}

// debug with ./build/algorithm/linked_list/test_leetcode_460_lfu_cache
// -tc='LFUCache capacity 1'
TEST_CASE("LFUCache capacity 1") {
  LFUCache cache(1);

  cache.put(1, 10);
  CHECK(cache.get(1) == 10);

  cache.put(2, 20); // Should evict key 1
  CHECK(cache.get(1) == -1);
  CHECK(cache.get(2) == 20);

  // Update existing key
  cache.put(2, 25);
  CHECK(cache.get(2) == 25);

  // Frequency has increased, so putting a new key should evict 2
  cache.get(2);     // Increase frequency to 2
  cache.put(3, 30); // Should still evict key 2 despite higher frequency
  CHECK(cache.get(2) == -1);
  CHECK(cache.get(3) == 30);
}

TEST_CASE("LFUCache frequency tracking") {
  LFUCache cache(3);

  cache.put(1, 10);
  cache.put(2, 20);
  cache.put(3, 30);

  // Increase frequency of key 1 and 2
  cache.get(1); // Frequency = 2
  cache.get(2); // Frequency = 2
  cache.get(1); // Frequency = 3

  // When adding key 4, key 3 should be evicted (lowest frequency)
  cache.put(4, 40);
  CHECK(cache.get(3) == -1); // Evicted
  CHECK(cache.get(1) == 10);
  CHECK(cache.get(2) == 20);
  CHECK(cache.get(4) == 40);
}

TEST_CASE("LFUCache frequency ties with LRU") {
  LFUCache cache(3);

  // All frequencies start at 1
  cache.put(1, 10);
  cache.put(2, 20);
  cache.put(3, 30);

  // Increase frequency of all to 2
  cache.get(1);
  cache.get(2);
  cache.get(3);

  // Increase frequency of key 1 to 3
  cache.get(1);

  // Access key 2, making key 3 the LRU among frequency 2 items
  cache.get(2);

  // Add key 4, should evict key 3 (same frequency as 2, but least recently
  // used)
  cache.put(4, 40);
  CHECK(cache.get(1) == 10);
  CHECK(cache.get(2) == 20);
  CHECK(cache.get(3) == -1); // Evicted
  CHECK(cache.get(4) == 40);
}

TEST_CASE("LFUCache update existing key") {
  LFUCache cache(2);

  cache.put(1, 10);
  cache.put(2, 20);

  // Update key 1, should increase its frequency
  cache.put(1, 15);
  CHECK(cache.get(1) == 15);

  // Adding key 3 should evict key 2 (lower frequency than key 1)
  cache.put(3, 30);
  CHECK(cache.get(1) == 15);
  CHECK(cache.get(2) == -1); // Evicted
  CHECK(cache.get(3) == 30);
}

// TEST_CASE("LFUCache with zero capacity") {
//   LFUCache cache(0);

//   cache.put(1, 10);
//   CHECK(cache.get(1) == -1); // Nothing should be stored
// }

TEST_CASE("LFUCache eviction with multiple frequency levels") {
  LFUCache cache(4);

  cache.put(1, 10);
  cache.put(2, 20);
  cache.put(3, 30);
  cache.put(4, 40);

  // Increase frequencies
  cache.get(1); // Frequency = 2
  cache.get(1); // Frequency = 3
  cache.get(2); // Frequency = 2
  cache.get(3); // Frequency = 2

  // Key 4 has lowest frequency (1)
  cache.put(5, 50); // Should evict key 4
  CHECK(cache.get(4) == -1);
  CHECK(cache.get(5) == 50);

  // Of keys with frequency 2, key 2 was accessed before key 3
  cache.put(6, 60); // Should evict key 2
  CHECK(cache.get(2) == -1);
  CHECK(cache.get(3) == 30);
  CHECK(cache.get(6) == 60);
}
