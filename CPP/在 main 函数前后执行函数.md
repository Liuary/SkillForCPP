# 在 main 函数前后执行函数

```c++
/**
 * @file OrderFunctExect.cpp
 * @author Liuary (Liuary@domain.com)
 * @brief 用以展示在 main 函数前后执行函数的操作
 * @version 0.1
 * @date 2022-04-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <iostream>
using namespace std;

// 主函数
int main(int argc, char const *argv[])
{
    cout << "main\n";
    return 0;
}

// 全局类构造函数会先于 main 执行
class MainClass
{
public:
    MainClass(int num)
    {
        cout << "Main Class " + to_string(num) + " !\n";
    }
};

// 全局变量调用函数初始化也会先于 main 执行
const static int _ = []()
{
    cout << "Init onexit.\n";
    // _onexit 回调函数的参数（函数）会按照栈弹出的方式在 main 之后执行
    _onexit([]()
    {
        cout << "On Exit !\n";
        return 0;
    });
    return 0;
}();

MainClass mainClass1(1);
MainClass mainClass2(2);
MainClass mainClass3(3);
MainClass mainClass4(4);
MainClass mainClass5(5);
```
