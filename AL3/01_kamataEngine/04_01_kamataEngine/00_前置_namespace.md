# namespace

因为不同库、类或功能中，可能会出现同名函数或变量。  
所以会在前面使用 `::`，告诉编译器：

“**这个函数或内容属于谁**”。

这叫做：`スコープ解決演算子 / 作用域解析运算符`

```cpp
KamataEngine::Update(); // Update 属于 KamataEngine

Enemy::Update();        // Update 属于 Enemy 类

std::cout;              // cout 属于 std 标准库
```

---

## using namespace

虽然 `::` 解决了同名问题，但是如果经常使用某个库、引擎或 namespace，  
每次都写来源会比较麻烦。

因此可以设置：

`using namespace 来源名;`

这样在使用某个库、引擎或 namespace 中的函数时，可以省略前面的来源名。

```cpp
using namespace KamataEngine;

Update(); // 省略 KamataEngine::
```

但和同名问题一样，如果写了多个 `using namespace`，并且这些来源中存在同名函数或变量，编译器也可能因为不知道要使用哪一个而报错。