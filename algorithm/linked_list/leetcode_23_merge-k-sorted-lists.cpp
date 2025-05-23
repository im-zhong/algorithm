// 2025/4/21
// zhangzhong
// https://leetcode.cn/problems/merge-k-sorted-lists/

// 23. 合并K个升序链表

// 给你一个链表数组，每个链表都已经按升序排列。
// 请你将所有链表合并到一个升序链表中，返回合并后的链表。

// 示例 1：
// 输入：lists = [[1,4,5],[1,3,4],[2,6]]
// 输出：[1,1,2,3,4,4,5,6]
// 解释：链表数组如下：
// [
//   1->4->5,
//   1->3->4,
//   2->6
// ]
// 将它们合并到一个有序链表中得到。
// 1->1->2->3->4->4->5->6

// 示例 2：
// 输入：lists = []
// 输出：[]

// 示例 3：
// 输入：lists = [[]]
// 输出：[]

// 提示：
// k == lists.length
// 0 <= k <= 10^4
// 0 <= lists[i].length <= 500
// -10^4 <= lists[i][j] <= 10^4
// lists[i] 按 升序 排列
// lists[i].length 的总和不超过 10^4

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include <queue>
#include <vector>

struct ListNode {
  int val;
  ListNode* next;
  ListNode() : val(0), next(nullptr) {}
  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode* next) : val(x), next(next) {}
};

struct IndexedValue {
  size_t index;
  int value;

  bool operator>(const IndexedValue& other) const noexcept {
    return this->value > other.value;
  }
};

class Solution {
public:
  // mine
  ListNode* mergeKLists(std::vector<ListNode*>& lists) {

    // 用优先队列做 NlogK
    // 直接把lists全部拿出来排序 NlogN
    // K << N, 那么用优先队列做更快的

    // 需要一个dummy head
    ListNode head{0, nullptr};
    ListNode* work = &head;

    // C++
    // 中的pq默认是大顶堆，既最大的元素在top，默认的comparator是std::less
    // 这里我们需要按生序排序，所以需要一个小顶堆 std::greater
    std::priority_queue<IndexedValue, std::vector<IndexedValue>,
                        std::greater<IndexedValue>>
        pq;

    // 第一步应该是把所有的lists的第一个元素放到pg中，如果有的话
    for (size_t i = 0; i < lists.size(); i++) {
      if (!lists[i]) {
        continue;
      }

      // 我们需要知道这个元素来自那个链表
      // 所以还需要这个链表对应的下标
      // 作为一个pair放到pq中
      pq.push(IndexedValue{i, lists[i]->val});
    }

    // 然后，我们需要从pg中拿出一个
    while (!pq.empty()) {
      // get the front and pop it
      IndexedValue indexed_value{pq.top()};
      pq.pop();

      // insert into the tail of the dummy list
      work->next = lists[indexed_value.index];
      work = work->next;

      // 然后对应的链表需要向后一个
      lists[indexed_value.index] = lists[indexed_value.index]->next;
      // 如果非空，需要将这个新的元素插入到pq中
      if (lists[indexed_value.index]) {
        pq.push(
            IndexedValue{indexed_value.index, lists[indexed_value.index]->val});
      }
    }

    return head.next;
  }

  // others, use pointer directly，这样做快了很多
  ListNode* mergeKLists1(std::vector<ListNode*>& lists) {

    // 用优先队列做 NlogK
    // 直接把lists全部拿出来排序 NlogN
    // K << N, 那么用优先队列做更快的
    // K是链表的条数，N是节点的总数
    // 优先队列 pq 中的元素个数最多是k，所以一次 poll 或者 add
    // 方法的时间复杂度是 logK 所有的链表节点都会被加入和弹出 pq

    // 需要一个dummy head
    ListNode head{0, nullptr};
    ListNode* work = &head;

    // C++
    // 中的pq默认是大顶堆，既最大的元素在top，默认的comparator是std::less
    // 这里我们需要按生序排序，所以需要一个小顶堆 std::greater

    auto cmp = [](const ListNode* lhs, const ListNode* rhs) -> bool {
      return lhs->val > rhs->val;
    };
    // 相比于使用标准库的std::less<int> std::greater<int>
    // 使用自己写的compaartor，必须传入一个comparator的实例
    std::priority_queue<ListNode*, std::vector<ListNode*>, decltype(cmp)> pq{
        cmp};

    // 第一步应该是把所有的lists的第一个元素放到pg中，如果有的话
    for (ListNode* list : lists) {
      if (!list) {
        continue;
      }

      // 我们需要知道这个元素来自那个链表
      // 所以还需要这个链表对应的下标
      // 作为一个pair放到pq中
      pq.push(list);
    }

    // 然后，我们需要从pg中拿出一个
    while (!pq.empty()) {
      // get the front and pop it
      ListNode* curr_min = pq.top();
      pq.pop();

      // insert into the tail of the dummy list
      work->next = curr_min;
      work = work->next;

      // 然后对应的链表需要向后一个

      // 如果非空，需要将这个新的元素插入到pq中
      if (curr_min->next) {
        pq.push(curr_min->next);
      }
    }

    return head.next;
  }

  // 递归，分治算法
  // 先给出合并两个链表的算法
  ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
    // create a dummp head
    ListNode head{0, nullptr};
    // make head point to it self, for recurrent list
    // head.next = &head;
    // 不过好像也不需要，因为题目中给出的是一个简单的单链表

    // 再定义一个tail 指向最后一个元素
    // 每次都在tail后面插入元素 然后更新tail
    ListNode* tail = &head;

    while (list1 && list2) {
      // 找到两者的最小值
      if (list1->val < list2->val) {
        tail->next = list1;
        list1 = list1->next;
        // 可以吧tail提取出来
        // tail = tail->next;
      } else {
        tail->next = list2;
        list2 = list2->next;
        // tail = tail->next;
      }
      tail = tail->next;
    }

    // 这个时候可能有链表还有，就全都挂到最后即可
    // 哦 不对 这里不用在来一个循环了
    // 这是链表，直接挂在后面就行了！
    ListNode* remaining_list = list1 ? list1 : list2;
    // while (remaining_list) {
    //   tail->next = remaining_list;
    //   remaining_list = remaining_list->next;
    //   tail = tail->next;
    // }
    tail->next = remaining_list;

    // 那这样的话，这里就不用更新最后的tail地址了
    // 因为merge到最后 一定会有一个链表会剩下！
    // 最终，让tail指向nullptr, 结束这个链表
    // tail->next = nullptr;
    return head.next;
  }

  ListNode* mergeKListsImpl(std::vector<ListNode*>& lists, int begin, int end) {

    // 递归结束条件
    if (begin >= end) {
      return nullptr;
    }
    if ((end - begin) == 1) {
      return lists[begin];
    }

    int middle = begin + (end - begin) / 2;
    ListNode* left_list = mergeKListsImpl(lists, begin, middle);
    ListNode* right_list = mergeKListsImpl(lists, middle, end);
    return mergeTwoLists(left_list, right_list);
  }

  // 然后解决合并K个生序链表的算法
  ListNode* mergeKLists3(std::vector<ListNode*>& lists) {

    // 应该先找到链表的中点
    // 把两个链表给分开？
    // 不对不对！傻逼了
    // 参数是一个数组啊
    // 我们是把左边的所有链表合并成一条有序的链表
    // 右边的链表合并成一条有序的链表
    // 然后再把左边和右边的给合并起来
    // 需要写一个辅助函数

    return mergeKListsImpl(lists, 0, lists.size());
  }
};

TEST_CASE("Example 1: Multiple non-empty lists") {
  Solution s;

  // Create first list: 1->4->5
  ListNode* list1 = new ListNode(1, new ListNode(4, new ListNode(5)));

  // Create second list: 1->3->4
  ListNode* list2 = new ListNode(1, new ListNode(3, new ListNode(4)));

  // Create third list: 2->6
  ListNode* list3 = new ListNode(2, new ListNode(6));

  std::vector<ListNode*> lists = {list1, list2, list3};

  ListNode* result = s.mergeKLists(lists);

  // Expected: 1->1->2->3->4->4->5->6
  CHECK(result->val == 1);
  CHECK(result->next->val == 1);
  CHECK(result->next->next->val == 2);
  CHECK(result->next->next->next->val == 3);
  CHECK(result->next->next->next->next->val == 4);
  CHECK(result->next->next->next->next->next->val == 4);
  CHECK(result->next->next->next->next->next->next->val == 5);
  CHECK(result->next->next->next->next->next->next->next->val == 6);
  CHECK(result->next->next->next->next->next->next->next->next == nullptr);

  // Note: Memory cleanup omitted for brevity
}

TEST_CASE("Example 2: Empty list array") {
  Solution s;
  std::vector<ListNode*> lists;
  ListNode* result = s.mergeKLists(lists);
  CHECK(result == nullptr);
}

TEST_CASE("Example 3: Array with single empty list") {
  Solution s;
  std::vector<ListNode*> lists = {nullptr};
  ListNode* result = s.mergeKLists(lists);
  CHECK(result == nullptr);
}
