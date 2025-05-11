// 2025/5/10
// zhangzhong
// https://leetcode.cn/problems/remove-duplicates-from-sorted-list-ii/description/

// 82. 删除排序链表中的重复元素 2
// 给定一个已排序的链表的头 head ，
// 删除原始链表中所有重复数字的节点，只留下不同的数字 。返回 已排序的链表 。

// 示例 1：

// 输入：head = [1,2,3,3,4,4,5]
// 输出：[1,2,5]
// 示例 2：

// 输入：head = [1,1,1,2,3]
// 输出：[2,3]

// 提示：

// 链表中节点数目在范围 [0, 300] 内
// -100 <= Node.val <= 100
// 题目数据保证链表已经按升序 排列

// 可算明白了，是只返回不同的数字，leetcode 83是unique，数字都剩下一个

// 相当简单，只需要遍历一次链表

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

class Solution {
public:
  // what can I say, man. so easy!
  ListNode* deleteDuplicates(ListNode* head) {

    // 我这种做法，本质上还是快慢双指针的做法
    // work就是快指针
    // 而prev就是慢指针
    // 只不过我这里使用了一个count，让代码变得更容易理解了。
    // 相比这里的快慢指针代码
    // https://labuladong.online/algo/problem-set/linkedlist-two-pointers/#%E8%A7%A3%E6%B3%95%E4%BB%A3%E7%A0%81

    // 首先处理 corner case
    if (head == nullptr) {
      return nullptr;
    }

    // still use list dummy head trick
    ListNode dummy_head;
    dummy_head.next = nullptr;
    ListNode* insert_after = &dummy_head;

    // 关于一个数字是否重复，我们只有遍历他的下一个节点才能知道
    // 同时，处理单链表，我们必须拿着将要处理的节点的前一个节点才行

    ListNode* prev_node = head;
    // 你会发现，我们始终需要一个计数器，来计算某个数值的出现次数
    int count = 1;
    int prev_value = head->val;

    // 我们要看下一个值
    ListNode* work = prev_node->next;
    // 有两种情况，prevnode要挂到answer上
    // 1. work是空指针，且prev_value是第一次出现
    // 2. 出现了新的值，且count是1
    // 3. 如果仍然是旧的值，则count++
    for (ListNode* work = head->next; work != nullptr; work = work->next) {
      if (work->val == prev_value) {
        count++;
      } else {
        // 先插入再更新
        if (count == 1) {
          // 插入prev节点
          insert_after->next = prev_node;
          insert_after = insert_after->next;
        }

        // 只要和之前的值不一样，就需要更新
        prev_value = work->val;
        prev_node = work;
        count = 1;
      }
    }

    // 处理最后一个数字
    if (count == 1) {
      insert_after->next = prev_node;
      insert_after = insert_after->next;
    }
    insert_after->next = nullptr;
    return dummy_head.next;
  }
};

TEST_CASE("Example 1: Multiple duplicates in the middle") {
  // [1,2,3,3,4,4,5] -> [1,2,5]
  ListNode* n5 = new ListNode(5);
  ListNode* n4_2 = new ListNode(4, n5);
  ListNode* n4_1 = new ListNode(4, n4_2);
  ListNode* n3_2 = new ListNode(3, n4_1);
  ListNode* n3_1 = new ListNode(3, n3_2);
  ListNode* n2 = new ListNode(2, n3_1);
  ListNode* n1 = new ListNode(1, n2);

  Solution solution;
  ListNode* result = solution.deleteDuplicates(n1);

  // Check result is [1,2,5]
  CHECK(result->val == 1);
  CHECK(result->next->val == 2);
  CHECK(result->next->next->val == 5);
  CHECK(result->next->next->next == nullptr);
}

TEST_CASE("Example 2: Duplicates at the beginning") {
  // [1,1,1,2,3] -> [2,3]
  ListNode* n3 = new ListNode(3);
  ListNode* n2 = new ListNode(2, n3);
  ListNode* n1_3 = new ListNode(1, n2);
  ListNode* n1_2 = new ListNode(1, n1_3);
  ListNode* n1_1 = new ListNode(1, n1_2);

  Solution solution;
  ListNode* result = solution.deleteDuplicates(n1_1);

  // Check result is [2,3]
  CHECK(result->val == 2);
  CHECK(result->next->val == 3);
  CHECK(result->next->next == nullptr);
}

TEST_CASE("Empty list") {
  Solution solution;
  ListNode* result = solution.deleteDuplicates(nullptr);

  CHECK(result == nullptr);
}

TEST_CASE("List with a single element") {
  ListNode* head = new ListNode(5);

  Solution solution;
  ListNode* result = solution.deleteDuplicates(head);

  CHECK(result->val == 5);
  CHECK(result->next == nullptr);
}

TEST_CASE("List with all duplicates") {
  // [1,1,1,1,1] -> []
  ListNode* n5 = new ListNode(1);
  ListNode* n4 = new ListNode(1, n5);
  ListNode* n3 = new ListNode(1, n4);
  ListNode* n2 = new ListNode(1, n3);
  ListNode* n1 = new ListNode(1, n2);

  Solution solution;
  ListNode* result = solution.deleteDuplicates(n1);

  CHECK(result == nullptr);
}

TEST_CASE("List with no duplicates") {
  // [1,2,3,4,5] -> [1,2,3,4,5]
  ListNode* n5 = new ListNode(5);
  ListNode* n4 = new ListNode(4, n5);
  ListNode* n3 = new ListNode(3, n4);
  ListNode* n2 = new ListNode(2, n3);
  ListNode* n1 = new ListNode(1, n2);

  Solution solution;
  ListNode* result = solution.deleteDuplicates(n1);

  CHECK(result->val == 1);
  CHECK(result->next->val == 2);
  CHECK(result->next->next->val == 3);
  CHECK(result->next->next->next->val == 4);
  CHECK(result->next->next->next->next->val == 5);
  CHECK(result->next->next->next->next->next == nullptr);
}

TEST_CASE("Duplicates at the end") {
  // [1,2,3,4,4,4] -> [1,2,3]
  ListNode* n6 = new ListNode(4);
  ListNode* n5 = new ListNode(4, n6);
  ListNode* n4 = new ListNode(4, n5);
  ListNode* n3 = new ListNode(3, n4);
  ListNode* n2 = new ListNode(2, n3);
  ListNode* n1 = new ListNode(1, n2);

  Solution solution;
  ListNode* result = solution.deleteDuplicates(n1);

  CHECK(result->val == 1);
  CHECK(result->next->val == 2);
  CHECK(result->next->next->val == 3);
  CHECK(result->next->next->next == nullptr);
}
