# GameByCpp —— EasyX 小游戏练习集

基于 **EasyX 图形库** 的 C++ 小游戏 / 图形练习集合，目前包含「井字棋」和「跟随鼠标移动的小球」两个独立的 Visual Studio 工程。GitHub 仓库名为 **GameByCpp**（本地文件夹名为 `[EasyX]井字棋`），远程地址：<https://github.com/A-lioth/GameByCpp>。

## 技术栈

| 项目 | 说明 |
| --- | --- |
| 语言 | C++（源码为单文件 `test.cpp`，入口 `main`） |
| 图形库 | EasyX（源码中通过 `#include <graphics.h>` 引入） |
| IDE / 构建 | Visual Studio 2022 解决方案（`.sln`，`VisualStudioVersion = 17.9.34902.65`） |
| 平台工具集 | `v143`（VS2022 的 MSVC 工具集，两个工程四种配置均为 `v143`） |
| 工程版本 | `VCProjectVersion` = `17.0`，`WindowsTargetPlatformVersion` = `10.0` |
| 字符集 | `Unicode`（代码使用 `TCHAR` / `_T()` / `_stprintf_s`） |
| 配置 | `Debug` / `Release` × `Win32` / `x64`（共 4 种，解决方案里 `x86` 映射到 `Win32`） |
| 链接子系统 | `Console`（`<SubSystem>Console</SubSystem>`，四个配置一致） |
| 目标平台 | Windows（EasyX 仅支持 Windows + MSVC） |

## 目录结构

```
GameByCpp/                     # 本地目录名：[EasyX]井字棋
├── .gitignore                 # 标准的 VisualStudio.gitignore 模板
├── 井字棋/
│   └── test/
│       ├── test.sln           # 解决方案（VS2022，Format Version 12.00）
│       ├── test.vcxproj       # C++ 控制台应用程序工程
│       ├── test.vcxproj.filters
│       └── test.cpp           # 井字棋全部逻辑（152 行）
└── 跟随鼠标移动的小球/
    └── test/
        ├── test.sln           # 解决方案（VS2022，Format Version 12.00）
        ├── test.vcxproj       # C++ 控制台应用程序工程
        ├── test.vcxproj.filters
        └── test.cpp           # 小球跟随鼠标（29 行）
```

两个子项目互相独立，各自拥有独立的 `.sln` 与 `.vcxproj`，没有共享代码或公共库；仓库根目录本身不是解决方案。

## 主要功能 / 模块说明

### 一、井字棋（`井字棋/test/test.cpp`）

3×3 双人对战井字棋，鼠标点击落子，窗口 600×600。

**数据结构**

- `char board[3][3]`：棋盘状态，`'-'` 表示空位，`'X'` / `'O'` 表示已落子。
- `char currentPlayer`：当前执子方，初始为 `'X'`，每次成功落子后切换。

**函数划分**

| 函数 | 职责 |
| --- | --- |
| `checkWin(char c)` | 判断字符 `c` 是否连成一线：逐条检查 3 行、3 列、主对角线、副对角线共 8 种情况。 |
| `checkDraw()` | 遍历全部 9 格，若无 `'-'` 空位则判定为平局。 |
| `drawBoard()` | 用 `line()` 画 4 条线（横线 y=200/400、竖线 x=200/400），把 600×600 窗口均分为 3×3 棋盘格。 |
| `drawPiece()` | 双层循环遍历棋盘：`'X'` 用两条交叉 `line()` 画在格子内，`'O'` 用 `circle()`（半径 100，格子中心）绘制，空位不画。 |
| `drawTips()` | 用 `settextcolor(YELLOW)` + `outtextxy(0, 0, ...)` 在左上角显示 `Player X's turn` / `Player O's turn` 提示文字。 |

**主循环（`main`）**

1. `initgraph(600, 600)` 创建绘图窗口，`BeginBatchDraw()` 开启批量绘制。
2. 用 EasyX 的 `ExMessage` + `peekmessage()` 处理消息，仅响应 `WM_LBUTTONDOWN`：把鼠标坐标除以 200 得到格子下标 `index_x = x / 200`、`index_y = y / 200`，若该格为空则落子并切换玩家；已占用的格子点击无效。
3. 每帧 `cleardevice()` 清屏后依次调用 `drawBoard()`、`drawPiece()`、`drawTips()`，再 `FlushBatchDraw()` 提交。
4. 每帧调用 `checkWin('X')` / `checkWin('O')` / `checkDraw()`，命中则用 `MessageBox(GetHWnd(), ...)` 弹出 `X wins!` / `O wins!` / `Draw!` 并置 `run = false` 退出。
5. 用 `GetTickCount()` 计算帧耗时，不足 `1000/60` 毫秒时 `Sleep()` 补齐，把循环限制在约 60 FPS。
6. 退出时 `EndBatchDraw()`、`closegraph()`，返回 0。

### 二、跟随鼠标移动的小球（`跟随鼠标移动的小球/test/test.cpp`）

窗口 1280×720 的实时图形演示，无游戏逻辑，只验证 EasyX 的消息循环与批量绘制。

- `initgraph(1280, 720)` 创建窗口，`BeginBatchDraw()` 开启批量绘制。
- 球心坐标 `x = 300`、`y = 300` 作为初始位置。
- 主循环中 `peekmessage()` 读取消息，收到 `WM_MOUSEMOVE` 时把球心更新为鼠标坐标 `msg.x` / `msg.y`。
- 每帧 `cleardevice()` 清屏 → `solidcircle(x, y, 100)` 画实心圆（半径 100）→ `FlushBatchDraw()` 提交，形成小球跟随鼠标的效果。
- 循环条件为 `while (true)`，只能通过关闭窗口结束进程（代码末尾的 `EndBatchDraw()` / `closegraph()` 在无限循环之后，正常流程中不会被执行）。

## 运行 / 构建方式

1. **安装 EasyX 图形库（前置条件）**
   两个工程的 `.vcxproj` 中**没有**配置 `AdditionalIncludeDirectories`、`AdditionalLibraryDirectories` 或 `AdditionalDependencies`，源码里也没有 `#pragma comment(lib, ...)`。也就是说，工程文件本身不引用 EasyX 的任何路径，完全依赖 **EasyX 安装器**（<https://easyx.cn>）把 `graphics.h` 与对应的 `.lib` 安装到 Visual Studio 的 VC 包含目录 / 库目录中。因此编译前必须先安装 EasyX，并确认安装时选择了当前使用的 VS 版本与工具集（v143）。
2. **打开解决方案**：用 Visual Studio 2022 打开对应子项目的 `.sln`：
   - 井字棋：`井字棋/test/test.sln`
   - 小球：`跟随鼠标移动的小球/test/test.sln`
   注意根目录下没有 `.sln`，两个子项目需分别打开。
3. **选择配置**：在工具栏选择 `Debug`/`Release` 与 `x64`/`x86`（工程内部平台名为 `Win32`/`x64`）。
4. **编译运行**：`Ctrl + Shift + B` 生成，`F5` 或 `Ctrl + F5` 运行（链接子系统为 Console，运行时可能同时出现控制台窗口，属正常现象）。
5. 若编译报找不到 `graphics.h`，说明 EasyX 未安装或安装的版本与 v143 工具集不匹配，重装 EasyX 即可（工程文件无需改动）。

> 本仓库未附带 EasyX 的头文件或库文件，也未提供 CMake / 命令行构建脚本，构建完全依赖 Visual Studio 工程。

## 备注

- **练习性质**：仓库是 EasyX 学习过程中的练手代码集合，每个子项目都是单文件、无外部依赖（除 EasyX）的小程序，重点在消息循环、绘图 API、批量绘制与简单游戏逻辑，未做工程化拆分（无头文件、无资源文件、无单元测试）。
- **仅 Windows 平台**：EasyX 依赖 Windows GDI 与 MSVC 工具链，两个工程也只配置了 `Win32` / `x64` 的 MSVC 目标，无法在 Linux / macOS 或非 MSVC 编译器下直接构建。
- **仓库演进历史**（`git log` 共 6 次提交，`master` 分支，远程 `origin` 指向 GameByCpp）：
  - `b60e621`（2024-04-27）添加 `.gitattributes` 和 `.gitignore`
  - `0d68035`（2024-04-27）添加项目文件
  - `40cf4de`（2024-07-31）fix: 重新整理仓库内容
  - `3899970`（2024-07-31）init: 创建了"跟随鼠标移动的小球"项目文件，并初步实现了相关功能
  - `fc87c28`（2024-07-31）feat: 完善了代码内容
  - `2abad89`（2024-07-31）feat: 实现井字棋的棋盘绘制、棋子绘制、输赢判断、信息提示等功能
  即：先有仓库骨架与"小球"演示，再在同一批提交中把井字棋从雏形完善为可玩的完整功能。
- **`.gitignore`** 使用 GitHub 官方的 VisualStudio 模板，忽略 `Debug/`、`Release/`、`x64/`、`x86/`、`.vs/`、`*.user`、`*.obj`、`*.pdb` 等生成物，因此编译输出不会进入版本库。
- 根目录存在一个本地文件 `[EasyX]井字棋.vcxproj.user`（内容为空 `<PropertyGroup />`），已被 `.gitignore` 的 `*.user` 规则忽略，不属于版本库内容，可忽略。
- 未在本仓库中找到 LICENSE、CI 配置或第三方依赖清单，相关说明待补充。
