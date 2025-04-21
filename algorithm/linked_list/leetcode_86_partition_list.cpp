// 2025/4/21
// zhangzhong
// https://leetcode.cn/problems/partition-list/description/

// 86. 分隔链表

// 给你一个链表的头节点 head 和一个特定值 x ，请你对链表进行分隔，使得所有 小于
// x 的节点都出现在 大于或等于 x 的节点之前。
// 你应当 保留 两个分区中每个节点的初始相对位置。

// 示例 1：
// 输入：head = [1,4,3,2,5,2], x = 3
// 输出：[1,2,2,4,3,5]

// 示例 2：
// 输入：head = [2,1], x = 2
// 输出：[1,2]

// 提示：
// 链表中节点的数目在范围 [0, 200] 内
// -100 <= Node.val <= 100
// -200 <= x <= 200

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
  ListNode* partition(ListNode* head, int x) {

    // 非常简单，就弄两个dummy head
    // 然后所有小于x的元素就插入到这链表后面
    // 所有大于等于x的元素插入到第二个dummy head后面
    // 然后把这两个链表链接起来即可

    ListNode left_head{0, nullptr};
    ListNode* left_list{&left_head};

    ListNode right_head{0, nullptr};
    ListNode* right_list{&right_head};

    ListNode* work = head;
    while (work != nullptr) {
      if (work->val < x) {
        // insert into left
        left_list->next = work;
        // update ptrs
        left_list = left_list->next;

      } else {
        // insert into right
        right_list->next = work;
        right_list = right_list->next;
      }
      work = work->next;
    }

    // concat left list and right list
    left_list->next = right_head.next;
    // make this list end
    right_list->next = nullptr;

    return left_head.next;
  }
};

// Helper functions
ListNode* createList(const std::vector<int>& values) {
  if (values.empty()) {
    return nullptr;
  }

  ListNode* head = new ListNode(values[0]);
  ListNode* current = head;

  for (size_t i = 1; i < values.size(); ++i) {
    current->next = new ListNode(values[i]);
    current = current->next;
  }

  return head;
}

void freeList(ListNode* head) {
  while (head) {
    ListNode* temp = head;
    head = head->next;
    delete temp;
  }
}

bool compareList(ListNode* head, const std::vector<int>& expected) {
  size_t index = 0;
  ListNode* current = head;

  while (current && index < expected.size()) {
    if (current->val != expected[index]) {
      return false;
    }
    current = current->next;
    index++;
  }

  return (current == nullptr && index == expected.size());
}

TEST_CASE("Partition List") {
  Solution solution;

  // Test Case 1
  std::vector<int> input1 = {1, 4, 3, 2, 5, 2};
  std::vector<int> expected1 = {1, 2, 2, 4, 3, 5};
  ListNode* head1 = createList(input1);
  ListNode* result1 = solution.partition(head1, 3);
  CHECK(compareList(result1, expected1));
  freeList(result1);

  // Test Case 2
  std::vector<int> input2 = {2, 1};
  std::vector<int> expected2 = {1, 2};
  ListNode* head2 = createList(input2);
  ListNode* result2 = solution.partition(head2, 2);
  CHECK(compareList(result2, expected2));
  freeList(result2);

  // Test Case 3 - Empty list
  CHECK(solution.partition(nullptr, 0) == nullptr);

  // Test Case 4 - Single node
  ListNode* head4 = new ListNode(5);
  ListNode* result4 = solution.partition(head4, 10);
  CHECK(compareList(result4, {5}));
  freeList(result4);

  // Test Case 5 - All nodes less than x
  std::vector<int> input5 = {1, 2, 3};
  std::vector<int> expected5 = {1, 2, 3};
  ListNode* head5 = createList(input5);
  ListNode* result5 = solution.partition(head5, 4);
  CHECK(compareList(result5, expected5));
  freeList(result5);

  // Test Case 6 - All nodes greater than or equal to x
  std::vector<int> input6 = {3, 4, 5};
  std::vector<int> expected6 = {3, 4, 5};
  ListNode* head6 = createList(input6);
  ListNode* result6 = solution.partition(head6, 3);
  CHECK(compareList(result6, expected6));
  freeList(result6);
}
