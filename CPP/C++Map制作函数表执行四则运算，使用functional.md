# C++Map制作函数表执行四则运算，使用functional

> 使用std::map制作函数表，用到了functional和lambda。代码因为展示多种不同的写法，因此比较杂乱，实际应根据需求选择最合适的一种来写。

```C++
#include <iostream>
#include <map>
#include <functional>
#include <cmath>

using namespace std;

int add(int a, int b)
{
    return a + b;
}

auto mod = [](int a, int b)
{
    return a % b;
};

class divide
{
public:
    int operator()(int de, int di)
    {
        return de / di;
    }
};

int main(int argc, char const *argv[])
{
    map<string, function<int(int, int)>> binops = 
        {
            {"+", add},
            {"-", std::minus<int>()},
            {"*", [](int a, int b){return a * b;}},
            {"/", divide()},
            {"%", mod}
        };
    binops["^"] = [](int a, int b){return pow(a, b);};
    binops.insert({"\\", [](int a, int b){return (int)pow(a, 1.0/b);}});
    cout << binops["\\"](27, 3) << endl;
    std::system("pause");
    return 0;
}

```
