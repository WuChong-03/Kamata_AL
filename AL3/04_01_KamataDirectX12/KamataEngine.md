# KAMATA ENGINE 基础

KAMATA ENGINE 是学校课程使用的游戏引擎，内部使用 DirectX12。

```cpp
// 引入 KAMATA ENGINE 基础功能
#include "KamataEngine.h"
```

KAMATA ENGINE 的许多功能都放在 `KamataEngine` 这个命名空间中，因此使用这些功能时，通常需要写成 `KamataEngine::函数名()` 的形式。

这里的 `KamataEngine::` 表示“KamataEngine 命名空间里的内容”，用于区分这个函数来自哪个库。

例如：

```cpp
KamataEngine::Initialize();
KamataEngine::Update();
KamataEngine::Finalize();
```

---

## using namespace

每次都写 `KamataEngine::` 会比较麻烦，因此可以使用 `using namespace KamataEngine;` 来省略 `KamataEngine::` 这个命名空间前缀。

例如：

```cpp
KamataEngine::Update(); // 不使用 using namespace 时
Update();               // 使用 using namespace 后
```

---

## 注意点

`using namespace` 虽然方便，但不要乱用。  
如果同时省略多个命名空间，可能会出现同名函数冲突，导致编译器不知道应该调用哪一个。

另外，不要在 `.h` 头文件里写 `using namespace`，否则会影响到所有包含这个头文件的源文件，容易造成命名污染和类名冲突。

通常建议：

- `.cpp` 文件中可以根据需要使用
- `.h` 文件中尽量不要使用
- 核心函数或容易混淆的地方，可以保留 `KamataEngine::`，让代码来源更清楚

---

## DirectXCommon

`DirectXCommon::` 和 `KamataEngine::` 不完全一样。

`KamataEngine::` 通常表示命名空间前缀，而 `DirectXCommon::GetInstance()` 中的 `DirectXCommon` 是类名，表示调用 `DirectXCommon` 类里的函数。

```cpp
DirectXCommon* dxCommon = DirectXCommon::GetInstance();
```

这句代码表示取得 DirectXCommon 的实例，用来进行绘图开始、绘图结束等底层处理。


和之前使用的 Novice 版 KamataEngine 不同，DX12 版本不会提前帮我们写好：

- include
- 程序入口
- 键盘读取

等前置代码。

因此这些内容需要我们自己编写，并作为之后开发使用的基础模板。

<br>

## 1. 入口与出口 

和 `main()` 作为控制台程序入口一样，`WinMain()` 是 Windows 窗口程序的入口函数，也是游戏程序最基本的开始位置。

```cpp
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    return 0;
}
```

- HINSTANCE等参数是 Windows 窗口程序固定入口写法

---

## 2. 基本流程

游戏程序的基本流程和之前学习的一样：

```text
初始化
↓
更新循环
↓
结束释放
```

---

### 2.1. 初始化

使用 `Initialize()` 启动引擎。  
内部会自动创建窗口，并完成各种初始化操作。

```cpp
KamataEngine::Initialize(L"GC2C_02_ゴ_チュウ");
```

- `L""`：引号里用于设置窗口标题名字

---

### 2.2. 主循环

```cpp
while (true) {

    // 更新处理

    // 引擎自更新 + 自检退出
    if (KamataEngine::Update()) {
        break;
    }
}
```

- 关于退出主循环

    `Update()` 是引擎每帧执行的更新函数，除了更新窗口、输入等引擎状态外，还会自动检测窗口是否被关闭（例如点击右上角 × 或 Alt + F4）；当检测到窗口关闭时会返回 `true`，从而通过 `break` 退出主循环
    
    当然开发者也可以自己额外添加例如 ESC 等其它退出条件。

---

### 2.3. 释放

使用 `Finalize()` 结束引擎，并释放窗口和各种资源。

```cpp
KamataEngine::Finalize();
```

---

## 4. DirectXCommon

KAMATA ENGINE 内部实际上仍然使用 DirectX12 进行绘图。  
`DirectXCommon` 是负责管理 DirectX 绘图处理的类。

```cpp
// 获取 DirectXCommon
DirectXCommon* dxCommon =
    DirectXCommon::GetInstance();
```

主要负责：

- GPU控制
- 绘图开始
- 绘图结束

等处理。

---

## 5. 画面绘制（描画）

游戏中的绘图处理需要写在：

```cpp
PreDraw();
↓
绘图代码
↓
PostDraw();
```

之间。

```cpp
// 主循环
while (true) {

    // 引擎更新
    if (KamataEngine::Update()) {
        break;
    }

    // 开始绘图
    dxCommon->PreDraw();

    // 在这里写绘图代码

    // 结束绘图
    dxCommon->PostDraw();
}
```

---

### 5.1. PreDraw()

`PreDraw()` 用于开始绘图。

内部会自动：

- 清空画面
- 设置绘图目标
- 准备 GPU

等。

```cpp
dxCommon->PreDraw();
```

---

### 5.2. PostDraw()

`PostDraw()` 用于结束绘图，并真正把结果显示到屏幕。

```cpp
dxCommon->PostDraw();
```

---

## 6. GameScene 类

以前：

```text
所有代码全部写在 main.cpp
```

现在会把：

```text
游戏内容
```

单独拆分到类中管理。

这叫：

```text
Scene（场景）设计
```

例如：

- 标题场景
- 游戏场景
- 通关场景
- GameOver场景

等。

---

### 6.1. GameScene.h

```cpp
#pragma once

// 引入引擎
#include "KamataEngine.h"

// 游戏场景类
class GameScene
{

};
```

---

### 6.2. GameScene.cpp

```cpp
// 引入头文件
#include "GameScene.h"

// 省略 namespace
using namespace KamataEngine;
```

---

## 7. 添加基本函数

游戏类通常都会有：

- Initialize()
- Update()
- Draw()

三个基本函数。

```cpp
#pragma once

#include "KamataEngine.h"

// 游戏场景类
class GameScene
{
public:

    // 初始化
    void Initialize();

    // 更新
    void Update();

    // 绘图
    void Draw();
};
```

---

## 8. Initialize（初始化）

`Initialize()` 只会执行一次，通常用于：

- 变量初始化
- 图片读取
- 音频读取
- 游戏开始准备

等。

```cpp
#include "GameScene.h"

using namespace KamataEngine;

// 初始化
void GameScene::Initialize() {

    // 变量初始化

    // 图片读取

    // 游戏开始准备
}
```

---

## 9. Update（更新）

`Update()` 每帧执行一次，负责：

- 输入处理
- 玩家移动
- 敌人AI
- 碰撞判定
- 游戏逻辑更新

等。

```cpp
// 更新
void GameScene::Update() {

    // 输入处理

    // 移动处理

    // 碰撞判定

    // 游戏逻辑更新
}
```

---

## 10. Draw（绘图）

`Draw()` 只负责画面绘制。

游戏开发中：

```text
更新
```

和：

```text
绘图
```

必须分开。

```cpp
// 绘图
void GameScene::Draw() {

    // Sprite绘制

    // 模型绘制

    // UI绘制
}
```

---

## 11. 创建 GameScene 实例

使用 `new` 可以在内存中创建对象。

```cpp
// 引入 GameScene
#include "GameScene.h"
```

```cpp
// 创建 GameScene
GameScene* gameScene =
    new GameScene();

// 初始化
gameScene->Initialize();
```

---

## 12. Update 和 Draw 调用

主循环中：

- Update() 负责更新
- Draw() 负责绘图

```cpp
// 主循环
while (true) {

    // 引擎更新
    if (KamataEngine::Update()) {
        break;
    }

    // 游戏更新
    gameScene->Update();

    // 绘图开始
    dxCommon->PreDraw();

    // 游戏绘图
    gameScene->Draw();

    // 绘图结束
    dxCommon->PostDraw();
}
```

---

## 13. delete（释放）

使用 `new` 创建的对象不会自动释放，因此必须使用 `delete` 手动释放。

```cpp
// 删除 GameScene
delete gameScene;

// 赋值 nullptr
gameScene = nullptr;
```

`nullptr` 表示：

```text
这个指针已经无效
```

---

## 14. 最终整体结构

```cpp
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    // 引擎初始化
    KamataEngine::Initialize();

    // 获取 DirectXCommon
    DirectXCommon* dxCommon =
        DirectXCommon::GetInstance();

    // 创建 GameScene
    GameScene* gameScene =
        new GameScene();

    // 初始化
    gameScene->Initialize();

    // 主循环
    while (true) {

        // 引擎更新
        if (KamataEngine::Update()) {
            break;
        }

        // 游戏更新
        gameScene->Update();

        // 开始绘图
        dxCommon->PreDraw();

        // 游戏绘图
        gameScene->Draw();

        // 结束绘图
        dxCommon->PostDraw();
    }

    // 删除 GameScene
    delete gameScene;

    // nullptr
    gameScene = nullptr;

    // 引擎结束
    KamataEngine::Finalize();

    return 0;
}
```

---

## 15. 本章重点

本章主要学习：

- KAMATA ENGINE 基础结构
- WinMain
- 初始化
- 更新
- 绘图
- Scene设计
- new / delete
- Draw 与 Update 分离

---

## 16. 作业内容

1. 修改标题栏为自己的 Google 名字
2. 添加指定 Tag 并 Push 到 GitHub
3. 标记作业完成