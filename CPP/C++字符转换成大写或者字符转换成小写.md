# C++字符转换成大写或者字符转换成小写

> 实际上是用的位运算：与运算和或运算

```C++
#include <iostream>
#include <string>

using namespace std;

string getDaxie(string str)
{
    string res = "";
    for (auto i : str)
    {
        res += (i & 223);
    }
    return res;
}

string getXiaoxie(string str)
{
    string res = "";
    for (auto i : str)
    {
        res += (i | 32);
    }
    return res;
}

int main(int argc, char const *argv[])
{
    cout << getDaxie("AbAc") << endl;
    cout << getXiaoxie("AbAc") << endl;
    return 0;
}
```
