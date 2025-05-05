// 2025/5/5
// zhangzhong
// 380. O(1)时间插入，删除和获取随机元素

// 实现RandomizedSet 类：

// RandomizedSet() 初始化 RandomizedSet 对象
// bool insert(int val) 当元素 val 不存在时，向集合中插入该项，并返回 true
// ；否则，返回 false 。 bool remove(int val) 当元素 val
// 存在时，从集合中移除该项，并返回 true ；否则，返回 false 。 int getRandom()
// 随机返回现有集合中的一项（测试用例保证调用此方法时集合中至少存在一个元素）。每个元素应该有
// 相同的概率 被返回。 你必须实现类的所有函数，并满足每个函数的 平均
// 时间复杂度为 O(1) 。

// 示例：

// 输入
// ["RandomizedSet", "insert", "remove", "insert", "getRandom", "remove",
// "insert", "getRandom"]
// [[], [1], [2], [2], [], [1], [2], []]
// 输出
// [null, true, false, true, 2, true, false, 2]

// 解释
// RandomizedSet randomizedSet = new RandomizedSet();
// randomizedSet.insert(1); // 向集合中插入 1 。返回 true 表示 1 被成功地插入。
// randomizedSet.remove(2); // 返回 false ，表示集合中不存在 2 。
// randomizedSet.insert(2); // 向集合中插入 2 。返回 true 。集合现在包含 [1,2]
// 。 randomizedSet.getRandom(); // getRandom 应随机返回 1 或 2 。
// randomizedSet.remove(1); // 从集合中移除 1 ，返回 true 。集合现在包含 [2] 。
// randomizedSet.insert(2); // 2 已在集合中，所以返回 false 。
// randomizedSet.getRandom(); // 由于 2 是集合中唯一的数字，getRandom 总是返回 2
// 。

// 提示：

// -231 <= val <= 231 - 1
// 最多调用 insert、remove 和 getRandom 函数 2 * 105 次
// 在调用 getRandom 方法时，数据结构中 至少存在一个 元素。

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <chrono>
#include <doctest/doctest.h>
#include <random>
#include <unordered_map>
#include <vector>

// 和我昨天想的一样，就是hash 加上一个 array，然后hash保存的是数组的索引
// 删除的时候把腰删除的元素和尾部的元素交换，然后修改hash的索引即可
// 为了支持任意多的元素，这里的底层数组可以使用vector实现

class RandomizedSet {
public:
  RandomizedSet()
      : e(std::chrono::system_clock::now().time_since_epoch().count()) {}

  // bool insert(int val) 当元素 val 不存在时，向集合中插入该项，并返回 true
  // ；否则，返回 false
  bool insert(int val) {
    if (contains(val)) {
      return false;
    }

    // just push back
    _vec.push_back(val);
    _map.insert({val, _vec.size() - 1});
    return true;
  }
  // bool remove(int val) 当元素 val
  // 存在时，从集合中移除该项，并返回 true ；否则，返回 false
  bool remove(int val) {
    if (!contains(val)) {
      return false;
    }

    int index = _map[val];
    // 交换index指向的元素和最后一个位置的元素
    std::swap(_vec[index], _vec.back());

    // now we need to update _map
    // erase the val

    // 如果要删除的元素就是最后一个，那么可能会出问题
    // 我们上面已经把元素给删除了，下面还会访问这个元素
    // 所以这种情况需要额外处理？
    // 或者我们可以在最后再执行删除的动作？

    // update the swapped value, because the original value in the back
    // is been swapped to index
    _map[_vec[index]] = index;

    // ok，这样的顺序就可以正确的处理特殊的情况了
    _map.erase(val);
    // just pop back
    _vec.pop_back();
    return true;
  }

  int getRandom() {
    std::uniform_int_distribution<int> u(0, _vec.size() - 1);
    return _vec[u(e)];
  }

  bool contains(int val) { return _map.count(val) > 0; }

private:
  std::unordered_map<int, int> _map;
  std::vector<int> _vec;
  std::default_random_engine e;
};

/**
 * Your RandomizedSet object will be instantiated and called as such:
 * RandomizedSet* obj = new RandomizedSet();
 * bool param_1 = obj->insert(val);
 * bool param_2 = obj->remove(val);
 * int param_3 = obj->getRandom();
 */

TEST_CASE("RandomizedSet example from problem description") {
  RandomizedSet randomizedSet;

  CHECK(randomizedSet.insert(1) == true); // Inserting 1 should succeed
  CHECK(randomizedSet.remove(2) ==
        false); // Removing non-existent element should fail
  CHECK(randomizedSet.insert(2) == true); // Inserting 2 should succeed

  // Random element should be either 1 or 2
  int random = randomizedSet.getRandom();
  CHECK((random == 1 || random == 2));

  CHECK(randomizedSet.remove(1) == true); // Removing 1 should succeed
  CHECK(randomizedSet.insert(2) ==
        false); // Inserting existing element should fail

  // Only 2 remains, so getRandom should return 2
  CHECK(randomizedSet.getRandom() == 2);
}

TEST_CASE("RandomizedSet basic operations") {
  RandomizedSet set;

  // Test empty set
  CHECK(set.remove(1) == false);

  // Test inserting and contains
  CHECK(set.insert(10) == true);
  CHECK(set.contains(10) == true);
  CHECK(set.contains(20) == false);

  // Test duplicate insertion
  CHECK(set.insert(10) == false);

  // Test removing existing element
  CHECK(set.remove(10) == true);
  CHECK(set.contains(10) == false);

  // Test removing already removed element
  CHECK(set.remove(10) == false);
}

TEST_CASE("RandomizedSet with multiple elements") {
  RandomizedSet set;

  // Insert multiple elements
  CHECK(set.insert(5) == true);
  CHECK(set.insert(10) == true);
  CHECK(set.insert(15) == true);
  CHECK(set.insert(20) == true);
  CHECK(set.insert(25) == true);

  // Check all elements exist
  CHECK(set.contains(5) == true);
  CHECK(set.contains(10) == true);
  CHECK(set.contains(15) == true);
  CHECK(set.contains(20) == true);
  CHECK(set.contains(25) == true);

  // Remove middle element
  CHECK(set.remove(15) == true);
  CHECK(set.contains(15) == false);
  CHECK(set.contains(5) == true);
  CHECK(set.contains(25) == true);

  // Random should be one of the remaining elements
  int random = set.getRandom();
  CHECK((random == 5 || random == 10 || random == 20 || random == 25));

  // Test removing first element
  CHECK(set.remove(5) == true);
  CHECK(set.contains(5) == false);

  // Test removing last element
  CHECK(set.remove(25) == true);
  CHECK(set.contains(25) == false);
}

TEST_CASE("RandomizedSet edge cases") {
  RandomizedSet set;

  // Insert and immediately remove
  CHECK(set.insert(42) == true);
  CHECK(set.remove(42) == true);
  CHECK(set.contains(42) == false);

  // Insert, remove, then insert again
  CHECK(set.insert(100) == true);
  CHECK(set.remove(100) == true);
  CHECK(set.insert(100) == true);
  CHECK(set.contains(100) == true);
  CHECK(set.remove(100) == true);

  // Add multiple elements, then remove all
  set.insert(1);
  set.insert(2);
  set.insert(3);
  set.remove(1);
  set.remove(2);
  set.remove(3);

  // Add new element after clearing
  CHECK(set.insert(200) == true);
  CHECK(set.getRandom() == 200); // Only one element, so must be 200
}
