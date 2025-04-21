// 2025/4/21
// zhangzhong
// https://leetcode.cn/problems/middle-of-the-linked-list/description/

// 876. 链表的中间结点

// 给你单链表的头结点 head ，请你找出并返回链表的中间结点。
// 如果有两个中间结点，则返回第二个中间结点。

// 示例 1：
// 输入：head = [1,2,3,4,5]
// 输出：[3,4,5]
// 解释：链表只有一个中间结点，值为 3 。

// 示例 2：
// 输入：head = [1,2,3,4,5,6]
// 输出：[4,5,6]
// 解释：该链表有两个中间结点，值分别为 3 和 4 ，返回第二个结点。

// 提示：
// 链表的结点数范围是 [1, 100]
// 1 <= Node.val <= 100

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
  // 感觉我的这个实现比较丑陋
  ListNode* middleNode(ListNode* head) {

    // 用不用dummy head呢？
    //

    ListNode dummy_head{0, head};

    // 链表至少有一个节点
    // ListNode* fast = head;
    // ListNode* slow = head;

    ListNode* fast = &dummy_head;
    ListNode* slow = &dummy_head;
    // 这样写返回的是even中的第一个节点
    // 如果想要返回第二个，可能需要一个dummy head
    while (fast->next) {
      fast = fast->next;
      // 只有当fast可以完整的越过两个节点，slow才会动一下
      if (fast->next) {
        fast = fast->next;
        slow = slow->next;
      }
    }

    slow = slow->next;

    return slow;
  }

  // 卧槽，这个是真的简单！
  ListNode* middleNode1(ListNode* head) {

    ListNode* fast = head;
    ListNode* slow = head;

    // 但是这里的判断条件和上面的一样，当且仅当，fast指针真的可以移动两步的时候
    // 才会去移动slow指针
    while (fast && fast->next) {
      slow = slow->next;
      fast = fast->next->next;
    }

    return slow;
  }
};

TEST_CASE("Example 1: Middle of odd-length list") {
  Solution s;

  // Create list: 1->2->3->4->5
  ListNode* head = new ListNode(
      1, new ListNode(2, new ListNode(3, new ListNode(4, new ListNode(5)))));

  ListNode* result = s.middleNode(head);

  // Expected: middle is [3,4,5]
  CHECK(result->val == 3);
  CHECK(result->next->val == 4);
  CHECK(result->next->next->val == 5);
  CHECK(result->next->next->next == nullptr);

  // Note: Memory cleanup omitted for brevity
}

TEST_CASE("Example 2: Middle of even-length list") {
  Solution s;

  // Create list: 1->2->3->4->5->6
  ListNode* head = new ListNode(
      1, new ListNode(
             2, new ListNode(
                    3, new ListNode(4, new ListNode(5, new ListNode(6))))));

  ListNode* result = s.middleNode(head);

  // Expected: middle is [4,5,6] (second middle node for even-length list)
  CHECK(result->val == 4);
  CHECK(result->next->val == 5);
  CHECK(result->next->next->val == 6);
  CHECK(result->next->next->next == nullptr);

  // Note: Memory cleanup omitted for brevity
}
