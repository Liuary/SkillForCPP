# C++使用宽字符串输出中文

如下：

```c++
/**
 * 本代码用以测试宽字符串的使用（用以保存中文字符），文本编码为 UTF-8 ；
 * 因此在 Win 平台运行时，需要将 CMD 字符集设定为 UTF-8 。
 * 参考链接：https://my.oschina.net/shelllife/blog/1827897
 * */
#include <iostream>
#include <string>
#include <locale>
#include <codecvt>

using namespace std;

int main(int argc, char const *argv[])
{

// 在win环境下，将代码页切换到UTF-8
#ifdef _WIN32
    system("C:\\Windows\\system32\\chcp 65001");
    system("cls");
#endif
    
    // 创建一个中文宽字符串，需要以 L 限定字符串
    wstring ws(L"这是一段中文");

    // 将宽字符串转换为 UTF-8 编码的 std::string 。
    wstring_convert<codecvt_utf8<wchar_t>> covn;
    string res = covn.to_bytes(ws);

    // 输出已转换的 std::string 即可
    cout << res << endl;

    return 0;
}

```
