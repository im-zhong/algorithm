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
#include <utility>

#include <cassert>
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

  // ! 不对！不需要！
  void EraseAfter() {
    // 我们需要返回被删除的节点的下一个节点
    // SinglyLinkedListNode* deleted_node = this->next;

    this->next = this->next->next;
    // return this->next;
  }

  [[nodiscard]] const int& GetValue() const { return this->value; }
  int& GetValue() { return this->value; }

  void SetNext(SinglyLinkedListNode* next) { this->next = next; }
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

    bool operator!=(const iterator& other) const { return !(*this == other); }

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

  // 单链表可以做rotate吗？
  // 可以，但是应该不可能做到O(1),
  // 因为我们为了找到尾元素，必须遍历一次，除非维护一个额外的尾指针，这会增加代码的复杂度，tradeoff

  // 循环单链表确实可以在O(1)时间内实现**特定方向**的旋转操作，具体取决于旋转的定义和实现方式。

  // ### 关键分析：
  // 1. **结构设计**：
  //    若维护一个指针（如`current`或`tail`），指向链表的某个节点作为逻辑上的“当前头节点”，则可以通过调整该指针的位置实现旋转。例如：
  //    -
  //    **左旋（将头节点移动到尾部）**：若维护尾指针`tail`，则`tail->next`为头节点。左旋时，只需将`tail`移动到`tail->next`（即原头节点），新的头节点变为`tail->next->next`（原第二个节点），时间复杂度为O(1)。
  //    -
  //    **逻辑旋转**：若维护一个`current`指针指向当前头节点，每次旋转只需将其后移（`current
  //    =
  //    current->next`），时间复杂度为O(1)。此时，链表顺序的逻辑视图发生改变，但实际节点间的链接不变。

  // 2. **向右旋转的挑战**：
  //    向右旋转（将尾节点移动到头部）需要访问尾节点的前驱节点，这在单链表中无法直接实现，通常需O(n)时间遍历。但若通过调整指针维护方式，仅改变逻辑头节点而不实际修改链表结构，则右旋也可在O(1)时间内完成。

  // ### 结论：
  // 循环单链表可以通过**维护指向逻辑头节点的指针**并仅调整该指针的位置，在O(1)时间内实现**左旋或逻辑旋转**。若允许这种设计，则答案是肯定的。

  // **答案：**
  // 是的，循环单链表可以通过调整逻辑头指针的位置在O(1)时间内实现旋转操作。例如，维护一个指针指向当前头节点，每次旋转将其移动到下一个节点，时间复杂度为O(1)。
  // deepseek 给出的答案我认为并不合适，因为我们需要一个 fake head
  // 来保证代码的简洁，使用一个current head指针，会让编码的复杂度增加。

  //  反观双链表是可以做到的rotate_left
  // rotate_right 两个都是O(1)的

  // merge 可以做到吗？merge都是有顺序才行哦
  // merge太简单了 单链表和双链表都是可以的，因为只需要 front pop_front
  // 这样的操作

  void merge(SinglyLinkedList& other) {
    // The function does nothing if other refers to the same object as *this.
    // Otherwise, merges other into *this. Both lists should be sorted. No
    // elements are copied, and the container other becomes empty after the
    // merge.

    // 这个只有函数参数是引用的时候才行，那么为了支持移动操作
    // 我们还得实现一个参数为&& 的函数，不过算了，这个不是STL的严肃复刻
    if (this == &other) {
      return;
    }

    size_t const merged_size = this->size() + other.size();
    if (merged_size == 0) {
      return;
    }

    // 不行，使用这种方式合并出来的是需要额外申请节点的
    // 我们的insert实现的是插入值 所以需要额外的空间
    // 所以这里想要好的实现，不能用迭代器
    // 需要直接使用节点的指针来实现了

    // 直接合并到原链表的实现起来太麻烦了，而且很容易出错
    // 还是用一个新的链表比较合适
    // 然后合并好之后，可以用这个新的链表代替原来的链表？
    // 这个move操作容易实现吗？我们在合并结束之后，是可以拿到尾指针的
    // 那么这个操作就是O(1)的

    // 不行 这两个必须结合起来
    // 我们必须声明一个新的head
    // SinglyLinkedListNode new_head{0};
    // SinglyLinkedListNode* insert_after = &new_head;

    // 这里也不用迭代器，直接用指针
    // auto this_iter = this->begin();
    // auto other_iter = other.begin();
    // 我们这里始终拿着head指针才对！
    // 删除直接pop front 就行了！
    SinglyLinkedListNode* this_head = &this->head;
    SinglyLinkedListNode* other_head = &other.head;

    // 如果我们把合并链表的操作提取出来的话
    // 在sort里面就不需要修改两次节点了
    // 这样就真的完美了呀 卧槽 哈哈哈
    auto [new_begin, new_tail] = _merge(this_head, other_head);

    // while (this_head->GetNext() != &this->head &&
    //        other_head->GetNext() != &other.head) {
    //   // 这里需要注意的是，我们的链表是循环链表
    //   // 所以我们需要对nullptr进行特判
    //   // 但是我们在实现的时候，已经把head的next指向了head
    //   // 所以我们只需要判断next是否等于head就行了

    //   // 这里的比较是有问题的，因为我们没有考虑到循环链表的情况
    //   // 这里应该是比较两个节点的值
    //   if (this_head->GetNext()->GetValue() <
    //       other_head->GetNext()->GetValue()) {

    //     // insert this_iter->GetNext after new head
    //     // 得先从原链表中删除这个节点，否则会导致链表断开的吧
    //     // ！！！ 我明白了，所以erase 需要返回删除节点的下一个节点！
    //     // 这里就需要这个功能！
    //     // SinglyLinkedListNode* tmp = this_head->GetNext();
    //     // // pop front, no release memory
    //     // this_head->EraseAfter();

    //     // insert_iter->InsertAfter(tmp);
    //     // // 反而是这个insert_iter是需要更新的！
    //     // insert_iter = insert_iter->GetNext();
    //     insert_after = _pop_front_to(this_head, insert_after);
    //   } else {
    //     // insert other_iter->GetNext after new head
    //     // 这里的实现是错误的，因为我们没有考虑到循环链表的情况
    //     // 这里应该是比较两个节点的值
    //     // SinglyLinkedListNode* tmp = other_head->GetNext();
    //     // // pop front, no release memory
    //     // other_head->EraseAfter();

    //     // insert_iter->InsertAfter(tmp);
    //     // // 反而是这个insert_iter是需要更新的！
    //     // insert_iter = insert_iter->GetNext();
    //     insert_after = _pop_front_to(other_head, insert_after);
    //   }
    // }

    // 找到那个还有元素的链表
    // SinglyLinkedListNode* remaining_head =
    //     this_head->GetNext() != &this->head ? this_head : other_head;

    // // 把remaing iter的元素挨个pop front出来，插入到insert_iter的后面即可
    // // 这里这个head的指针就是他自己啊 要把这些iter的名字改成head
    // while (remaining_head->GetNext() != remaining_head) {
    //   // TODO(zhangzhong): 这段代码在这个函数里面重复了三次了
    //   // 可以抽象出来，就是从head中erase after，然后插入到某个节点后面
    //   // 然后返回插入的节点的指针就行了
    //   // SinglyLinkedListNode* tmp = remaining_head->GetNext();
    //   // // 一定要先断开链表 再插入 否则链表会断掉 无法遍历后续的节点
    //   // // 甚至可能出现死循环的bug
    //   // remaining_head->EraseAfter();
    //   // insert_iter->InsertAfter(tmp);
    //   // // 更新insert
    //   // insert_iter = insert_iter->GetNext();
    //   insert_after = _pop_front_to(remaining_head, insert_after);
    // }

    // 最终insert_iter指向什么？
    // 如果两个链表都是空的，那么insert_iter指向new_head
    // 否则insert_iter指向new_head的最后一个节点
    // 现在我们需要将
    // case 2
    // 其实这两句话 也可以写成一个函数 因为有两次重复了
    // 不过咱们等三次重复在提取吧
    this_head->SetNext(new_begin);
    new_tail->SetNext(this_head);
    // 分析一下是否适用于case 1
    // 不行 不适用于 第一句代码就把 this_head -> new_head 给串起来了
    // 所以这个函数应该在最开始检查一下，两个链表都不能是空的

    // 更新size
    this->_size = merged_size;
  }

  // cut 单链表也是可以做到的，对于双链表非常简单
  // 因为给定任何一个节点，可以在O(1)的时间内找到前驱和后继，然后把前驱的next指向head即可
  // 然后创建一个新的链表，把后半段链表的节点都插入到新的链表中
  // 不对，还是要看 cut函数指定的节点，应该放在前面还是后面
  // 对于双链表 cut_before cut_after 都可以做
  // 但是对于单链表 不管是什么cut
  // 都没有办法在O(1)的时间内完成，还是因为没有办法找到tail

  // find  就是遍历了 这个很简单

  // sort 需要做自底向上的归并排序
  // 需要实现merge

  // 如果是数组的自底向上的排序要如何实现
  // void sort_array() {
  //   // should be in place

  //   int array[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  //   int size = sizeof(array) / sizeof(int);
  //   for (int step = 1; step < size; step *= 2) {
  //     // 首先是两两相邻的自数组做inplace merge
  //     // 假设我们有一个函数叫做 inplace_merge(int*, left, mid, right)
  //     for (int i = 0; i < size; i += 2 * step) {
  //       int low = i;
  //       int mid = low + step - 1;
  //       int high = mid + step;

  //       // 注意到 mid 和 high都有可能越界
  //       // 这里需要判断一下
  //       if (mid >= size) {
  //         // 这说明最后凑不成一对 就不用管了
  //         break;
  //       }

  //       if (high >= size) {
  //         // 这说明最后一对的右边的区间不是step个 修正到size即可
  //         high = size - 1;
  //       }

  //       inplace_merge(array, low, mid, high);
  //     }
  //   }
  // }

  SinglyLinkedListNode* advance(SinglyLinkedListNode* node, int step) {
    // 这里的实现是错误的，因为我们没有考虑到循环链表的情况
    // 这里应该是比较两个节点的值

    while (step > 0 && node != &this->head) {
      node = node->GetNext();
      --step;
    }
    return node;
  }

  void sort() {
    if (this->empty()) {
      return;
    }

    // 自底向上的排序
    for (int step = 1; step < this->size(); step *= 2) {
      // 在最开始的时候，每个链表都只有一个元素，那么他就是有序的
      // 我们就两两合并就行了
      // 但是合并的返回值是什么呢？
      // 这个要怎么实现呢？

      // 分割链表为多个长度为 step 的子链表
      // 两两合并相邻子链表
      // 若所有子链表已合并为单个链表，退出循环

      // 需要先分割链表，我们无法像array那样 直接提供一个span
      // 不过链表可以提供一个range啊，就是提供一个begin和end

      // 应该不能通过迭代器来实现，太不灵活了
      // 还是直接操作指针实现比较方便

      // 不对，这里和上面的merge函数不同，这里将要merge的两个链表有绝对非空的保证
      // 所以这个时候那个head就没有必要了
      // 所以反而这里需要使用迭代器来实现
      // [begin, end)
      // 我们从一个节点开始 advance(step) 就可以拿到end指针
      // 当然我们可也能advance不到，不到就算了，也不重要，刚好模拟上面的
      // 这说明最后一对的右边的区间不是step个 修正到size即可

      SinglyLinkedListNode* head = &this->head;
      // 这里不应该用begin，这里的判断逻辑还需要再想一想
      // SinglyLinkedListNode* begin = this->head.GetNext();
      // 应该是这样的
      while (head->GetNext() != &this->head) {
        // 这里的实现是错误的，因为我们没有考虑到循环链表的情况
        // 这里应该是比较两个节点的值
        // 这里需要判断一下，是否已经到头了
        // 否则就获得下一个区间
        // 这里会自动处理 end

        SinglyLinkedListNode* tail = this->advance(head, step);
        if (tail == &this->head) {
          // 说明已经到头了
          break;
        }

        // 否则就获得下一个区间
        SinglyLinkedListNode* right_head = tail;
        // 这里会自动处理 end
        SinglyLinkedListNode* right_tail = this->advance(right_head, step);

        // 为了实现inplace merge
        // 我们假象这个 [begin, end) 这整段链表是在一个环上 取下了某段
        // 因为我们在merge的时候，最简单的方法是把他们从链表上取下来，插入到一个新的head后面
        //

        // 应该在inplace merge之前，就保存head和begin的位置
        // 因为在merge之后，现在的right_tail的位置可能就不是tail了

        // 但是此时 right_tail可能就是head了
        SinglyLinkedListNode* next_head = &this->head;
        if (right_tail != &this->head) {
          // 说明已经到头了
          // 这里的实现是错误的，因为我们没有考虑到循环链表的情况
          // 这里应该是比较两个节点的值
          next_head = right_tail->GetNext();
        }

        _inplace_merge(head, tail, right_head, right_tail);

        // begin = right_tail->GetNext();
        head = next_head;
      }
    }
  }

  // Removes all consecutive duplicate elements from the container. Only the
  // first element in each group of equal elements is left.
  // 所以实际上回清楚内存事吗
  void unique() {

    if (this->empty()) {
      return;
    }

    // 否则我们一定能拿到第一个元素
    // 而且第一个元素一定不可能重复
    // 所以可以直接特判

    // 非常简单，应该就是保存一个当前的节点指针就行了 work
    // 从head开始

    SinglyLinkedListNode* curr = this->head.GetNext();
    int prev_value = curr->GetValue();

    while (curr->GetNext() != &this->head) {

      // 拿到head 我们需要和当前的值进行对比
      // 如果是相同的 就要去掉 否则就更新prev _value

      // 但是一开始prev_value是不存在的 所以
      // 最好是在最开始特判一下
      if (curr->GetNext()->GetValue() == prev_value) {
        // 这里需要注意的是，我们的链表是循环链表
        // 所以我们需要对nullptr进行特判
        // 但是我们在实现的时候，已经把head的next指向了head
        // 所以我们只需要判断next是否等于head就行了

        // 这里的比较是有问题的，因为我们没有考虑到循环链表的情况
        // 这里应该是比较两个节点的值
        // 不能调这个，因为没有释放内存
        // curr->EraseAfter();
        SinglyLinkedListNode* tmp = curr->GetNext();
        curr->EraseAfter();
        delete tmp;
      } else {
        prev_value = curr->GetNext()->GetValue();
      }

      curr = curr->GetNext();
    }
  }

  void reverse() {
    // reverse 操作也非常简单
    // 而且我昨天睡觉的时候想到了一个不用额外的head的方法
    // 就是用一个指针保存begin
    // 然后把head短接
    // 然后不断的insert after head 即可

    if (this->empty()) {
      return;
    }

    SinglyLinkedListNode* curr = this->head.GetNext();

    // 把head短接
    this->head.SetNext(&this->head);

    while (curr != &this->head) {
      // 先拿到next
      SinglyLinkedListNode* next = curr->GetNext();
      // 然后把curr插入到head后面
      this->head.InsertAfter(curr);
      // update curr
      curr = next;
    }
  }

  iterator find(int value) {
    auto iter = begin();
    for (; iter != end(); iter++) {
      if (*iter == value) {
        break;
      }
    }
    return iter;
  }

  // Removes all elements satisfying specific criteria. Invalidates only the
  // iterators and references to the removed elements.
  void remove(int value) {
    // 显然还是需要一个work指针
    // 或者使用迭代器？
    auto iter = begin();
    while (iter != end()) {
      if (*iter == value) {
        // 原来如此，这个函数回返回删除后的迭代器，刚好满足这里的要求
        iter = erase_after(iter);
      } else {
        ++iter;
      }
    }
  }

  bool contains(int value) { return find(value) != end(); }

  // Moves elements from another forward_list to *this. The elements are
  // inserted after the element pointed to by pos.
  static void splice_after(iterator pos, SinglyLinkedList& other) {
    // 最常见的方式应该是调用 splice_after(end())
    // 也就是把两个链表给拼接起来

    SinglyLinkedListNode* other_head = &other.head;

    // 同样，我们需要从pos里面拿到他的节点指针
    SinglyLinkedListNode* insert_after = pos.current_node;

    // 我们需要挨个把other的元素给pop出来
    while (other_head->GetNext() != &other.head) {
      // 不对，我们这样直接操作的都是迭代器
      // 我们需要拿到指针！
      SinglyLinkedListNode* front = other_head->GetNext();

      // 然后把这个节点插入到pos后面
      insert_after->InsertAfter(front);
      // 然后更新insert_after
      insert_after = front;

      other_head->EraseAfter();
    }
  }

private:
  void _inplace_merge(SinglyLinkedListNode* left_head,
                      SinglyLinkedListNode* left_tail,
                      SinglyLinkedListNode* right_head,
                      SinglyLinkedListNode* right_tail) {

    // 首相，两个链表的长度不一定相同
    // 但是一定不是空的！
    assert(left_head != left_tail);
    assert(right_head != right_tail);
    // 这两个链表应该是衔接的
    assert(left_tail == right_head);

    // 这里比较简单的做法应该是设置两个假的头
    SinglyLinkedListNode new_left_head{0};
    SinglyLinkedListNode new_right_head{0};
    // 在cut之前保存begin
    SinglyLinkedListNode* left_begin = left_head->GetNext();
    SinglyLinkedListNode* right_begin = right_head->GetNext();
    SinglyLinkedListNode* old_end = right_tail->GetNext();

    // right_end 就是这段链表的尾部 需要保留
    // 我还需要这整段链表的头部
    // 需要通过函数参数传入 head

    // 我们先把这整个链表给拆下来，然后记录head和tail的位置
    // 这样就变成了两个独立的链表
    // 就可以调用我们上面写的merge函数了！！
    // 这应该是一个比较合理的实现方法

    // ok 现在把这两段链表个拆下来
    left_head->SetNext(old_end);

    // 那么现在问题来了，我们不用new head，merge能不能正常工作呢？
    // 不对，我们已经把链表给cut下来了
    // 那么实际上这两个正常的head和tail了
    // 所以new head是必须要用的！

    // 然后让这两个假的头作为这两个新的链表的头 指向他们
    new_left_head.SetNext(left_begin);
    // 不行，因为单链表没有prev操作，我们还需要让
    // left_end前面的那个指针指向left_head
    // 不过也可以不传入，咱们这里再advance一下吧
    left_tail->SetNext(&new_left_head);

    new_right_head.SetNext(right_begin);
    right_tail->SetNext(&new_right_head);
    // 草，这里也需要
    // 感觉传入的方式不对，不应该传入begin和end
    // 应该传入 head和tail
    // head就是第一个元素的前一个，tail就是最后一个元素！
    // 这在单链表里面才是对的

    // 然后就可以进行mege了
    // 最后会合并到 new_left_head 上
    // 但是我们需要拿到新的链表的tail
    auto [new_begin, new_tail] = _merge(&new_left_head, &new_right_head);
    // 然后我们吧 new_left_head -> ... -> new_tail 再插入回到原链表的位置即可
    left_head->SetNext(new_begin);
    new_tail->SetNext(old_end);
  }

  std::pair<SinglyLinkedListNode*, SinglyLinkedListNode*>
  _merge(SinglyLinkedListNode* this_head, SinglyLinkedListNode* other_head) {

    SinglyLinkedListNode new_head{0};
    SinglyLinkedListNode* insert_after = &new_head;

    // 这里也不用迭代器，直接用指针
    // auto this_iter = this->begin();
    // auto other_iter = other.begin();
    // 我们这里始终拿着head指针才对！
    // 删除直接pop front 就行了！
    // SinglyLinkedListNode* this_head = &this->head;
    // SinglyLinkedListNode* other_head = &other.head;

    while (this_head->GetNext() != this_head &&
           other_head->GetNext() != other_head) {
      // 这里需要注意的是，我们的链表是循环链表
      // 所以我们需要对nullptr进行特判
      // 但是我们在实现的时候，已经把head的next指向了head
      // 所以我们只需要判断next是否等于head就行了

      // 这里的比较是有问题的，因为我们没有考虑到循环链表的情况
      // 这里应该是比较两个节点的值
      if (this_head->GetNext()->GetValue() <
          other_head->GetNext()->GetValue()) {

        // insert this_iter->GetNext after new head
        // 得先从原链表中删除这个节点，否则会导致链表断开的吧
        // ！！！ 我明白了，所以erase 需要返回删除节点的下一个节点！
        // 这里就需要这个功能！
        // SinglyLinkedListNode* tmp = this_head->GetNext();
        // // pop front, no release memory
        // this_head->EraseAfter();

        // insert_iter->InsertAfter(tmp);
        // // 反而是这个insert_iter是需要更新的！
        // insert_iter = insert_iter->GetNext();
        insert_after = _pop_front_to(this_head, insert_after);
      } else {
        // insert other_iter->GetNext after new head
        // 这里的实现是错误的，因为我们没有考虑到循环链表的情况
        // 这里应该是比较两个节点的值
        // SinglyLinkedListNode* tmp = other_head->GetNext();
        // // pop front, no release memory
        // other_head->EraseAfter();

        // insert_iter->InsertAfter(tmp);
        // // 反而是这个insert_iter是需要更新的！
        // insert_iter = insert_iter->GetNext();
        insert_after = _pop_front_to(other_head, insert_after);
      }
    }

    // 找到那个还有元素的链表
    SinglyLinkedListNode* remaining_head =
        this_head->GetNext() != &this->head ? this_head : other_head;

    // 把remaing iter的元素挨个pop front出来，插入到insert_iter的后面即可
    // 这里这个head的指针就是他自己啊 要把这些iter的名字改成head
    while (remaining_head->GetNext() != remaining_head) {
      // TODO(zhangzhong): 这段代码在这个函数里面重复了三次了
      // 可以抽象出来，就是从head中erase after，然后插入到某个节点后面
      // 然后返回插入的节点的指针就行了
      // SinglyLinkedListNode* tmp = remaining_head->GetNext();
      // // 一定要先断开链表 再插入 否则链表会断掉 无法遍历后续的节点
      // // 甚至可能出现死循环的bug
      // remaining_head->EraseAfter();
      // insert_iter->InsertAfter(tmp);
      // // 更新insert
      // insert_iter = insert_iter->GetNext();
      insert_after = _pop_front_to(remaining_head, insert_after);
    }

    // 最终insert_iter指向什么？
    // 如果两个链表都是空的，那么insert_iter指向new_head
    // 否则insert_iter指向new_head的最后一个节点
    // 现在我们需要将
    // case 2
    // this_head->SetNext(new_head.GetNext());
    // insert_after->SetNext(this_head);
    // 分析一下是否适用于case 1
    // 不行 不适用于 第一句代码就把 this_head -> new_head 给串起来了
    // 所以这个函数应该在最开始检查一下，两个链表都不能是空的

    // // 更新size
    // this->_size = merged_size;
    // 这样的话，我们需要返回两个东西，一个是begin 一个是tail
    //
    return {new_head.GetNext(), insert_after};
  }

  static SinglyLinkedListNode* _pop_front_to(SinglyLinkedListNode* head,
                                             SinglyLinkedListNode* insert) {

    SinglyLinkedListNode* tmp = head->GetNext();
    head->EraseAfter();
    insert->InsertAfter(tmp);
    return tmp;
  }

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

  list.sort();
}
