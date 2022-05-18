在 UE4 中，想要进行数据存储，那么就需要了解到 `UGameInstance` 和 `USaveGame` 两个类。

# 一、UGameInstance

[GameInstance](https://docs.unrealengine.com/4.26/en-US/API/Runtime/Engine/Engine/UGameInstance/) 在游戏**开始**时产生，在游戏**结束**时销毁。

在整个游戏进程中， GameInstance 拥有唯一而完整的生命周期，这使得它可以用来存储全局的变量。

即使切换关卡，GameInstance 中的全局变量也不会被重新初始化。

# 二、USaveGame

[SaveGame](https://docs.unrealengine.com/4.26/en-US/API/Runtime/Engine/GameFramework/USaveGame/) 用来存储游戏中的数据到本地，有同步、异步以及同步存取到内存中的选择。

# 三、存/读档流程

我们以跨`关卡`（[Level](https://docs.unrealengine.com/4.26/en-US/API/Runtime/Engine/Engine/ULevel/)）存储角色位置为示例，描述存取存档的流程。

## 1. 需求设计与分析

我们打算保存角色在每一个关卡最后停留的位置，以便我们重新载入本关卡的时候，角色能出现在之前停留的位置。

首先，我们可以考虑在每个关卡**初始化**的时候读取角色的位置，而在关卡**销毁**的时候存储角色的位置。

但是，这需要我们在每个关卡的脚本中都要去写一样的存取代码，显得十分麻烦。

因为切换关卡时，所有的 `Actor` （也包括角色）都会被销毁，而进入新关卡时，所有的 Actor 都会执行 BeginPlay 。

所以，我们可以直接将存取数据的代码写在 角色 `Character` 的 `BeginPlay` 和 `Destroyed` 中。

在游戏运行过程中，这些存取操作通过 GameInstance 实现，但是在游戏的开始和结束，则分别要在 SaveGame 中读取和存储。

存储的变量可以是一个 `TMap` ，参考如下：

```c++
// 玩家位置存储[关卡名, 坐标]
UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, FVector> PlayerPosition;
```

获取关卡名的方式如下：

```c++
if (ULevel* tLevel = GetLevel())
{
    return tLevel->GetPathName();
}
```

注意不能使用 GetFName 或者 GetName ，它们在未处理的情况下多个关卡只能得到相同的命名。

## 2. 设计 GameInstance 和 SaveGame

### (1) GameInstance

自定义 GameInstance 可以参考这一篇[博客](https://blog.csdn.net/ttm2d/article/details/106425836)。

具体的流程是:

- 继承 UGameInstance ，并在项目设置的 Map & Mode 中配置自定义 GameInstance 为继承的类
- 添加需要存储的变量
- 重载 GameInstance 的 `Init` 函数，在此处读取 SaveGame 存档
- 重载 GameInstance 的 `Shutdown` 函数，在此处保存 SaveGame 存档
- 为 GameInstance 添加一个静态函数以便在 C++ 中方便地获取其实例
 
### (2) SaveGame

自定义 SaveGame 可以参考这一篇[博客](https://www.likecs.com/show-540190.html)。

而在官方的文档中包含了自定义 SaveGame 委托的[函数参数格式](https://docs.unrealengine.com/4.26/zh-CN/InteractiveExperiences/SaveGame/)

其中的 `const FString& SlotName` 插槽命名也是存档文件的命名，存/读文档时要保证这个插槽命名一致，不然操作了不同的存档文件，无法正确的处理数据。

我个人采用了异步办法存取存档。

将存档和读档的函数写入 GameInstance 中，作为其的成员函数。

# 四、一些其他的问题

经过测试后， GameInstance 能符合预期，但是在游戏退出的时候，角色的 Destroyed 函数并不会被执行到。

这导致如果直接退出游戏，那么角色最后的位置并不会被同步到 GameInstance ，而 GameInstance 在保存到 SaveGame 的时候也就不包含了。

解决方法是手动退出游戏，并且在退出游戏之前将所有需要存档的数据同步到 GameInstance 。

具体方案分两步：

- GameInstance 中创建一个多播动态委托，在需要最后存档的地方添加这个委托
- GameInstance 添加一个退出游戏的函数，函数先执行委托，再执行退出操作。
 
委托/代理相关可以参考可可西的这篇[博客](https://www.cnblogs.com/kekec/p/10678905.html)

退出游戏的函数可以参考如下代码：

```c++
void ULCGlobalGameInstance::SaveGameAndExit()
{
    // 执行多播委托，同步数据
    OnSaveExit.Broadcast();
	
    if (GEngine)
	{
		UObject* tContextObject = GEngine->GetWorldFromContextObject(GEngine->GameViewport);
        // 退出游戏
		UKismetSystemLibrary::QuitGame(tContextObject, nullptr, EQuitPreference::Quit, false);
	}
}
```

然后在 `UMG` 中创建一个退出按钮，在点击事件中获取 GameInstance 并调用这个函数即可。
