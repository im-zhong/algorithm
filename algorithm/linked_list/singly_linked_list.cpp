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

#include <cstddef>
#include <iterator>

#include <doctest/doctest.h>
#include <forward_list>
#include <iostream>

// 其实，还是需要一些小的utils帮助类的，比如一些随机数的生成，这有助于测试
// 但是这并不妨碍我们每个cpp都是一个doctest的可执行文件的原则

class SinglyLinkedListNode {

public:
  SinglyLinkedListNode(int value, SinglyLinkedListNode* next)
      : next{next}, value{value} {};
  explicit SinglyLinkedListNode(int value) : value{value} {};
  // ban copy
  SinglyLinkedListNode(const SinglyLinkedListNode&) = delete;
  SinglyLinkedListNode& operator=(const SinglyLinkedListNode&) = delete;
  // move
  SinglyLinkedListNode(SinglyLinkedListNode&&) = default;
  SinglyLinkedListNode& operator=(SinglyLinkedListNode&&) = default;
  // destructor
  ~SinglyLinkedListNode() = default;

  // 这些操作只有在链表是循环链表的时候才是对的！
  // 否则需要对nullptr，链表尾部进行特判！
  void InsertAfter(SinglyLinkedListNode* node) {
    // Insert the node after this node
    node->next = this->next;
    this->next = node;
  }

  void EraseAfter() { this->next = this->next->next; }

  [[nodiscard]] const int& GetValue() const { return this->value; }
  int& GetValue() { return this->value; }

  void SetValue(int value) { this->value = value; }

  [[nodiscard]] SinglyLinkedListNode* GetNext() const { return this->next; }

private:
  SinglyLinkedListNode* next{nullptr};
  int value{0};
};

// 循环链表（Circular Linked
// List）相较于常规链表（尾节点指向nullptr）具有以下优点：

// 无需检查空指针：遍历时不会遇到空指针，简化了遍历算法，不需要特殊的终止条件检查

// 从任意节点可以访问整个链表：即使只有一个节点的引用，也可以访问整个链表中的所有节点

// 尾部操作更高效：单向循环链表如果保存尾节点引用，可以O(1)时间实现尾部插入操作（普通单链表需要O(n)时间遍历到尾部）

// 简化循环操作：非常适合需要循环处理的场景，如轮询调度算法、约瑟夫环问题等

// 旋转操作高效：旋转链表只需要移动头指针，无需重新链接节点

// 无需对尾节点特殊处理：所有节点处理方式一致，没有"末尾"的特殊情况

// 适合表示循环结构：更自然地表示循环缓冲区、轮盘菜单等数据结构

// 循环链表在需要反复遍历或循环访问元素的应用中特别有用，如操作系统中的进程调度、多媒体播放循环等场景。

// 我觉得循环链表和拥有一个额外的头节点的作用，很大一部分都是简化编程。这一点就足够了。

class SinglyLinkedList {

public:
  // 最重要的是实现一个遍历的方法
  // 最简单的就是实现一个迭代器
  // 刚好我不太会写迭代器，用这些算法来实践一下

  // 容器就是返回一个begin和一个end，

  struct iterator {
  public:
    // for iterator trait?
    //     在C++的迭代器设计中，开头定义这些类型是为了满足标准库对迭代器概念的要求。这些类型定义是STL迭代器特征（iterator
    //     traits）的一部分，具有以下作用：

    // iterator_category：

    // 指定迭代器的类别（前向、双向、随机访问等）
    // 允许算法根据迭代器的能力选择最优实现
    // forward_iterator_tag表示这是一个只能向前移动的迭代器
    // value_type：

    // 定义迭代器所指向的值的类型
    // 算法需要知道它们处理的元素类型
    // difference_type：

    // 用于表示两个迭代器之间的距离
    // 通常是有符号整数类型，如std::distance()的返回类型
    // pointer：

    // 迭代器的operator->返回的类型
    // 通常是指向value_type的指针
    // reference：

    // 迭代器的operator*返回的类型
    // 通常是对value_type的引用
    // 这些类型定义使你的自定义迭代器与STL算法兼容，让泛型编程成为可能。标准库算法通过std::iterator_traits提取这些类型信息，从而能够正确地处理不同类型的迭代器。
    using iterator_category = std::forward_iterator_tag;
    using value_type = int;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type&;

    // 那这样的话，为了实现循环链表，迭代器必须拿着两个指针
    // 一个是当前节点，一个是头节点
    iterator(SinglyLinkedListNode* head, SinglyLinkedListNode* node)
        : head{head}, current_node{node} {}

    // cause this is a iterator, not a const_iterator, so all the method should
    // be mutable Deference operator, non const
    reference operator*() { return current_node->GetValue(); }

    // Member access operator, non const
    pointer operator->() { return &(current_node->GetValue()); }

    // ++iter
    iterator& operator++() {
      this->current_node = current_node->GetNext();
      return *this;
    }

    // iter++
    // 这里和前置的很大的不同 就是这里返回的值 而不是引用
    iterator operator++(int) {
      // copy the current iter
      iterator tmp = *this;
      this->current_node = current_node->GetNext();
      return tmp;
    }

    // comparision, for loop with iter != end
    bool operator==(const iterator& other) const {
      return this->current_node == other.current_node;
    }

    // 循环链表的优点在这里就体现出来了，我们不需要对尾部节点进行特判，因为end其实就是头节点
    // 否则我们需要对nullptr进行特判
    // bool operator!=(const iterator& other) const {
    //   return this->current_node != other.current_node;
    // }

  private:
    // TODO(zhangzhong)
    // 不对，他不需要一个额外的head指针，我们就只保存一个当前节点就行了
    // 让end{head}就行了！这样就完美了
    // 每个迭代器就只有一个指针，zero cost abstraction !
    SinglyLinkedListNode* head{nullptr};
    SinglyLinkedListNode* current_node{nullptr};

    friend SinglyLinkedList;
  };

  using value_type = iterator::value_type;

  iterator begin() { return iterator{&this->head, this->head.GetNext()}; }
  iterator end() { return iterator{&this->head, &this->head}; }

  [[nodiscard]] size_t size() const { return this->_size; }
  [[nodiscard]] bool empty() const { return this->size() == 0; }

  // TODO(zhangzhong)
  // 我是否可以加入一些自己的巧思
  // 我使用expected来增强这个链表的类型
  // 比如 front 这个函数， 如果链表是空的时候，这个函数不会返回一个值
  // 但是当链表非空的时候，是可以的
  // 这个实现是错误的！当链表是空的时候
  [[nodiscard]] int front() const { return this->head.GetNext()->GetValue(); }

  void clear() {
    // 比较简单的实现方法是erase after head
    // 然后循环调用即可
    while (this->empty()) {
      this->pop_front();
    }
  }

  void insert_after(iterator pos, value_type value) {
    pos.current_node->InsertAfter(new SinglyLinkedListNode{value});
    ++this->_size;
  }

  // 1) Iterator to the element following the erased one, or end() if no such
  // element exists.
  iterator erase_after(iterator pos) {
    if (this->empty()) {
      return this->end();
    }

    // 这里必须正确的处理内存
    // 我们需要引入内存检查的工具！

    // 我们先拿到需要erase的节点
    // 并且这个节点不能是head
    SinglyLinkedListNode* next = pos.current_node->GetNext();

    pos.current_node->EraseAfter();
    --this->_size;

    // free memory
    if (next != &this->head) {
      delete next;
    }

    return ++pos;
  }

  // 循环链表！！！
  void push_front(value_type value) { this->insert_after(this->end(), value); }
  void pop_front() { this->erase_after(this->end()); }

  // TODO(zhangzhong)
  // 还有很多的链表操作
  // rotate, merge, cut, splice, reverse, sort, unique, find, remove

private:
  SinglyLinkedListNode head;
  size_t _size{0};
};

TEST_CASE("testing singly linked list") {
  std::forward_list<int> list;

  list.push_front(1);

  // all the container has an iterator type
  // which could ++ or *
  // so in my impl, I should impl an iterator type and overload the proper op
  auto it = list.begin();
  std::cout << *it << "\n";
}
