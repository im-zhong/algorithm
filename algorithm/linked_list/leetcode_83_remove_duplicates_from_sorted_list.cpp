// 2025/4/22
// zhangzhong
// https://leetcode.cn/problems/remove-duplicates-from-sorted-list/

// 83. 删除排序链表中的重复元素

// 给定一个已排序的链表的头 head ， 删除所有重复的元素，使每个元素只出现一次
// 。返回 已排序的链表 。

// 示例 1：
// 输入：head = [1,1,2]
// 输出：[1,2]

// 示例 2：
// 输入：head = [1,1,2,3,3]
// 输出：[1,2,3]

// 提示：
// 链表中节点数目在范围 [0, 300] 内
// -100 <= Node.val <= 100
// 题目数据保证链表已经按升序 排列

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
  ListNode* deleteDuplicates(ListNode* head) {
    // make sure we at least have one node
    if (head == nullptr) {
      return nullptr;
    }

    // 咱们先按照比较简单的思路
    // 就是删除相邻的重复节点

    ListNode* prev = head;
    while (prev->next) {
      if (prev->next->val == prev->val) {
        // delete prev->next
        prev->next = prev->next->next;
        // 不对，在这种情况下，prev不用动
      } else {
        // 如果碰到了两个相邻的不重复元素
        // 则prev向后移动
        prev = prev->next;
      }
    }

    return head;
  }

  // 注意对比这里的实现和leetcode 26  removeDuplicates2 中的实现
  ListNode* deleteDuplicates2(ListNode* head) {

    // 第二种写法是类似 leetcode 26 的unique的写法
    // 也就是快慢指针的写法
    // 同样，我们应该首先假设链表非空
    // 能够拿到第一个元素的话，代码会简单很多

    if (head == nullptr) {
      return nullptr;
    }

    ListNode* slow = head;
    // ListNode* fast = head->next;

    for (ListNode* fast = head->next; fast; fast = fast->next) {
      if (fast->val == slow->val) {
        continue;
      }

      // 这里应该移动节点！
      // slow = slow->next;
      // 不对，还有需要注意的地方
      // 当slow->next 就是fast的时候
      // 什么都不要做
      //   if (slow->next == fast) {
      //     slow = slow->next;
      //   } else {
      //     slow->next = fast;
      //     slow = slow->next;
      //     // 这里需要断开链表，否则最后重复的一系列元素都会连在一起
      //     // 我觉得在刷题的时候注意断开链表是一个不错的习惯
      //     // 不对，不是在这里不能断开
      //     // slow->next = nullptr;
      //   }
      slow->next = fast;
      slow = slow->next;
    }
    // 是在这里断开
    slow->next = nullptr;
    return head;
  }
};

// Helper functions for list operations
ListNode* createList(const std::vector<int>& values) {
  ListNode dummy;
  ListNode* tail = &dummy;

  for (int val : values) {
    tail->next = new ListNode(val);
    tail = tail->next;
  }

  return dummy.next;
}

std::vector<int> listToVector(ListNode* head) {
  std::vector<int> result;
  while (head) {
    result.push_back(head->val);
    head = head->next;
  }
  return result;
}

void freeList(ListNode* head) {
  while (head) {
    ListNode* temp = head;
    head = head->next;
    delete temp;
  }
}

TEST_CASE("Example 1: [1,1,2] -> [1,2]") {
  Solution solution;
  ListNode* head = createList({1, 1, 2});
  ListNode* result = solution.deleteDuplicates(head);
  std::vector<int> resultVector = listToVector(result);
  CHECK(resultVector == std::vector<int>{1, 2});
  freeList(result);
}

TEST_CASE("Example 2: [1,1,2,3,3] -> [1,2,3]") {
  Solution solution;
  ListNode* head = createList({1, 1, 2, 3, 3});
  ListNode* result = solution.deleteDuplicates(head);
  std::vector<int> resultVector = listToVector(result);
  CHECK(resultVector == std::vector<int>{1, 2, 3});
  freeList(result);
}

TEST_CASE("Empty list: [] -> []") {
  Solution solution;
  ListNode* head = nullptr;
  ListNode* result = solution.deleteDuplicates(head);
  std::vector<int> resultVector = listToVector(result);
  CHECK(resultVector.empty());
}

TEST_CASE("Single element: [1] -> [1]") {
  Solution solution;
  ListNode* head = createList({1});
  ListNode* result = solution.deleteDuplicates(head);
  std::vector<int> resultVector = listToVector(result);
  CHECK(resultVector == std::vector<int>{1});
  freeList(result);
}

TEST_CASE("All duplicates: [1,1,1] -> [1]") {
  Solution solution;
  ListNode* head = createList({1, 1, 1});
  ListNode* result = solution.deleteDuplicates(head);
  std::vector<int> resultVector = listToVector(result);
  CHECK(resultVector == std::vector<int>{1});
  freeList(result);
}

TEST_CASE("No duplicates: [1,2,3] -> [1,2,3]") {
  Solution solution;
  ListNode* head = createList({1, 2, 3});
  ListNode* result = solution.deleteDuplicates(head);
  std::vector<int> resultVector = listToVector(result);
  CHECK(resultVector == std::vector<int>{1, 2, 3});
  freeList(result);
}
