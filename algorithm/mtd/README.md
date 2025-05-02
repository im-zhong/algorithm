# My Templated Library, mtd

## C++ tips

1. <https://chat.deepseek.com/a/chat/s/2bc4b45c-d6b4-45d0-95dc-4accbbb1c39b>
在C++中，若在多个源文件中多次初始化同一个类的静态成员变量并赋予不同值，会导致以下结果：

1. **编译阶段**：单个源文件（`.cpp`）独立编译时不会报错，因为每个文件中的初始化语句在语法上是合法的。

2. **链接阶段**：链接器会检测到多个目标文件（`.o`或`.obj`）中存在对同一静态成员变量的重复定义，触发**链接错误**。例如，GCC会报错：`重复定义 'MyClass::myStaticVar'`，MSVC则提示`LNK2005: 符号已定义`。

3. **根本原因**：违反C++的**单一定义规则（ODR）**。静态成员变量的初始化属于定义，且每个符号在程序中只能有唯一定义。

4. **未定义行为**：若强行绕过链接错误（如通过特殊编译选项），程序行为将不可预测。最终变量的值可能取决于链接顺序或优化策略，但这种情况应严格避免。

### 示例代码

```cpp
// MyClass.h
class MyClass {
public:
    static int myStaticVar;  // 声明
};

// File1.cpp
#include "MyClass.h"
int MyClass::myStaticVar = 5;  // 定义

// File2.cpp
#include "MyClass.h"
int MyClass::myStaticVar = 10;  // 重复定义，引发链接错误
```

### 正确做法

- **单一初始化**：在**一个且仅一个源文件**中初始化静态成员变量。
- **类内初始化（C++17起）**：若静态成员为`const`且类型为整型或枚举，可直接在类内初始化：

  ```cpp
  class MyClass {
  public:
      static const int myStaticVar = 42;  // 合法（C++17及以后）
  };
  ```

### 总结

多次初始化同一静态成员变量必然导致链接失败，程序无法生成可执行文件。遵循单一定义规则，确保静态成员变量仅初始化一次。

在 C++ 中，`::operator new` 和直接使用 `new` 操作符在底层机制和使用场景上有显著区别。以下是两者的核心差异：

---

### **1. 基本功能**

- **`new` 操作符**
  是一个高级操作符，包含 **两步操作**：
  - **分配内存**：调用底层 `operator new` 函数分配原始内存。
  - **构造对象**：在分配的内存上调用对象的构造函数（若有对象类型）。

  ```cpp
  MyClass* obj = new MyClass();  // 分配内存 + 调用构造函数
  ```

- **`::operator new`**
  是一个底层内存分配函数，**仅分配原始内存**（类似 `malloc`），**不涉及构造函数调用**。

  ```cpp
  void* raw_mem = ::operator new(sizeof(MyClass));  // 仅分配内存，不构造对象
  ```

---

### **2. 使用场景**

- **`new` 的典型用途**
  直接用于创建动态对象，隐式处理内存分配和构造：

  ```cpp
  MyClass* obj = new MyClass();  // 一步完成分配和构造
  ```

- **`::operator new` 的典型用途**
  需要手动分离内存分配和对象构造的场景（例如实现内存池或自定义分配策略）：

  ```cpp
  // 分配原始内存
  void* raw_mem = ::operator new(sizeof(MyClass));

  // 使用 placement new 在原始内存上构造对象
  MyClass* obj = new (raw_mem) MyClass();  // 调用构造函数

  // 手动析构并释放内存
  obj->~MyClass();
  ::operator delete(raw_mem);
  ```

---

### **3. 异常行为**

- **`new`**
  默认在内存不足时抛出 `std::bad_alloc` 异常。
  可通过 `nothrow` 版本避免异常（返回 `nullptr`）：

  ```cpp
  MyClass* obj = new (std::nothrow) MyClass();  // 失败返回 nullptr
  ```

- **`::operator new`**
  默认抛出 `std::bad_alloc`，但可通过参数控制异常行为：

  ```cpp
  void* raw_mem = ::operator new(size, std::nothrow);  // 不抛异常，失败返回 nullptr
  ```

---

### **4. 重载行为**

- **`new` 操作符**
  若类中重载了 `operator new`，则优先使用类的重载版本：

  ```cpp
  class MyClass {
  public:
      static void* operator new(size_t size) {
          // 自定义分配逻辑
      }
  };

  MyClass* obj = new MyClass();  // 调用 MyClass::operator new
  ```

- **`::operator new`**
  强制使用全局版本的 `operator new`，绕过类的重载：

  ```cpp
  void* raw_mem = ::operator new(sizeof(MyClass));  // 始终使用全局 operator new
  ```

---

### **5. 内存对齐（C++17 起）**

- **`::operator new`**
  支持显式指定内存对齐方式：

  ```cpp
  // 分配 64 字节对齐的内存
  void* aligned_mem = ::operator new(1024, std::align_val_t(64));
  ```

- **`new`**
  对齐由编译器隐式处理，通常无法直接指定（除非通过类型特性）。

---

### **6. 释放方式**

- **`new` 分配的内存**
  必须用 `delete` 释放，触发析构函数并释放内存：

  ```cpp
  delete obj;  // 调用析构函数 + 释放内存
  ```

- **`::operator new` 分配的内存**
  必须用 `::operator delete` 释放，且需手动调用析构函数（若已构造对象）：

  ```cpp
  obj->~MyClass();            // 手动析构
  ::operator delete(raw_mem); // 释放原始内存
  ```

---

### **总结**

| **特性**                | `new` 操作符               | `::operator new`            |
|-------------------------|---------------------------|-----------------------------|
| **内存分配 + 构造**     | ✔️                        | ❌（仅分配内存）             |
| **异常行为**            | 默认抛异常 / 可禁用       | 默认抛异常 / 可禁用         |
| **重载敏感**            | 受类内重载影响            | 强制使用全局版本            |
| **内存对齐控制**        | 隐式                      | 显式（C++17+）              |
| **释放方式**            | `delete`                 | `::operator delete` + 析构  |

---

### **何时使用哪种方式？**

- 使用 `new`：需要简单、安全地创建动态对象。
- 使用 `::operator new`：需要手动管理内存生命周期（如实现内存池、优化性能或调试内存问题）。

在 C++17 中，`std::allocator` 的成员类型（如 `pointer`、`const_pointer`、`reference`、`const_reference`）被标记为 **弃用**，并在 C++20 中正式移除。这是 C++ 标准库演进中的重要变化，其背后的设计动机和替代方法如下：

---

### **为什么这些类型被弃用？**

1. **简化分配器接口**
   C++ 标准委员会希望将分配器的核心职责明确为 **纯内存管理**（分配/释放原始内存），而不是定义与对象相关的语义（如指针和引用类型）。这种分离符合“单一职责原则”，使分配器的设计更清晰。

2. **推广 `std::allocator_traits` 的使用**
   C++11 引入了 `std::allocator_traits`，它通过模板特化为所有符合 `Allocator` 概念的类型提供**统一的访问接口**。即使某个分配器未显式定义 `pointer` 等成员类型，`allocator_traits` 也能通过默认模板推导出合理的类型（例如默认 `pointer` 为 `T*`）。弃用直接成员类型是为了强制开发者使用 `allocator_traits`，从而写出更通用、更兼容的代码。

3. **支持更灵活的分配器**
   某些自定义分配器（例如基于内存池或非连续内存的分配器）可能需要定义自己的 `pointer` 类型（例如代理指针或句柄）。通过 `allocator_traits` 统一提取这些类型，可以避免硬编码依赖 `T*`，增强代码的扩展性。

---

### **如何获取分配器的指针类型？**

应使用 **`std::allocator_traits<Alloc>::pointer`** 代替直接访问 `Alloc::pointer`。即使分配器未显式定义 `pointer`，`allocator_traits` 也会提供默认值（`value_type*`）。

#### 示例代码

```cpp
#include <memory>
#include <vector>

template <typename Alloc>
void example_usage() {
    using Traits = std::allocator_traits<Alloc>;
    using Pointer = typename Traits::pointer;       // 正确获取指针类型
    using ConstPointer = typename Traits::const_pointer;

    Alloc alloc;
    Pointer p = Traits::allocate(alloc, 1);        // 分配内存
    Traits::construct(alloc, p, 42);               // 构造对象（若需要）
    // ...
    Traits::deallocate(alloc, p, 1);               // 释放内存
}

int main() {
    example_usage<std::allocator<int>>();
    return 0;
}
```

---

### **关键变化总结**

| **C++ 版本**       | **行为**                                                                 |
|--------------------|--------------------------------------------------------------------------|
| **C++11 之前**     | `std::allocator` 必须显式定义 `pointer`、`const_pointer` 等成员类型。     |
| **C++17**          | 弃用 `pointer`、`const_pointer`、`reference`、`const_reference` 成员类型。 |
| **C++20 及之后**   | 彻底移除这些成员类型，强制使用 `std::allocator_traits`。                  |

---

### **设计哲学：依赖 `allocator_traits` 的优势**

1. **默认行为兼容性**
   如果分配器未定义 `pointer`，`allocator_traits` 会默认使用 `value_type*`。这意味着即使分配器没有显式提供这些类型，代码仍能正常工作。

2. **支持异构分配器**
   对于返回“胖指针”或自定义指针类型的分配器（例如在特定硬件或内存模型中的指针），`allocator_traits` 能正确提取其类型，而无需修改使用方代码。

3. **统一接口**
   所有标准库容器（如 `std::vector`、`std::list`）内部均通过 `allocator_traits` 与分配器交互。用户代码也应遵循此模式以保证一致性。

---

### **迁移指南**

- **旧代码（C++17 前）**

  ```cpp
  std::allocator<int> alloc;
  std::allocator<int>::pointer p = alloc.allocate(1);
  ```

- **新代码（C++17+）**

  ```cpp
  std::allocator<int> alloc;
  using Traits = std::allocator_traits<decltype(alloc)>;
  Traits::pointer p = Traits::allocate(alloc, 1);
  ```

---

### **扩展：`std::allocator` 的其他变化**

- **`void` 特化的移除（C++20）**
  C++20 移除了 `std::allocator<void>` 的特化，因为 `std::allocator_traits` 已能正确处理 `void` 类型，无需特化。直接使用通用模板即可：

  ```cpp
  std::allocator<void> alloc;  // C++20 前合法，C++20 起错误
  std::allocator<std::byte> alloc; // 替代方案（C++17 引入 std::byte）
  ```

---

### **总结**

- **弃用原因**：推动使用 `std::allocator_traits`，简化分配器设计，提升灵活性。
- **替代方案**：通过 `std::allocator_traits<Alloc>::pointer` 获取指针类型。
- **最佳实践**：始终通过 `allocator_traits` 与分配器交互，避免直接依赖分配器的成员类型。

书上并没有提到 allocator triatis, 代码中给出了一些高级的内存实现，而不是直接封装new和delete。我更想直接看trait技巧。我现在大概知道allocator是干什么的了，是什么接口，足够我向下看了。

在大多数C++标准库实现（如GCC的libstdc++和LLVM的libc++）中，**全局的 `::operator new` 默认确实会调用 `malloc`**，但它的实现包含了C++特有的逻辑（如异常处理），并非直接等价于`malloc`。以下是关键细节：

---

### **1. 默认行为：`operator new` 与 `malloc` 的关系**

- **底层依赖**
  标准库通常将 `::operator new` 实现为对 `malloc` 的封装。例如，在glibc中，`operator new` 的简化逻辑如下：

  ```cpp
  void* operator new(size_t size) {
      void* ptr = malloc(size);
      if (ptr == nullptr)
          throw std::bad_alloc();  // 分配失败时抛异常
      return ptr;
  }
  ```

  - **与 `malloc` 的区别**：`operator new` 在内存不足时抛出异常，而 `malloc` 返回 `nullptr`。

- **内存对齐**
  C++17 起，`operator new` 默认保证分配的内存满足 `alignof(std::max_align_t)` 对齐要求（与 `malloc` 一致）。对于更高对齐的需求，需使用对齐版本：

  ```cpp
  void* ptr = ::operator new(size, std::align_val_t(64));  // 64字节对齐
  ```

---

### **2. 自定义分配：重载 `operator new`**

- **全局重载**
  开发者可以重载全局的 `::operator new`，完全绕过 `malloc`：

  ```cpp
  void* operator new(size_t size) {
      // 自定义分配逻辑（如内存池）
      return custom_alloc(size);
  }
  ```

  此时，标准库容器的内存分配将使用此自定义实现。

- **类特定重载**
  类可以定义自己的 `operator new`，仅影响该类的对象分配：

  ```cpp
  class MyClass {
  public:
      static void* operator new(size_t size) {
          return custom_alloc_for_myclass(size);
      }
  };
  ```

---

### **3. 实现差异：标准库的灵活性**

- **标准未强制要求**
  C++标准并未规定 `operator new` 必须基于 `malloc`，这只是一种常见实现选择。某些嵌入式环境的标准库可能直接管理内存，无需依赖 `malloc`。

- **调试与工具**
  一些调试工具（如Valgrind）会拦截 `operator new` 和 `malloc` 调用以检测内存泄漏，但两者的调用栈是独立的。

---

### **4. 与 `new` 操作符的关系**

- **`new` 的完整流程**
  当使用 `new T()` 时，实际发生两步操作：
  1. **内存分配**：调用 `operator new(sizeof(T))`（可能全局或类特定版本）。
  2. **对象构造**：在分配的内存上调用 `T` 的构造函数。

- **`malloc` 的局限性**
  直接使用 `malloc` 分配内存后，需手动调用构造函数（通过placement new）：

  ```cpp
  T* obj = static_cast<T*>(malloc(sizeof(T)));
  new (obj) T();  // 手动构造
  ```

---

### **总结**

| **特性**          | `::operator new`                     | `malloc`                          |
|--------------------|--------------------------------------|-----------------------------------|
| **语言**          | C++                                  | C                                 |
| **失败行为**      | 抛出 `std::bad_alloc`               | 返回 `nullptr`                   |
| **内存对齐**      | 默认同 `malloc`，可扩展高对齐需求   | 依赖实现，通常为自然对齐          |
| **构造/析构**     | 仅分配内存，不调用构造/析构         | 同上                              |
| **可重载性**      | 支持全局或类特定重载                | 不可重载                          |
| **底层实现**      | 通常封装 `malloc`，但非强制         | 直接由C库管理                     |

---

### **何时需要关注这一细节？**

- **性能优化**：若需替换默认内存分配策略（如使用内存池），可重载 `operator new`。
- **调试内存问题**：理解 `operator new` 与 `malloc` 的关系有助于分析内存泄漏或碎片。
- **跨语言交互**：在C/C++混合代码中，需确保 `malloc`/`free` 与 `new`/`delete` 的配对使用。

## Question

1. 为什么使用protected继承？

## TODO

1. 我需要研究一下type tratis 技法， 也就是实现一下头文件<type_traits>
2. 我觉得这不是可以在短时间内学完的，我们需要慢慢来，我们需要抽时间，慢慢实现，我们先根据目前学习到的知识，写一个简单的，不包含复杂的type traits的实现，或者，我们先使用type traits！而不是实现他们！对，先用！慢慢的熟悉了，在想着实现，语言特性有点多，全学完再刷题的话，又TM全忘了！
3. just check the <<C++ templates: The Complete guide>> Appendix D! using the standard type utilities! 嗯 浏览了一遍，我感觉我行了！
4. <https://chat.deepseek.com/a/chat/s/6007e594-822d-448f-b0b2-578780622137> 模板元编程实现判断殊俗的一个例子
