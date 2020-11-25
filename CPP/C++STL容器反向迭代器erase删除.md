# C++STL容器反向迭代器reverse_iterator的erase删除

参考代码：

```c++
std::next(s.rbegin()).base()
```

其中用到了两个函数，一个是 [`std::next()`](https://zh.cppreference.com/w/cpp/iterator/next) ，另一个是 [`std::list<T,Allocator>::erase`](https://zh.cppreference.com/w/cpp/iterator/reverse_iterator/base) 。

前者作用是返回迭代器 `it` 的第 `n` 个后继：

```c++
// std >= c++11
template< class InputIt >
InputIt next(
  InputIt it,
  typename std::iterator_traits<InputIt>::difference_type n = 1 );

// std >= c++17
template< class InputIt >
constexpr InputIt next(
  InputIt it,
  typename std::iterator_traits<InputIt>::difference_type n = 1 );
```

后者作用是返回**底层迭代器**，即  `reverse_iterator` **当前所指元素的下一个元素**。

```c++
// std < c++17
iterator_type base() const;

// std >= c++17
constexpr iterator_type base() const;
```

引用 C++ 文档中文站的一句话：

> 尽管表达式 ++c.begin() 通常能编译，然而不保证会这么做： c.begin() 是右值表达式，而无[*遗留输入迭代器* *(LegacyInputIterator)* ](https://zh.cppreference.com/w/cpp/named_req/InputIterator)要求指定右值的自增保证进行。尤其是迭代器以指针实现或其 `operator++` 为左值引用限定时， ++c.begin() 不能编译，而 std::next(c.begin()) 可以。

建议使用 std::next 而非迭代器自增自减。

---

参考链接:

- [<ZH cppreference>std::next](https://zh.cppreference.com/w/cpp/iterator/next)
- [<ZH cppreference>std::reverse_iterator<Iter>::base](https://zh.cppreference.com/w/cpp/iterator/reverse_iterator/base)
- [<stack overflow> How to call erase with a reverse iterator](https://stackoverflow.com/questions/1830158/how-to-call-erase-with-a-reverse-iterator)
- [<CSDN> 在遍历中使用 iterator/reverse_iterator 进行 Erase 的用法](https://blog.csdn.net/kesalin/article/details/24265303)

