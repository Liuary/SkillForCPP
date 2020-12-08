# C++宽字符串和普通字符串的相互转换

```C++

// 请将本文档编码设置为 UTF-8
#include <iostream>
#include <string>
#include <cstdlib>
#include <locale>
#include <clocale>
#include <codecvt>

// 将一个 UTF-8 编码的宽字符 std::wstring 转成 UTF-8 编码的普通字符 std::string
std::string CastWstrToStr(const std::wstring& wstr)
{
    // // 这个无法实现多字节字符的转换,故弃用,选择C语言风格的函数转换得到普通字符串
    // // 指定编码和字符类型
    // std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    // // 返回字节流
    // return converter.to_bytes(wstr);

    // C语言风格实现
    // 获取传入宽字符串的 C 风格字符串
    const wchar_t* wstr_c = wstr.c_str();
    // 多字节编码规则集合的初始转化状态，参考: https://zh.cppreference.com/w/cpp/string/multibyte/mbstate_t
    std::mbstate_t state = std::mbstate_t();
    // 创建一个普通字符串，并且初始化所有元素为普通字节的空格字符
    std::string str(wstr.size() + 1, ' ');
    // 将传入宽字符串映射为普通字符串，参考: https://zh.cppreference.com/w/cpp/string/multibyte/wcsrtombs
    str.resize(std::wcsrtombs(&str[0], &wstr_c, str.size(), &state));
    
    // 返回转换完成的普通字符串
    return str;

}

// 这个函数用来将宽字符串转换为可输出的普通字符串（宽字符中文需要转换才能输出），如果代码中编码确定为 UTF8 且确定采用宽字符串，那么请使用这个函数
std::string CastWstrToStrDef(const std::wstring& wstr)
{
    // 指定编码和字符类型
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    // 返回字节流
    return converter.to_bytes(wstr);
}

// 将一个 UTF-8 编码的普通字符 std::string 转成 UTF-8 编码的宽字符 std::wstring
std::wstring CastStrToWstr(const std::string& str)
{

    // // 这个无法实现多字节字符的转换,故弃用,选择C语言风格的函数转换得到宽字符串
    // // 指定编码和字符类型
    // std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    // // 返回从字节流得到的宽字符
    // return converter.from_bytes(str);

    // C语言风格实现
    // 获取传入字符串的 C 风格字符串
    const char* str_c = str.c_str();
    // 多字节编码规则集合的初始转化状态，参考: https://zh.cppreference.com/w/cpp/string/multibyte/mbstate_t
    std::mbstate_t state = std::mbstate_t();
    // 创建一个宽字符串，并且初始化所有元素为宽字节的空格字符
    std::wstring wstr(str.size() + 1, L' ');
    // 将传入字符串映射为宽字符串，参考: https://zh.cppreference.com/w/cpp/string/multibyte/mbsrtowcs
    wstr.resize(std::mbsrtowcs(&wstr[0], &str_c, wstr.size(), &state));

    // 返回转换完成的宽字符串
    return wstr;
}


int main(int argc, char const *argv[])
{
    
#ifdef _WIN32
#ifndef SET_CMD_UTF8
    std::setlocale(LC_ALL, "zh-CN.utf8");
    system("C:\\Windows\\system32\\chcp 65001");
    // system("cls");
#define SET_CMD_UTF8
#endif
#endif

    std::cout << "------ u8 -> ws -> u8 \n";
    std::string sstr = "这是一段UTF8中文...";
    std::cout << sstr << " " << sstr.size() << "\n";
    std::wstring wst = CastStrToWstr(sstr);
    std::wcout << wst << L" " << wst.size() << L"\n";
    std::string st = CastWstrToStr(wst);
    std::cout << st << " " << st.size() << "\n";
    std::cout << "------  \n\n";

    std::cout << "------ ws -> u8 \n";
    std::wstring swstr = L"一二三四5";
    std::wcout << L"wide string size is : " << swstr.size() << L"\n";
    std::string st2 = CastWstrToStrDef(swstr);
    std::cout << st2 << " " << st2.size() << "\n";
    std::cout << "------  \n\n";

    return 0;
}

```
