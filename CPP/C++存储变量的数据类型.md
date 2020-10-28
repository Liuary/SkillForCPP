# 用来保存变量的类型，参考 STL 容器中的 value_type 写成：

## 第一版

这一版需要指定变量类型

```c++
#include <iostream>

using namespace std;

// 存储使用的类型
template<typename T>
struct TypeSave
{
    typedef T value_type;
};


int main(int argc, char const *argv[])
{
    TypeSave<double> Td;

    // 取出使用的类型
    decltype(Td)::value_type a;

    a = 2.33;

    cout << a << endl;

    return 0;
}
```

## 第二版

这一版使用了模板函数，创建时不需要指定类型

```c++
#include <iostream>

using namespace std;

// 存储使用的类型
template<typename T>
struct TypeSave
{
    typedef T value_type;
};

// 将传入参数的类型保存下来，返回一个 TypeSave 类型结构体
template<typename T>
TypeSave<T> MakeTypeSave(T var)
{
    TypeSave<T> val = var;
    return val;
}


int main(int argc, char const *argv[])
{

    // 直接获取类型即可
    decltype(MakeTypeSave(2.33))::value_type b = 1.01;

    cout << 1.01 << endl;

    return 0;
}
```

# 第三版

使用宏定义降低代码量

```c++
#include <iostream>

using namespace std;

// 存储使用的类型
template<typename T>
struct TypeSave
{
    typedef T value_type;
};

// 将传入参数的类型保存下来，返回一个 TypeSave 类型结构体
template<typename T>
TypeSave<T> MakeTypeSave(T var)
{
    TypeSave<T> val = var;
    return val;
}

// 宏定义以减少代码量
#define GetTypeSave(var) decltype(MakeTypeSave(var))
#define GetType(var) decltype(MakeTypeSave(var))::value_type


int main(int argc, char const *argv[])
{

    // 直接获取类型即可
    // decltype(MakeTypeSave(2.33))::value_type b = 1.01;

    // 宏定义获取
    GetType(2.33) b = 1.01;

    cout << 1.01 << endl;

    return 0;
}

```
