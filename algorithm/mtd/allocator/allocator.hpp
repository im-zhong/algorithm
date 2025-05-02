// 2025/4/30
// zhangzhong
// allocator

// file:///Users/zhangzhong/Downloads/html-book-20250209/reference/en/cpp/memory.html#Allocators
// 这里面东西挺多的额，咱们不急，先看C++11的东西就行
// C++11的学完了，再去看C++17的部分
// 这部分非常关键，是基础。

// 虽然书上给出了一个优化过的allocator实现，但是我认为这是没有必要的
// 因为malloc是经过高度优化的，没必要在库代码里再优化一遍，除非有特殊的需求，要完全绕过malloc
// 而且书上没有使用 allocator_traits , 我需要使用这个
// 我决定部分参考书中的代码，主要参考现有的标准，实现一个简单的，仅仅对operator
// new的一层薄薄的封装 即可
// 但是第一章剩下的，涉及到语法细节的也需要看

// file:///Users/zhangzhong/Downloads/html-book-20250209/reference/en/cpp/memory/allocator_traits/construct.html
// 将一个对象的构造分成两个阶段
// 1. 内存的申请 alloc, dealloc
// 2. 对象的构造 construct, destruct

namespace mtd {

template <typename T, typename... Args>
static void construct(Alloc& alloc, T* p, Args&&... args) {}

// destroy

}; // namespace mtd
