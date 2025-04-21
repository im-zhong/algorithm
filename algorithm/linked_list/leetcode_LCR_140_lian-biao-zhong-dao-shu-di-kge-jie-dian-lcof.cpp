// 2024/4/21
// zhangzhong
// https://leetcode.cn/problems/lian-biao-zhong-dao-shu-di-kge-jie-dian-lcof/description/

// LCR 140. 训练计划 2

// 给定一个头节点为 head
// 的链表用于记录一系列核心肌群训练项目编号，请查找并返回倒数第 cnt
// 个训练项目编号。

// 示例 1：
// 输入：head = [2,4,7,8], cnt = 1
// 输出：8

// 提示：
// 1 <= head.length <= 100
// 0 <= head[i] <= 100
// 1 <= cnt <= head.length

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
  ListNode* trainingPlan(ListNode* head, int cnt) {
    // 和leetcode_19 对比，这里因为是访问倒数第k个节点
    // 就没有使用dummy head
    ListNode* fast = head;
    ListNode* slow = head;

    // fast move cnt steps
    for (int i = 0; i < cnt; i++) {
      fast = fast->next;
    }

    while (fast) {
      fast = fast->next;
      slow = slow->next;
    }

    return slow;
  }
};
