# C++数组引用传参

> 使用数组的引用作为参数传递给函数，让原始数组不输入长度也能被函数输出

实际上是使用模板自动推测传入引用的数据类型和长度。

## 第一版

> 这一版是相对比较原始的，要指定对应的数组类型。原文链接[C++数组引用传参](https://blog.csdn.net/u014304293/article/details/22867715?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-4.nonecase&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-4.nonecase)

```C++
#include <iostream>

using namespace std;

template<size_t array_size>
void print(int (&a)[array_size])
{
    for (auto i : a)
    {
        cout << i << ends;
    }
    cout << endl;
}

int main()
{
    
    int a[]{ 1, 2, 3, 4, 5 };

    print(a);

}
```

## 第二版

> 这一版用模板指定数组的类型，因此可以处理任意类型的数组

```C++
#include <iostream>

using namespace std;

template<size_t array_size, template T>
void print(T (&a)[array_size])
{
    for (auto i : a)
    {
        cout << i << ends;
    }
    cout << endl;
}

int main()
{
    
    double a[]{ 1, 2, 3.14, 4, 5 };

    print(a);

}
```

## 第三版

> 这一版直接用模板代替了数组长度推测的部分，代码上最为简单

```C++
template<typename T>
void print(T &a)
{
    for (auto i : a)
    {
        cout << i << ends;
    }
    cout << endl;
}
```
---

这涉及到C++模板的一些特性：类型模板参数、无类型模板参数和模板模板参数
[参考链接](https://www.cnblogs.com/dongzhiquan/p/7726405.html)
