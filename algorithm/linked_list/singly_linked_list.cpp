// 2021/9/5
// zhangzhong
// slist.h

// singly linked list
// doubly linked list

// 要实现一个节点类型
// 为了简单，我们的算法库暂时不会讨论模板技术，当然之后会讨论
// 所以这里就简单的放一个int在里面就行了，

// #include <doctest/doctest.h>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

class SinglyLinkedListNode {

public:
  SinglyLinkedListNode() = default;
  // ban copy
  SinglyLinkedListNode(const SinglyLinkedListNode&) = delete;
  SinglyLinkedListNode& operator=(const SinglyLinkedListNode&) = delete;
  // move
  SinglyLinkedListNode(SinglyLinkedListNode&&) = default;
  SinglyLinkedListNode& operator=(SinglyLinkedListNode&&) = default;
  // destructor
  ~SinglyLinkedListNode() = default;

  void InsertAfter(SinglyLinkedListNode* node) {
    // Insert the node after this node
    node->next = this->next;
    this->next = node;
  }

  void EraseAfter() { this->next = this->next->next; }

  [[nodiscard]] int GetValue() const { return this->value; }
  void SetValue(int value) { this->value = value; }

  [[nodiscard]] SinglyLinkedListNode* GetNext() const { return this->next; }

private:
  SinglyLinkedListNode* next{nullptr};
  int value{0};
};

class SinglyLinkedList {

public:
  // 最重要的是实现一个遍历的方法
  // 最简单的就是实现一个迭代器
  // 刚好我不太会写迭代器，用这些算法来实践一下

  // 容器就是返回一个begin和一个end，

  struct iterator {};

private:
  SinglyLinkedListNode head;
};

TEST_CASE("testing singly linked list") {}
