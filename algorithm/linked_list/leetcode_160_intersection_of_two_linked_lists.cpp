// 2025/4/21
// zhangzhong
// https://leetcode.cn/problems/intersection-of-two-linked-lists/description/

// 160. 相交链表

// 给你两个单链表的头节点 headA 和 headB
// ，请你找出并返回两个单链表相交的起始节点。如果两个链表不存在相交节点，返回
// null 。

// 图示两个链表在节点 c1 开始相交：

// 题目数据 保证 整个链式结构中不存在环。

// 注意，函数返回结果后，链表必须 保持其原始结构 。

// 自定义评测：

// 评测系统 的输入如下（你设计的程序 不适用 此输入）：

// intersectVal - 相交的起始节点的值。如果不存在相交节点，这一值为 0
// listA - 第一个链表
// listB - 第二个链表
// skipA - 在 listA 中（从头节点开始）跳到交叉节点的节点数
// skipB - 在 listB 中（从头节点开始）跳到交叉节点的节点数
// 评测系统将根据这些输入创建链式数据结构，并将两个头节点 headA 和 headB
// 传递给你的程序。如果程序能够正确返回相交节点，那么你的解决方案将被
// 视作正确答案 。

// 示例 1：

// 输入：intersectVal = 8, listA = [4,1,8,4,5], listB = [5,6,1,8,4,5], skipA =
// 2, skipB = 3 输出：Intersected at '8' 解释：相交节点的值为 8
// （注意，如果两个链表相交则不能为 0）。 从各自的表头开始算起，链表 A 为
// [4,1,8,4,5]，链表 B 为 [5,6,1,8,4,5]。 在 A 中，相交节点前有 2 个节点；在 B
// 中，相交节点前有 3 个节点。 — 请注意相交节点的值不为 1，因为在链表 A 和链表 B
// 之中值为 1 的节点 (A 中第二个节点和 B 中第三个节点)
// 是不同的节点。换句话说，它们在内存中指向两个不同的位置，而链表 A 和链表 B
// 中值为 8 的节点 (A 中第三个节点，B 中第四个节点) 在内存中指向相同的位置。

// 示例 2：

// 输入：intersectVal = 2, listA = [1,9,1,2,4], listB = [3,2,4], skipA = 3,
// skipB = 1 输出：Intersected at '2' 解释：相交节点的值为 2
// （注意，如果两个链表相交则不能为 0）。 从各自的表头开始算起，链表 A 为
// [1,9,1,2,4]，链表 B 为 [3,2,4]。 在 A 中，相交节点前有 3 个节点；在 B
// 中，相交节点前有 1 个节点。 示例 3：

// 输入：intersectVal = 0, listA = [2,6,4], listB = [1,5], skipA = 3, skipB = 2
// 输出：No intersection
// 解释：从各自的表头开始算起，链表 A 为 [2,6,4]，链表 B 为 [1,5]。
// 由于这两个链表不相交，所以 intersectVal 必须为 0，而 skipA 和 skipB
// 可以是任意值。 这两个链表不相交，因此返回 null 。

// 提示：

// listA 中节点数目为 m
// listB 中节点数目为 n
// 1 <= m, n <= 3 * 104
// 1 <= Node.val <= 105
// 0 <= skipA <= m
// 0 <= skipB <= n
// 如果 listA 和 listB 没有交点，intersectVal 为 0
// 如果 listA 和 listB 有交点，intersectVal == listA[skipA] == listB[skipB]

// 进阶：你能否设计一个时间复杂度 O(m + n) 、仅用 O(1) 内存的解决方案？

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
  ListNode* getIntersectionNode(ListNode* headA, ListNode* headB) {
    // 一个比较简单的思路是分别遍历两个链表，求出两个链表的长度
    // 然后将两个链表在尾部对齐，指针同时向后遍历，知道指针相等（注意到，这样也可以包含nullptr），就返回
    // 咱们先写这个简单的解
    int lenA = 0;
    for (ListNode* work = headA; work; work = work->next) {
      lenA++;
    }

    int lenB = 0;
    for (ListNode* work = headB; work; work = work->next) {
      lenB++;
    }

    // 找出两者中的最大者
    if (lenA > lenB) {
      // A 更长
      // headA 需要前进 lenB - lenA 步
      for (int i = 0; i < lenA - lenB; i++) {
        headA = headA->next;
      }
    } else {
      // B 更长
      // headB 需要前进
      for (int i = 0; i < lenB - lenA; i++) {
        headB = headB->next;
      }
    }

    // headA和headB 齐头并进
    while (headA != headB) {
      headA = headA->next;
      headB = headB->next;
    }

    return headA;
  }

  // 还有一种更好的解法，而且这个思路挺有意思的
  // 就是把两个链表给串接起来
  // 这样两条链表的长度就是一样的了
  // 如果两者有交点，那么一定会在最后一段重复！
  // 我记得我之前也做过一些题目，有类似的思想，就是将一个序列重复一次
  // 往往问题就迎刃而解了
  ListNode* getIntersectionNode1(ListNode* headA, ListNode* headB) {
    ListNode* p1 = headA;
    ListNode* p2 = headB;

    ListNode* headAA = headA;
    ListNode* headBB = headB;

    // 这样写，需要处理多次切换的逻辑
    // 有一种写法不需要处理这样的逻辑
    while (p1 != p2) {
      // 不对！这个切换只能发生一次
      if (p1->next) {
        p1 = p1->next;
      } else {
        p1 = headBB;
        headBB = nullptr;
      }

      if (p2->next) {
        p2 = p2->next;
      } else {
        p2 = headAA;
        headAA = nullptr;
      }
    }
    return p1;
  }

  // 为什么这样是对的呢？
  // 因为两个链表在串起来之后一定是等长的
  // 那么如果两个链表没有任何的交点，我们防止的就是在这种情况下无限交换导致的死循环
  // 在某一个时刻，p1 和
  // p2一定都遍历完了串起来的链表，所以他们都是空指针，所以就退出了循环
  ListNode* getIntersectionNode2(ListNode* headA, ListNode* headB) {
    ListNode* p1 = headA;
    ListNode* p2 = headB;

    while (p1 != p2) {
      p1 = p1 ? p1->next : headB;
      p2 = p2 ? p2->next : headA;
    }

    return p1;
  }
};

TEST_CASE("Example 1: Lists intersect at node with value 8") {
  Solution s;

  // Create the shared part [8,4,5]
  ListNode* intersection = new ListNode(8, new ListNode(4, new ListNode(5)));

  // Create list A: [4,1] + shared part
  ListNode* headA = new ListNode(4, new ListNode(1, intersection));

  // Create list B: [5,6,1] + shared part
  ListNode* headB =
      new ListNode(5, new ListNode(6, new ListNode(1, intersection)));

  ListNode* result = s.getIntersectionNode(headA, headB);

  CHECK(result == intersection);
  CHECK(result->val == 8);

  // Note: Memory cleanup omitted for brevity
}

TEST_CASE("Example 2: Lists intersect at node with value 2") {
  Solution s;

  // Create the shared part [2,4]
  ListNode* intersection = new ListNode(2, new ListNode(4));

  // Create list A: [1,9,1] + shared part
  ListNode* headA =
      new ListNode(1, new ListNode(9, new ListNode(1, intersection)));

  // Create list B: [3] + shared part
  ListNode* headB = new ListNode(3, intersection);

  ListNode* result = s.getIntersectionNode(headA, headB);

  CHECK(result == intersection);
  CHECK(result->val == 2);

  // Note: Memory cleanup omitted for brevity
}

TEST_CASE("Example 3: Lists do not intersect") {
  Solution s;

  // Create list A: [2,6,4]
  ListNode* headA = new ListNode(2, new ListNode(6, new ListNode(4)));

  // Create list B: [1,5]
  ListNode* headB = new ListNode(1, new ListNode(5));

  ListNode* result = s.getIntersectionNode(headA, headB);

  CHECK(result == nullptr);

  // Note: Memory cleanup omitted for brevity
}
