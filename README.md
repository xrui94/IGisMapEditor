# IGisMapEditor

![screenshot_01.png](./imgs/screenshot_01.png)

## 一、 项目简介

IGisMapEditor 是一个基于 Microsoft Foundation Classes (MFC) 框架开发的桌面端地理信息系统（GIS）地图编辑器应用程序。该软件提供了地图绘制、编辑、空间分析等基础 GIS 功能，适用于中小型地理信息应用场景。

**📖 详细设计说明文档：** 请查看 [IGisMapEditor_Design.md](./IGisMapEditor_Design.md) 了解项目的主要功能、技术架构、核心模块和使用说明。

## 二、 项目来源

本项目是从原作者 [ndsorrowchi](https://github.com/ndsorrowchi) 的 GitHub 仓库 [IGisMapEditor](https://github.com/ndsorrowchi/IGisMapEditor) fork 而来。原始项目使用 Visual Studio 2010 开发，最后更新于 7 年前。

## 三、 项目结构

```
IGisMapEditor/
├── IGisMapEditor/           # 主项目目录
│   ├── res/                 # 资源文件（图标、位图等）
│   ├── *.h                  # 头文件
│   ├── *.cpp                # 源代码文件
│   └── IGisMapEditor.vcxproj # Visual Studio 项目文件
├── Debug/                   # 调试版本输出目录
├── Release/                 # 发布版本输出目录
├── IGisMapEditor.sln        # Visual Studio 解决方案文件
├── README.md                # 项目说明文档
├── IGisMapEditor_Design.md  # 详细设计说明文档
└── imgs/                    # 项目截图
```

## 四、 编译构建说明

### 系统要求
- Windows 10/11 操作系统
- Visual Studio 2022 (Community 或更高版本)
- SQL Server 数据库（用于存储地理数据）

### 编译步骤
1. 使用 Visual Studio 2022 打开 `IGisMapEditor.sln` 解决方案文件
2. 选择配置：Debug 或 Release
3. 选择平台：x64（推荐） 或 Win32
4. 点击"生成" → "生成解决方案"
5. 等待编译完成

### 注意事项
- ⚠️ 编译时**暂不要**添加 `/utf-8` 编译选项（原因见下文）
- ⚠️ 确保已安装 MFC 组件（Visual Studio 安装时选择"使用 C++ 的桌面开发"工作负载）
- ⚠️ 需要配置 SQL Server 数据库连接信息才能完整使用软件功能

## 五、 编译环境升级与修改说明

本版本对原始项目的编译环境进行了全面升级，并修复了相关的编译错误。

### 环境升级

1. **平台工具集升级**
   - 原版本：Visual Studio 2010 (v100)
   - 新版本：Visual Studio 2022 (v143)

2. **Windows SDK 升级**
   - 原版本：Windows 7 SDK
   - 新版本：10.0.22621.0 (Windows 11 SDK)

3. **C++ 标准升级**
   - 原版本：C++98/03
   - 新版本：C++17

4. **目标程序架构升级**
   - 原版本：Win32
   - 新版本：x64

### 编译错误修复

#### 1. abs 函数调用不明确问题

**问题描述：**
在 `AlterBaseTool.cpp` 和 `Cordins.cpp` 中，使用 `abs()` 函数处理浮点型参数时出现"重载函数调用不明确"的编译错误。

**原因分析：**
在 C++17 标准中，`<cmath>` 头文件提供了多个 `abs` 函数重载版本（`abs(int)`、`abs(double)`、`abs(float)` 等），当参数类型不明确时，编译器无法确定应该调用哪个版本。

**修复方案：**
将浮点型参数的 `abs()` 函数调用替换为 `fabs()` 函数：
```cpp
// 修复前
abs(this->gp.point.x - this->tempgp.point.x)

// 修复后
fabs(this->gp.point.x - this->tempgp.point.x)
```

**修改文件：**
- `IGisMapEditor/AlterBaseTool.cpp` (第 197 行)
- `IGisMapEditor/Cordins.cpp` (第 113、117 行)

#### 2. 赋值运算符缺少 const 修饰符问题

**问题描述：**
在 `treelayers.h` 中，`Layer` 和 `Basepic` 类的赋值运算符参数类型缺少 `const` 修饰符，导致 `std::list` 容器在执行某些操作时出现编译错误。

**原因分析：**
在 C++17 中，标准库容器（如 `std::list`）的某些内部操作需要通过 `const` 引用进行赋值操作。如果赋值运算符不接受 `const` 引用参数，容器将无法正确执行这些操作。

**修复方案：**
为赋值运算符的参数添加 `const` 修饰符：
```cpp
// 修复前
Layer & operator =(Layer &l) { ... }
Basepic & operator =(Basepic &l) { ... }

// 修复后
Layer & operator =(const Layer &l) { ... }
Basepic & operator =(const Basepic &l) { ... }
```

**修改文件：**
- `IGisMapEditor/treelayers.h` (第 33 行 - Layer 类)
- `IGisMapEditor/treelayers.h` (第 92 行 - Basepic 类)

#### 3. 编码格式问题

**问题描述：**
添加 `/utf-8` 编译选项后，出现大量语法错误和未知字符错误。

**原因分析：**
原始项目的源代码文件使用 GB2312 编码（中文注释和字符串），但编译器被强制按 UTF-8 解析，导致中文字符串变成乱码，进而引发语法错误。

**修复方案：**
移除 `/utf-8` 编译选项，让编译器按系统默认编码（GB2312）解析源文件。

**重要提示：**
- ⚠️ **当前配置**：源文件使用 GB2312 编码，编译时**切勿**添加 `/utf-8` 编译选项
- 💡 **未来建议**：建议将所有代码文件统一转换为 UTF-8 编码，然后添加 `/utf-8` 编译选项，以提高代码的跨平台兼容性

**修改文件：**
- `IGisMapEditor/IGisMapEditor.vcxproj` (移除第 58 行的 `/utf-8` 编译选项)

## 六、已知问题与限制

1. **数据格式专有**：*.igp 格式为自定义格式，不兼容其他 GIS 软件
2. **无空间索引**：未使用 SQL Server 原生空间数据类型，查询效率较低
3. **编码问题**：源文件使用 GB2312 编码，需要特别注意编译选项
4. **互操作性差**：无法直接与其他 GIS 软件交换数据
5. **功能有限**：仅提供基础的 GIS 功能，不适合复杂的空间分析

## 七、未来改进方向

1. **迁移到 UTF-8 编码**：统一使用 UTF-8 编码，提高跨平台兼容性
2. **使用原生空间数据类型**：采用 SQL Server 的 `geography`/`geometry` 类型
3. **添加空间索引**：提高空间查询性能
4. **支持标准格式**：增加对 Shapefile、GeoJSON 等标准格式的支持
5. **增强空间分析**：添加更多空间分析功能
6. **改进用户界面**：现代化 UI 设计，提升用户体验

## 八、 许可证

本项目遵循原始项目的许可证。请查看原始仓库 [ndsorrowchi/IGisMapEditor](https://github.com/ndsorrowchi/IGisMapEditor) 了解详细信息。

## 九、 贡献

欢迎提交 Issue 和 Pull Request 来改进这个项目！

## 十、 致谢

感谢原作者 [ndsorrowchi](https://github.com/ndsorrowchi) 创建了这个项目。
