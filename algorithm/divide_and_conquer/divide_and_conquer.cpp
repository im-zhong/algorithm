// 2025/4/25
// zhangzhong
// https://labuladong.online/algo/essential-technique/divide-and-conquer/

// 分治这一部分没有什么题要刷，但是作者对分治的理解确实在我之上，或者说我本来并没有什么理解

// 分治的思想非常普遍，dp就是分治的思想，就是分解问题，

// 一个非常简单的例子，递归的求解fib

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

int fib(int n) {

  // 递归结束条件
  if (n == 0 || n == 1) {
    return n;
  }

  return fib(n - 1) + fib(n - 2);
}

// 作者给出了一个例子，就是计算n个数的和
// 当然这个题目真正最简单的做法就是直接迭代计算
// 但是这里只是作为演示
// 无效的分治
// O(N)
int get_sum_slow(const std::vector<int>& nums, int begin) {
  // 还要有递归结束条件
  if (begin >= nums.size()) {
    return 0;
  }
  return nums[begin] + get_sum_slow(nums, begin + 1);
}

// 有效的分治，就是二分原来的问题，在合并两个问题的解
// 最常见的：归并排序
// 采用二分的策略，类似归并排序的方式，将一个数组从中间切开，分别求左侧和右侧的和
// 然后把两者的和加起来
// 从算法分析的角度来看是O(logN)
// 其实这里的时间复杂度更像是递归树的深度
int get_sum_fast(const std::vector<int>& nums, int begin, int end) {
  // 二分的写法比较好的是左开右闭的

  // 递归结束条件
  // 递归的结束条件
  // 是要么有一个，要么没有
  if (begin == end) {
    return 0;
  }
  if (end - begin == 1) {
    return nums[begin];
  }

  // 找中点
  int middle = begin + (end - begin) / 2;

  int left_sum = get_sum_fast(nums, begin, middle);
  int right_sum = get_sum_fast(nums, middle, end);

  return left_sum + right_sum;
}

TEST_CASE("Testing sum functions") {
  // Test with empty vector
  std::vector<int> empty = {};
  CHECK(get_sum_slow(empty, 0) == 0);
  CHECK(get_sum_fast(empty, 0, 0) == 0);

  // Test with single element
  std::vector<int> single = {42};
  CHECK(get_sum_slow(single, 0) == 42);
  CHECK(get_sum_fast(single, 0, 1) == 42);

  // Test with multiple elements
  std::vector<int> multi = {1, 2, 3, 4, 5};
  CHECK(get_sum_slow(multi, 0) == 15);
  CHECK(get_sum_fast(multi, 0, 5) == 15);

  // Test with negative numbers
  std::vector<int> negative = {-1, -2, -3, -4};
  CHECK(get_sum_slow(negative, 0) == -10);
  CHECK(get_sum_fast(negative, 0, 4) == -10);

  // Test with mixed positive and negative
  std::vector<int> mixed = {-5, 10, -15, 20};
  CHECK(get_sum_slow(mixed, 0) == 10);
  CHECK(get_sum_fast(mixed, 0, 4) == 10);

  // Test partial sum with slow version
  CHECK(get_sum_slow(multi, 2) == 12); // 3+4+5

  // Test partial sum with fast version
  CHECK(get_sum_fast(multi, 2, 5) == 12); // 3+4+5
}

// 同样，对于K个有序链表的合并
// 也可以采用类似的做法
// 我们先实现两个有序链表合并的方法
// 然后将链表二分，直到可以合并
// 然后将左链表和右链表两两合并即可

struct ListNode {
  int val;
  ListNode* next;
  ListNode() : val(0), next(nullptr) {}
  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode* next) : val(x), next(next) {}
};

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
ListNode* mergeKLists(std::vector<ListNode*>& lists) {

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
