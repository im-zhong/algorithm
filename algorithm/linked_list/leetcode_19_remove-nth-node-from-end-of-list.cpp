// 2025/4/21
// zhangzhong
// https://leetcode.cn/problems/remove-nth-node-from-end-of-list/

// 19. 删除链表的倒数第N个结点

// 给你一个链表，删除链表的倒数第 n 个结点，并且返回链表的头结点。

// 示例 1：
// 输入：head = [1,2,3,4,5], n = 2
// 输出：[1,2,3,5]

// 示例 2：
// 输入：head = [1], n = 1
// 输出：[]

// 示例 3：
// 输入：head = [1,2], n = 1
// 输出：[1]

// 提示：
// 链表中结点的数目为 sz
// 1 <= sz <= 30
// 0 <= Node.val <= 100
// 1 <= n <= sz

// 进阶：你能尝试使用一趟扫描实现吗？

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

struct ListNode {
  int val;
  ListNode* next;
  ListNode() : val(0), next(nullptr) {}
  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode* next) : val(x), next(next) {}
};

class Solution {
public:
  ListNode* removeNthFromEnd(ListNode* head, int n) {

    // 不对！
    // 使用一个duumyhead

    ListNode dummy_head{0, head};

    // 我们使用两个指针，一个是快指针，一个是慢指针
    ListNode* fast = &dummy_head;
    ListNode* slow = &dummy_head;

    // at first, fast move nth step
    // 假设输入的数据都是正确的
    for (int i = 0; i < n; i++) {
      fast = fast->next;
    }

    // 然后fast和slow同时移动楼
    // 这里为什么始终拿到目标结点的前一个节点，就是因为我们要删除他
    // 在单链表中，想要删除一个节点，必须拿到他的前一个节点
    // 这就是为什么我们是判断 fast->next
    // 和一开始，我们让指针指向dummy head的原因
    while (fast->next) {
      fast = fast->next;
      slow = slow->next;
    }

    // slow指向的下一个元素就是要删除的位置
    // 不用释放内存
    slow->next = slow->next->next;
    return dummy_head.next;
  }
};

TEST_CASE("Example 1: Remove 2nd node from end of 5-node list") {
  Solution s;

  // Create list: 1->2->3->4->5
  ListNode* head = new ListNode(
      1, new ListNode(2, new ListNode(3, new ListNode(4, new ListNode(5)))));

  ListNode* result = s.removeNthFromEnd(head, 2);

  // Expected: 1->2->3->5
  CHECK(result->val == 1);
  CHECK(result->next->val == 2);
  CHECK(result->next->next->val == 3);
  CHECK(result->next->next->next->val == 5);
  CHECK(result->next->next->next->next == nullptr);

  // Note: Memory cleanup omitted for brevity
}

TEST_CASE("Example 2: Remove 1st node from end of 1-node list") {
  Solution s;

  // Create list: 1
  ListNode* head = new ListNode(1);

  ListNode* result = s.removeNthFromEnd(head, 1);

  // Expected: []
  CHECK(result == nullptr);

  // Note: Memory cleanup omitted for brevity
}

TEST_CASE("Example 3: Remove 1st node from end of 2-node list") {
  Solution s;

  // Create list: 1->2
  ListNode* head = new ListNode(1, new ListNode(2));

  ListNode* result = s.removeNthFromEnd(head, 1);

  // Expected: 1
  CHECK(result->val == 1);
  CHECK(result->next == nullptr);

  // Note: Memory cleanup omitted for brevity
}
