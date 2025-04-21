// 2025/4/21
// zhangzhong
// https://leetcode.cn/problems/linked-list-cycle/

// 141. 环形链表

// 给你一个链表的头节点 head ，判断链表中是否有环。
// 如果链表中有某个节点，可以通过连续跟踪 next 指针再次到达，则链表中存在环。
// 为了表示给定链表中的环，评测系统内部使用整数 pos
// 来表示链表尾连接到链表中的位置（索引从 0 开始）。注意：pos 不作为参数进行传递
// 。仅仅是为了标识链表的实际情况。
// 如果链表中存在环 ，则返回 true 。 否则，返回 false 。

// 示例 1：
// 输入：head = [3,2,0,-4], pos = 1
// 输出：true
// 解释：链表中有一个环，其尾部连接到第二个节点。

// 示例 2：
// 输入：head = [1,2], pos = 0
// 输出：true
// 解释：链表中有一个环，其尾部连接到第一个节点。

// 示例 3：
// 输入：head = [1], pos = -1
// 输出：false
// 解释：链表中没有环。

// 提示：
// 链表中节点的数目范围是 [0, 104]
// -105 <= Node.val <= 105
// pos 为 -1 或者链表中的一个 有效索引 。

// 进阶：你能用 O(1)（即，常量）内存解决此问题吗？

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
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
  // 可以看的出来，这道题的代码和leetcode 876 链表中点 非常非常相似
  bool hasCycle(ListNode* head) {

    ListNode* fast = head;
    ListNode* slow = head;

    while (fast && fast->next) {
      slow = slow->next;
      fast = fast->next->next;

      // 反之，如果在这里相遇了，那就就是有huan
      if (fast == slow) {
        return true;
      }
    }

    // 如果快指针可能成功的走到链表的结尾，说明没有环
    return false;
  }
};

TEST_CASE("Example 1: Cycle connecting to second node") {
  Solution s;

  // Create list: 3->2->0->-4 with cycle to index 1 (second node)
  ListNode* head = new ListNode(3);
  ListNode* second = new ListNode(2);
  ListNode* third = new ListNode(0);
  ListNode* fourth = new ListNode(-4);

  head->next = second;
  second->next = third;
  third->next = fourth;
  fourth->next = second; // Create cycle back to second node (index 1)

  bool result = s.hasCycle(head);
  CHECK(result == true);

  // Note: Memory cleanup omitted for brevity (would require breaking the cycle
  // first)
}

TEST_CASE("Example 2: Cycle connecting to first node") {
  Solution s;

  // Create list: 1->2 with cycle to index 0 (first node)
  ListNode* head = new ListNode(1);
  ListNode* second = new ListNode(2);

  head->next = second;
  second->next = head; // Create cycle back to first node (index 0)

  bool result = s.hasCycle(head);
  CHECK(result == true);

  // Note: Memory cleanup omitted for brevity
}

TEST_CASE("Example 3: No cycle") {
  Solution s;

  // Create list: 1 with no cycle
  ListNode* head = new ListNode(1);

  bool result = s.hasCycle(head);
  CHECK(result == false);

  // Note: Memory cleanup omitted for brevity
}

TEST_CASE("Empty list") {
  Solution s;

  ListNode* head = nullptr;

  bool result = s.hasCycle(head);
  CHECK(result == false);
}
