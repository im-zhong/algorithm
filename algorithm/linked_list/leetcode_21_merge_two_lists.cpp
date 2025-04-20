// 2025/4/20
// zhangzhong

// 21. 合并两个有序链表
// 将两个升序链表合并为一个新的 升序
// 链表并返回。新链表是通过拼接给定的两个链表的所有节点组成的。

// 示例 1：
// 输入：l1 = [1,2,4], l2 = [1,3,4]
// 输出：[1,1,2,3,4,4]

// 示例 2：
// 输入：l1 = [], l2 = []
// 输出：[]

// 示例 3：
// 输入：l1 = [], l2 = [0]
// 输出：[0]

// 提示：
// - 两个链表的节点数目范围是 [0, 50]
// - -100 <= Node.val <= 100
// - l1 和 l2 均按 非递减顺序 排列

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

struct ListNode {
  int val;
  ListNode* next;
  ListNode() : val(0), next(nullptr) {}
  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode* next) : val(x), next(next) {}
};
// 每个题目都用一个单独的文件夹 太麻烦了
// 对于leetcode而言，几个test case足以
// 但是对于ACM的题目，它需要读取一个输入文件，并且和输出文件做比较，这个时候就必须使用一个额外的文件夹了
// 那或许确实可以结合这来，在合适的地方使用合适的方法
// 就是cmake写起来就很麻烦了，怎么做测试也是个问题啊
class Solution {
public:
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
};

// Helper function to create a linked list from a vector
ListNode* createLinkedList(const std::vector<int>& values) {
  ListNode dummy(0);
  ListNode* tail = &dummy;

  for (int val : values) {
    tail->next = new ListNode(val);
    tail = tail->next;
  }

  return dummy.next;
}

// Helper function to convert linked list to vector for easy comparison
std::vector<int> linkedListToVector(ListNode* head) {
  std::vector<int> result;
  while (head) {
    result.push_back(head->val);
    head = head->next;
  }
  return result;
}

// Helper function to free memory used by linked list
void deleteLinkedList(ListNode* head) {
  while (head) {
    ListNode* temp = head;
    head = head->next;
    delete temp;
  }
}

TEST_CASE("Example 1: [1,2,4] and [1,3,4] -> [1,1,2,3,4,4]") {
  Solution solution;
  std::vector<int> values1 = {1, 2, 4};
  std::vector<int> values2 = {1, 3, 4};
  std::vector<int> expected = {1, 1, 2, 3, 4, 4};

  ListNode* list1 = createLinkedList(values1);
  ListNode* list2 = createLinkedList(values2);

  ListNode* merged = solution.mergeTwoLists(list1, list2);
  std::vector<int> result = linkedListToVector(merged);

  CHECK(result == expected);

  deleteLinkedList(merged);
}

TEST_CASE("Example 2: [] and [] -> []") {
  Solution solution;
  std::vector<int> values1 = {};
  std::vector<int> values2 = {};
  std::vector<int> expected = {};

  ListNode* list1 = createLinkedList(values1);
  ListNode* list2 = createLinkedList(values2);

  ListNode* merged = solution.mergeTwoLists(list1, list2);
  std::vector<int> result = linkedListToVector(merged);

  CHECK(result == expected);

  deleteLinkedList(merged);
}

TEST_CASE("Example 3: [] and [0] -> [0]") {
  Solution solution;
  std::vector<int> values1 = {};
  std::vector<int> values2 = {0};
  std::vector<int> expected = {0};

  ListNode* list1 = createLinkedList(values1);
  ListNode* list2 = createLinkedList(values2);

  ListNode* merged = solution.mergeTwoLists(list1, list2);
  std::vector<int> result = linkedListToVector(merged);

  CHECK(result == expected);

  deleteLinkedList(merged);
}

TEST_CASE("Additional Test: Non-overlapping lists") {
  Solution solution;
  std::vector<int> values1 = {1, 3, 5};
  std::vector<int> values2 = {2, 4, 6};
  std::vector<int> expected = {1, 2, 3, 4, 5, 6};

  ListNode* list1 = createLinkedList(values1);
  ListNode* list2 = createLinkedList(values2);

  ListNode* merged = solution.mergeTwoLists(list1, list2);
  std::vector<int> result = linkedListToVector(merged);

  CHECK(result == expected);

  deleteLinkedList(merged);
}
