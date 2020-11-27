# UE4 和 C++ Windows文件选择窗口（返回文件路径）

## 前言

`Unreal` 作为一个功能完备的游戏引擎，自然是包含了**文件选择窗口**的功能，这个功能被封装在模块 `DesktopPlatform` 中。

不过，这个模块是源码中 `Developer` 的部分，因此如果想要将项目打包成 `shipping` 模式（即发行模式），就会报错。

个人有在网上寻找解决方案，有[方案](https://www.cnblogs.com/linqing/p/12654996.html)是将 `Developer` 下对应的源代码提取出来，修改之后作为自定义的模块使用。

不过，在实际操作过程中，我发现该模块对 `Developer` 内容的依赖很深，修改起来难度颇大。

这可能是因为本人对 UE4 模块与源码相关内容比较陌生的缘故。

当然，这个方案是不符合 ELUA 协议的，因此用户最好自己写一个文件选择窗口。

只是文件选择窗口这个功能在游戏这一块应该不是一个很需要的东西。

最终我的方案是使用 `Windows.h` 中的 `GetOpenFileName` 函数来实现的，这个方案只能用于 `Windows` ， 而且是 `Win7` 及以后的版本。保险起见，请在 `Win10` 或者更新的版本中使用这一方案。

## 正文

在 Unreal 中使用 Win32 的 API 其实并不是一件容易的事情。

首先肯定是要包含头文件 Windows.h 的：

```c++
#include <Windows.h>
```

然后大概率会报一堆关于 Windows 版本的错误，因此你需要在这一行前面加上一个宏定义：

```c++
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
```

这还没完，在 Unreal 里你可能找不到对应的头文件，你可能还要找到一个叫 `commdlg.h` 的头文件，添加其路径：

```c++
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// 这个路径不同的电脑可能是不一致的，请自行寻找合适的位置，不过这个可能用不到
#include <C:\Program Files (x86)\Windows Kits\10\Include\10.0.19041.0\um\commdlg.h>
```

下面是调用对应函数的源码，文件筛选器我做的是图片的筛选器：

```c++
/*
// 这里是放到.h文件里的内容

// 打开文件的模式（单选，多选）
UENUM()
enum FSelectMode
{
	None,
	Multiple
};

// 打开文件选择对话框（放在蓝图函数库）
	UFUNCTION(BlueprintCallable, Category = "OpenWindowsFile")
		static bool OpenFilesSelectDialog(FSelectMode Mode, TArray<FString>& FilePathArray);

*/


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <C:\Program Files (x86)\Windows Kits\10\Include\10.0.19041.0\um\commdlg.h>

// 将 LPCWSTR 转换为 FString
void LpcwstrToString(const LPWSTR Lps, TArray<FString>& FilePathArray) {
	UE_LOG(LogTemp, Warning, TEXT("Call function LpcwstrToString..."));
	// 获取文件路径（这个是不包含文件名的）
	FString Path(Lps);

	const wchar_t* Current = Lps + Path.Len() + 1;

	// int32 Len = 8;

	while (*Current)
	{
		UE_LOG(LogTemp, Warning, TEXT("While..."));
		FString File(Current);
		
		FilePathArray.Add(Path + L"\\" + File);
		Current += (File.Len() + 1);
	}
}

// 这个前面的类名自己写
bool ULibOpenFilesDialog::OpenFilesSelectDialog(FSelectMode Mode, TArray<FString>& FilePathArray)
{
	
	UE_LOG(LogTemp, Warning, TEXT("Call function OpenFilesSelectDialog..."));

	// 存储文件信息
	OPENFILENAME OpenInfos;

	char szFile[300];

	ZeroMemory(&OpenInfos, sizeof(OpenInfos));

	// 多选或者单选
	Mode ?
		OpenInfos.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER :
		OpenInfos.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;

	OpenInfos.lStructSize = sizeof(OpenInfos);
	OpenInfos.hwndOwner = NULL;
	OpenInfos.lpstrFile = (LPWSTR)szFile;
	OpenInfos.lpstrFile[0] = '\0';
	// LPTSTR        lpstrCustomFilter;
	// DWORD         nMaxCustFilter;
	OpenInfos.nFilterIndex = 1;
	// LPTSTR        lpstrFile;
	OpenInfos.nMaxFile = sizeof(szFile);
	// OpenInfos.lpstrFilter = L"ALL\0 * .*\0Text\0 * .TXT\0";
	OpenInfos.lpstrFilter = L"Image\0*.png;*.jpg;*.jpeg;*.bmp;*.ico;*.exr;*.icns";
	OpenInfos.lpstrFileTitle = NULL;
	OpenInfos.nMaxFileTitle = 0;
	OpenInfos.lpstrInitialDir = NULL;

	if (GetOpenFileName(&OpenInfos))
	{
		LpcwstrToString(OpenInfos.lpstrFile, FilePathArray);
		return true;
	}
	return false;
}
```

如果到这一步编译成功，调用也没啥问题，那就皆大欢喜。

如果报了一些源码函数的错误，可以试试在你添加头文件（Windows.h）和宏定义（WIN32_LEAN_AND_MEAN）的前面加上下列内容：

```c++
#include "UMG.h"
#include "UMGStyle.h"
#include "Slate/SObjectWidget.h"
#include "IUMGModule.h"
#include "Blueprint/UserWidget.h"
```

如果还是不行，就只好别寻他法咯。

## 参考链接

- [博客园 -> 林清 - 整理 Developer Module 到 Source ](https://www.cnblogs.com/linqing/p/12654996.html)
- [CSDN -> coolsunxu - C++打开文件选择窗口](https://blog.csdn.net/coolsunxu/article/details/82915531)
- [Microsoft文档 - GetOpenFileName 函数文档](https://docs.microsoft.com/en-us/windows/win32/api/commdlg/nf-commdlg-getopenfilenamea)
- [Microsoft文档 - OPENFILENAME 结构体文档](https://docs.microsoft.com/en-us/windows/win32/api/commdlg/ns-commdlg-openfilenamea)
- [StackOverflow - OPENFILENAME returns incorrect lpstrFile when picking multi-files](https://stackoverflow.com/questions/39410075/openfilename-returns-incorrect-lpstrfile-when-picking-multi-files)

