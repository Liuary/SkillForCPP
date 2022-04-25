# constexpr

实际上就是告诉编辑器这玩意应该在编译期间得到确定值，但是如果得不到就无效

```c++
/// constexpr

#include <iostream>
#include <vector>

using namespace std;

const int GetConst()
{
    // const int a = 1;
    return 1;
}

int aa = 2;

constexpr int GetConstexpr()
{
    int a = 2;
    // return aa;
    return a;
}

template<int index>
void PrintVector(vector<int> arr)
{
    if (index >= 0 && index < arr.size())
    {
        cout << arr[index] << endl;
    }
}

int main(int argc, char const *argv[])
{

    vector<int> a{0, 1, 2, 3};

    // cout << "Const ";
    // PrintVector<GetConst()>(a);
    cout << "Constexpr ";
    PrintVector<GetConstexpr()>(a);

    return 0;
}

```
