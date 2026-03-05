# IGisMapEditor 设计说明文档

本文档详细描述了 IGisMapEditor 项目的主要功能、技术架构、核心模块和使用说明。

## 目录

1. [主要功能](#1-主要功能)
2. [技术架构](#2-技术架构)
3. [核心模块说明](#3-核心模块说明)
4. [使用说明](#4-使用说明)

---

## 1. 主要功能

### 1.1 地图编辑功能

#### 矢量要素绘制
- ✏️ **点要素绘制**：创建点状地理要素（如地标、观测点等）
- ✏️ **线要素绘制**：创建线状地理要素（如道路、河流等）
- ✏️ **面要素绘制**：创建面状地理要素（如区域、建筑物等）
- ✏️ **自定义样式**：支持自定义颜色、线型、填充样式、大小等属性

#### 图形编辑
- ✏️ **几何属性编辑**：修改已有图形的坐标、形状等几何信息
- ✏️ **样式属性编辑**：修改已有图形的颜色、大小、线型等样式
- ✏️ **点操作**：添加、删除、移动线段和多边形的顶点
- ✏️ **图形剪断**：对线段进行剪断操作

#### 文本注记
- ✏️ **文本标记**：在地图上添加文本标签
- ✏️ **字体样式**：自定义字体、大小、颜色等属性
- ✏️ **注记定位**：精确定位注记位置

### 1.2 数据库管理

#### SQL Server 集成
- 🗄️ **数据库连接**：支持连接到 SQL Server 2000/2005/2008/2012/2014/2016/2017/2019/2022
- 🗄️ **连接管理**：创建、断开数据库连接
- 🗄️ **连接参数**：服务器地址、数据库名称、用户名、密码

#### 数据表管理
- 🗄️ **表创建**：创建点、线、面、标记等地理数据表
- 🗄️ **表删除**：删除不再需要的地理数据表
- 🗄️ **表查询**：查询数据库中的所有地理数据表
- 🗄️ **表信息**：获取表的详细信息（类型、记录数等）

#### 数据持久化
- 🗄️ **数据插入**：将新绘制的地理要素保存到数据库
- 🗄️ **数据更新**：修改已有地理要素的属性
- 🗄️ **数据删除**：删除指定的地理要素
- 🗄️ **批量操作**：支持批量读取和写入数据

### 1.3 空间分析功能

#### 叠置分析
- 📊 **线与多边形叠置分析 (LineDjPolygon)**：
  - 分析线要素与多边形要素的空间关系
  - 计算线段在多边形内的部分
  - 生成叠置结果

- 📊 **多边形与多边形叠置分析 (PolyDjPoly)**：
  - 分析两个多边形图层之间的空间关系
  - 计算多边形的交集、并集、差集
  - 生成叠置结果表

#### 缓冲区分析
- 📊 **点缓冲区分析**：为点要素创建指定半径的缓冲区
- 📊 **线缓冲区分析**：为线要素创建指定距离的缓冲区
- 📊 **面缓冲区分析**：为面要素创建指定距离的缓冲区

### 1.4 地图浏览功能

#### 地图缩放
- 🔍 **放大**：放大地图显示，查看更详细的区域
- 🔍 **缩小**：缩小地图显示，查看更大的范围
- 🔍 **平移**：拖动地图，查看不同区域
- 🔍 **自由缩放**：支持鼠标滚轮缩放

#### 坐标显示
- 🔍 **实时坐标**：实时显示鼠标位置的地理坐标
- 🔍 **坐标格式**：支持多种坐标格式显示
- 🔍 **坐标转换**：自动进行坐标转换

#### 底图支持
- 🔍 **影像底图**：支持加载影像底图（如卫星图像、航拍图像）
- 🔍 **底图配准**：为底图设置地理坐标范围
- 🔍 **底图显示**：控制底图的可见性和显示顺序

#### 坐标转换
- 🔍 **地理坐标转屏幕坐标**：将地理坐标转换为屏幕像素坐标
- 🔍 **屏幕坐标转地理坐标**：将屏幕像素坐标转换为地理坐标
- 🔍 **坐标系统管理**：支持不同的坐标系统

### 1.5 图层管理

#### 图层操作
- 📂 **添加图层**：从数据库添加新的图层到项目
- 📂 **删除图层**：从项目中删除图层
- 📂 **排序图层**：调整图层的显示顺序
- 📂 **图层重命名**：修改图层名称

#### 可见性控制
- 📂 **显示/隐藏**：控制单个图层的显示状态
- 📂 **批量控制**：同时控制多个图层的显示状态
- 📂 **图层树**：在左侧图层树中直观管理图层

#### 编辑状态管理
- 📂 **可编辑状态**：设置图层是否可编辑
- 📂 **当前编辑图层**：指定当前正在编辑的图层
- 📂 **状态提示**：在图层树中显示图层的编辑状态

#### 多图层叠加
- 📂 **叠加显示**：多个图层同时显示在地图上
- 📂 **透明度控制**：控制图层的透明度
- 📂 **显示顺序**：调整图层的叠加顺序

### 1.6 空间查询

#### 矩形范围查询
- 🔎 **矩形选择**：用矩形框选择指定范围内的要素
- 🔎 **多类型查询**：同时查询点、线、面、标记等要素
- 🔎 **结果显示**：在列表中显示查询结果

#### 圆形范围查询
- 🔎 **圆形选择**：用圆形区域选择指定范围内的要素
- 🔎 **半径设置**：自定义查询圆的半径
- 🔎 **中心点定位**：指定查询圆的中心点

#### 多边形范围查询
- 🔎 **多边形选择**：用任意多边形选择指定范围内的要素
- 🔎 **复杂形状**：支持复杂的多边形形状
- 🔎 **精确查询**：精确控制查询范围

#### 多种要素类型支持
- 🔎 **点要素查询**：查询指定范围内的点要素
- 🔎 **线要素查询**：查询指定范围内的线要素
- 🔎 **面要素查询**：查询指定范围内的面要素
- 🔎 **标记查询**：查询指定范围内的文本标记

---

## 2. 技术架构

### 2.1 开发框架

#### MFC (Microsoft Foundation Classes)
- **框架类型**：Windows 桌面应用程序框架
- **架构模式**：文档-视图架构 (Document-View Architecture)
- **界面类型**：SDI (Single Document Interface) 单文档界面
- **主要特点**：
  - 提供丰富的 Windows 控件和对话框
  - 支持消息处理机制
  - 提供文档序列化功能
  - 支持打印和打印预览

#### SDI 架构设计
- **文档类 (CIGisMapEditorDoc)**：
  - 管理地图项目的数据
  - 处理文件的打开、保存、另存为操作
  - 管理数据库连接信息
  - 协调各个视图的数据显示

- **视图类 (CIGisMapEditorView)**：
  - 负责地图的显示和绘制
  - 处理鼠标和键盘事件
  - 实现地图的缩放、平移等操作
  - 响应用户的绘图和编辑操作

- **主框架类 (CMainFrame)**：
  - 管理应用程序的主窗口
  - 包含工具栏、状态栏、菜单栏
  - 管理停靠窗口（如图层树）
  - 协调各个子窗口的交互

#### 模块化设计
- **功能分离**：每个功能模块独立设计，职责明确
- **接口清晰**：模块之间通过清晰的接口进行通信
- **易于扩展**：可以方便地添加新的功能模块
- **代码复用**：提供基类和工具类，支持代码复用

### 2.2 编程语言

#### C++
- **主要开发语言**：使用 C++ 进行所有开发工作
- **面向对象**：充分利用 C++ 的面向对象特性
- **标准库**：使用 C++ 标准库（STL）进行数据结构管理
- **模板技术**：使用模板实现通用的算法和数据结构

#### C++17 标准
- **现代 C++ 特性**：
  - 使用 `std::list` 等标准容器
  - 使用智能指针管理资源
  - 使用 lambda 表达式简化代码
  - 使用 `constexpr` 和 `nullptr` 等新特性

- **类型安全**：
  - 强类型检查，减少运行时错误
  - 使用 `const` 和 `constexpr` 提高代码安全性
  - 使用 `auto` 关键字简化类型声明

### 2.3 数据存储

#### SQL Server
- **数据库版本**：支持 SQL Server 2000 及以上版本
- **数据库角色**：作为后端数据存储服务器
- **连接方式**：使用 ADO (ActiveX Data Objects) 技术连接
- **数据类型**：
  - 使用基本数据类型（int, float, nvarchar 等）
  - 使用 image 类型存储二进制数据（点集、字体等）

#### ADO 技术
- **连接对象**：管理数据库连接
- **记录集对象**：执行 SQL 查询并获取结果
- **命令对象**：执行 SQL 命令
- **错误处理**：捕获和处理数据库错误

#### 自定义数据结构
- **不使用原生空间数据类型**：
  - 未使用 SQL Server 的 `geography` 类型
  - 未使用 SQL Server 的 `geometry` 类型
  - 使用传统的关系型表结构

- **数据存储方式**：
  - 点要素：直接存储 X、Y 坐标
  - 线要素：存储点集（二进制）和边界框
  - 面要素：存储点集（二进制）和边界框
  - 标记要素：存储坐标、文本和字体样式

### 2.4 数据格式

#### 自定义项目格式：*.igp

**格式说明：**
`*.igp` (IGis Project) 是本项目自定义的专有项目文件格式，用于保存地图项目的配置信息。

**文件结构：**
```
底图数量 (整数)
├─ 底图1信息
│  ├─ 文件路径/名称
│  ├─ 左上角坐标 (ltx, lty)
│  └─ 右下角坐标 (rbx, rby)
├─ 底图2信息
│  ├─ 文件路径/名称
│  ├─ 左上角坐标 (ltx, lty)
│  └─ 右下角坐标 (rbx, rby)
├─ ...
图层数量 (整数)
├─ 数据库服务器地址
├─ 数据库名称
└─ 图层信息
   ├─ 图层1名称
   ├─ 图层2名称
   └─ ...
```

**格式特点：**
- **纯文本格式**：使用文本文件存储，易于阅读和手动编辑
- **配置信息**：只保存项目配置，不保存实际地理数据
- **数据分离**：实际地理数据存储在 SQL Server 数据库中
- **依赖性**：依赖特定的数据库结构和连接信息
- **互操作性**：互操作性较低，只能被本软件使用

**文件示例：**
```
2
D:\Maps\beijing_satellite.jpg
116.0 40.0 117.0 41.0
D:\Maps\beijing_road.jpg
116.2 40.2 116.8 40.8
3
localhost
GISDatabase
points_layer
lines_layer
polygons_layer
```

#### SQL Server 数据存储

**存储方式说明：**
本项目使用传统的关系型表结构存储地理数据，而非 SQL Server 的原生空间数据类型（`geography`/`geometry`）。

**数据表结构：**

**点图层表 (Type 1)：**
```sql
CREATE TABLE [表名] (
    ID int primary key,           -- 要素唯一标识
    pX float,                     -- X坐标
    pY float,                     -- Y坐标
    color int,                    -- 颜色（RGB值）
    size int,                     -- 点大小
    type nvarchar(50)             -- 点类型
)
```

**线图层表 (Type 2)：**
```sql
CREATE TABLE [表名] (
    ID int primary key,           -- 要素唯一标识
    LColor int,                   -- 线颜色（RGB值）
    LType nvarchar(50),           -- 线类型（实线、虚线等）
    LWidth int,                   -- 线宽度
    xMin float,                   -- 边界框最小X
    yMin float,                   -- 边界框最小Y
    xMax float,                   -- 边界框最大X
    yMax float,                   -- 边界框最大Y
    Point image,                  -- 点集（二进制存储）
    pCount int                    -- 点数量
)
```

**多边形表 (Type 3)：**
```sql
CREATE TABLE [表名] (
    ID int primary key,           -- 要素唯一标识
    pCount int,                   -- 点数量
    BorderType int,               -- 边界类型
    color int,                    -- 填充颜色（RGB值）
    Point image,                  -- 点集（二进制存储）
    BorderColor int,              -- 边界颜色（RGB值）
    xMin float,                   -- 边界框最小X
    yMin float,                   -- 边界框最小Y
    xMax float,                   -- 边界框最大X
    yMax float                    -- 边界框最大Y
)
```

**标记表 (Type 4)：**
```sql
CREATE TABLE [表名] (
    ID int primary key,           -- 要素唯一标识
    pX float,                     -- X坐标
    pY float,                     -- Y坐标
    Lable nvarchar(50),           -- 标签文本
    color int,                    -- 颜色（RGB值）
    FontStyle image               -- 字体样式（二进制存储）
)
```

**目录表 (CatalogTable)：**
```sql
CREATE TABLE [CatalogTable] (
    ID int primary key,           -- 表唯一标识
    TableName nvarchar(50),       -- 表名
    Type int                      -- 表类型（1-点，2-线，3-面，4-标记）
)
```

**存储方式优缺点：**

**优点：**
- 实现简单，易于理解
- 不依赖特定版本的 SQL Server
- 灵活性高，可以自定义各种属性
- 兼容性好，可以在任何支持 SQL 的数据库上运行

**缺点：**
- 无空间索引，查询效率低
- 无空间函数，无法使用 SQL Server 的空间分析功能
- 无空间参考系统，缺乏坐标系统支持
- 数据冗余，需要手动维护边界框等信息
- 互操作性差，无法与其他 GIS 软件直接交换数据

**与现代 GIS 数据库的对比：**

| 特性 | 本项目方式 | SQL Server 原生空间类型 |
|------|-----------|----------------------|
| 空间索引 | 无 | R-Tree 索引 |
| 空间函数 | 手动实现 | 内置丰富函数 |
| 坐标系统 | 无 | 支持 SRID |
| 查询性能 | 较低 | 高 |
| 互操作性 | 差 | 好 |
| 实现复杂度 | 简单 | 较复杂 |

---

## 3. 核心模块说明

### 3.1 数据库模块 (DataBase.h/cpp)

**模块功能：**
- 数据库连接管理
- SQL 语句执行
- 地理数据的增删改查
- 空间查询和分析

**主要类：**
- `DBConnection`：数据库连接类

**主要方法：**
```cpp
// 数据库连接
bool Open(CString server, CString dbname, CString uid, CString paw);
void disConnection();
bool isConnection();

// SQL 执行
_RecordsetPtr ExecuteSQL(CString sql);
_RecordsetPtr getRecordSet(CString table);

// 点要素操作
void addPoint(gisPoint& gp, CString table);
bool SearchPointByClick(CPoint point, gisPoint& gis_point, CString table, CZoomManager* pZoom);
void AlterPoint(gisPoint& gis_point, CString table, CDC* pDC, CZoomManager* pZoom);
void DeletePoint(gisPoint& gp, CString table);

// 线要素操作
void addLine(gisLine& gl, CString table);
bool SearchLineByClick(CPoint point, gisLine& gl, CString table, CZoomManager* pZoom);
void AlterLine(gisLine& gl, CString table, CDC* pDC, CZoomManager* pZoom);
void DeleteLine(gisLine& gl, CString table);

// 多边形要素操作
void addPolygon(gisPolygon& gp, CString table);
bool SearchPolyByClick(CPoint& point, gisPolygon& gPoly, CString table, CZoomManager* pZoom);
void DeletePolygon(gisPolygon& gPoly, CString table);
void AlterPolygon(gisPolygon& gPoly, CString table, CDC* pDC, CZoomManager* pZoom);

// 标记要素操作
void addMarker(gisMarker& gm, CString table);
bool SearchMarkerByClick(CPoint point, gisMarker& gm, CString table, CDC* pDC, CZoomManager* pZoom);
void AlterMarker(gisMarker& gm, CString table, CDC* pDC, CZoomManager* pZoom);
void DeleteMarker(gisMarker& gm, CString table);

// 表管理
bool CreateTable(CString table, int type);
bool DropTable(CString table);
bool SearchTable(CString table);
int GetDataBaseTable(CatalogTable* tableSet);

// 空间查询
void SearchByRect(CPoint p1, CPoint p2, CString table, gisPoint* gp, CZoomManager* pZoom, int& count);
void SearchByCircle(CPoint p1, CPoint p2, CString table, gisPoint* gp, CZoomManager* pZoom, int& count);
void SearchByPolygon(CPoint* point, int n, CString table, gisPoint* gp, CZoomManager* pZoom, int& count);

// 空间分析
bool LineDjPolygon(CString lineTable, CString PolyTable, CString ResultTable, CZoomManager* pZoom);
bool PolyDjPoly(CString PolyTable1, CString PolyTable2, CString ResultTable, CZoomManager* pZoom);
void HcAnalyse(gisPoint& gp);
```

### 3.2 图形数据结构 (DataTemplate.h)

**模块功能：**
- 定义地理要素的数据结构
- 提供图形属性的管理

**主要类：**

**gisPoint（点要素）：**
```cpp
struct gisPoint {
    DPoint point;       // 点坐标
    int color;          // 颜色（RGB值）
    int size;           // 点大小
    CString pType;      // 点类型
    int ID;             // 要素ID
};
```

**gisLine（线要素）：**
```cpp
struct gisLine {
    DPoint* LinePoint;  // 线段点集
    int pCount;         // 点数量
    int color;          // 线颜色（RGB值）
    CString lType;      // 线类型
    float width;        // 线宽度
    int ID;             // 要素ID
};
```

**gisPolygon（多边形要素）：**
```cpp
struct gisPolygon {
    DPoint* point;      // 多边形点集
    int pCount;         // 点数量
    int color;          // 填充颜色（RGB值）
    int BorderType;     // 边界类型
    int BorderColor;    // 边界颜色（RGB值）
    int ID;             // 要素ID
};
```

**gisMarker（标记要素）：**
```cpp
struct gisMarker {
    DPoint point;       // 标记位置
    CString text;       // 标记文本
    LOGFONT lfont;      // 字体样式
    int color;          // 颜色（RGB值）
    int ID;             // 要素ID
};
```

**CatalogTable（目录表）：**
```cpp
struct CatalogTable {
    CString name;       // 表名
    int type;           // 表类型
    int ID;             // 表ID
};
```

**DPoint（坐标点）：**
```cpp
struct DPoint {
    double x;           // X坐标
    double y;           // Y坐标
};
```

### 3.3 图形绘制工具 (GraphTool.h/cpp)

**模块功能：**
- 提供各种图形绘制工具
- 处理用户的绘图操作
- 管理绘制状态

**主要类：**

**CBaseGisTool（基础工具类）：**
```cpp
class CBaseGisTool {
protected:
    CIGisMapEditorView* pView;    // 视图指针
    CZoomManager* pZoom;          // 缩放管理器
    CDC* pDC;                     // 设备上下文
    bool hasInitlize;             // 是否已初始化
    bool LBtDown;                 // 鼠标左键状态
    CPoint lastPoint;             // 上一个点位置
public:
    virtual void LButtonDown(UINT nflags, CPoint point);   // 左键按下
    virtual void LButtonUp(UINT nflags, CPoint point);     // 左键释放
    virtual void RButtonDown(UINT nflags, CPoint point);   // 右键按下
    virtual void RButtonUp(UINT nflags, CPoint point);     // 右键释放
    virtual void MouseMove(UINT nflags, CPoint point);     // 鼠标移动
};
```

**CPointTool（点绘制工具）：**
```cpp
class CPointTool : public CBaseGisTool {
private:
    gisPoint tempgp;              // 临时点要素
    gisPoint gp;                  // 当前点要素
public:
    void LButtonDown(UINT nflags, CPoint point);   // 绘制点
    void flash(CDC* pDC);         // 闪烁效果
    void endflash(CDC* pDC);      // 结束闪烁
};
```

**CLineTool（线绘制工具）：**
```cpp
class CLineTool : public CBaseGisTool {
private:
    gisLine gl;                   // 当前线要素
    CPoint lastPoint;             // 上一个点位置
public:
    void LButtonDown(UINT nflags, CPoint point);   // 添加线段点
    void RButtonDown(UINT nflags, CPoint point);   // 结束绘制
    void flash(CDC* pDC);         // 闪烁效果
};
```

**CPolyTool（多边形绘制工具）：**
```cpp
class CPolyTool : public CBaseGisTool {
private:
    gisPolygon gPoly;             // 当前面要素
    CPoint lastPoint;             // 上一个点位置
public:
    void LButtonDown(UINT nflags, CPoint point);   // 添加多边形点
    void RButtonDown(UINT nflags, CPoint point);   // 结束绘制
    void flash(CDC* pDC);         // 闪烁效果
};
```

**CNoteTool（注记工具）：**
```cpp
class CNoteTool : public CBaseGisTool {
private:
    gisMarker gm;                 // 当前标记
public:
    void LButtonDown(UINT nflags, CPoint point);   // 添加标记
};
```

**CZoomTool（缩放工具）：**
```cpp
class CZoomTool : public CBaseGisTool {
public:
    void LButtonDown(UINT nflags, CPoint point);   // 开始缩放
    void LButtonUp(UINT nflags, CPoint point);     // 结束缩放
};
```

### 3.4 图形编辑工具 (AlterBaseTool.h/cpp)

**模块功能：**
- 提供各种图形编辑工具
- 处理用户的编辑操作
- 支持移动、修改、删除等操作

**主要类：**

**CAlterPointTool（点编辑工具）：**
```cpp
class CAlterPointTool : public CBaseGisTool {
private:
    gisPoint gp;                  // 当前点要素
    gisPoint tempgp;              // 临时点要素
    CString alterType;            // 编辑类型（Move/Delete）
public:
    void LButtonDown(UINT nflags, CPoint point);   // 选择点
    void MouseMove(UINT nflags, CPoint point);     // 移动点
    void flash(CDC* pDC);         // 闪烁效果
    void endflash(CDC* pDC);      // 结束闪烁
};
```

**CAlterLineTool（线编辑工具）：**
```cpp
class CAlterLineTool : public CBaseGisTool {
private:
    gisLine gl;                   // 当前线要素
    gisLine templine;             // 临时线要素
    CString alterType;            // 编辑类型
    int pos;                      // 选中点的位置
public:
    void getLine(gisLine& gl);    // 获取线要素
    void LButtonDown(UINT nflags, CPoint point);   // 选择线
    void MouseMove(UINT nflags, CPoint point);     // 移动线
    bool getLinePt(CPoint point, CPoint& p1, CPoint& p2, int& pos, CString table, CZoomManager* pZoom);  // 获取线段点
    bool delPtFromLine(CPoint point, CString table, gisLine& gl, CZoomManager* pZoom);  // 删除线段点
    bool MoveLinePt(CPoint point, CPoint& p1, CPoint& p2, int& pos, CString table, gisLine& gl, CZoomManager* pZoom);  // 移动线段点
};
```

**CAlterPolygonTool（多边形编辑工具）：**
```cpp
class CAlterPolygonTool : public CBaseGisTool {
private:
    gisPolygon gPoly;             // 当前面要素
    gisPolygon tempgPoly;         // 临时面要素
    CString alterType;            // 编辑类型
    int pos;                      // 选中点的位置
public:
    void getPoly(gisPolygon& gp); // 获取面要素
    void LButtonDown(UINT nflags, CPoint point);   // 选择面
    void MouseMove(UINT nflags, CPoint point);     // 移动面
    bool getPolygonPt(CPoint point, CPoint& p1, CPoint& p2, int& pos, CString table, CZoomManager* pZoom);  // 获取多边形点
    bool delPtFromPolygon(CPoint point, CString table, gisPolygon& gl, CZoomManager* pZoom);  // 删除多边形点
    bool MovePolygonPt(CPoint point, CPoint& p1, CPoint& p2, int& pos, CString table, gisPolygon& gl, CZoomManager* pZoom);  // 移动多边形点
};
```

**CAlterMarkerTool（标记编辑工具）：**
```cpp
class CAlterMarkerTool : public CBaseGisTool {
private:
    gisMarker gm;                 // 当前标记
    gisMarker tempgm;             // 临时标记
    CString alterType;            // 编辑类型
public:
    void getMarker(gisMarker& gm); // 获取标记
    void LButtonDown(UINT nflags, CPoint point);   // 选择标记
    void MouseMove(UINT nflags, CPoint point);     // 移动标记
    void flash(CDC* pDC);         // 闪烁效果
    void endflash(CDC* pDC);      // 结束闪烁
};
```

### 3.5 空间搜索工具 (SearchTool.h/cpp)

**模块功能：**
- 提供各种空间查询功能
- 支持矩形、圆形、多边形范围查询
- 返回查询结果

**主要类：**

**CSearchTool（基础搜索工具）：**
```cpp
class CSearchTool {
protected:
    CIGisMapEditorView* pView;    // 视图指针
    CZoomManager* pZoom;          // 缩放管理器
    DBConnection* pdb;            // 数据库连接
    Layer* layer;                 // 当前图层
public:
    // 矩形范围搜索
    void SearchByRect(CPoint p1, CPoint p2, CString table, gisPoint* gp, int& count);
    void SearchByRect(CPoint p1, CPoint p2, CString table, gisLine* gl, int& count);
    void SearchByRect(CPoint p1, CPoint p2, CString table, gisPolygon* gPoly, int& count);
    void SearchByRect(CPoint p1, CPoint p2, CString table, gisMarker* gm, int& count, CDC* pDC);

    // 圆形范围搜索
    void SearchByCircle(CPoint p1, CPoint p2, CString table, gisPoint* gp, int& count);
    void SearchByCircle(CPoint p1, CPoint p2, CString table, gisLine* gl, int& count);
    void SearchByCircle(CPoint p1, CPoint p2, CString table, gisPolygon* gPoly, int& count);
    void SearchByCircle(CPoint p1, CPoint p2, CString table, gisMarker* gm, int& count, CDC* pDC);

    // 多边形范围搜索
    void SearchByPolygon(CPoint* point, int n, CString table, gisPoint* gp, int& count);
    void SearchByPolygon(CPoint* point, int n, CString table, gisLine* gl, int& count);
    void SearchByPolygon(CPoint* point, int n, CString table, gisPolygon* gPoly, int& count);
    void SearchByPolygon(CPoint* point, int n, CString table, gisMarker* gm, int& count, CDC* pDC);
};
```

### 3.6 坐标系统 (Cordins.h/cpp)

**模块功能：**
- 坐标转换和管理
- 缩放管理
- 地理坐标与屏幕坐标转换

**主要类：**

**CZoomManager（缩放管理器）：**
```cpp
class CZoomManager {
private:
    DPoint lt, rb;                // 地图范围（左上角、右下角）
    int nwidth, nheight;          // 屏幕尺寸
    double zoom;                  // 缩放比例
public:
    // 坐标转换
    CPoint GPtoCP(DPoint gp);     // 地理坐标转屏幕坐标
    DPoint CPtoGP(CPoint cp);     // 屏幕坐标转地理坐标

    // 缩放管理
    void zoomin(CPoint center);   // 放大
    void zoomout(CPoint center);  // 缩小
    double ntimes();              // 获取缩放比例

    // 精度计算
    float xPrecision();           // X方向精度
    float yPrecision();           // Y方向精度

    // 范围管理
    DPoint getLT();               // 获取左上角坐标
    DPoint getRB();               // 获取右下角坐标
    void setRange(DPoint lt, DPoint rb);  // 设置地图范围
    void setScreenSize(int width, int height);  // 设置屏幕尺寸
};
```

**DPoint（坐标点）：**
```cpp
struct DPoint {
    double x;                     // X坐标
    double y;                     // Y坐标
};
```

### 3.7 图层管理 (treelayers.h/cpp)

**模块功能：**
- 图层管理
- 底图管理
- 图层集合管理

**主要类：**

**Layer（图层类）：**
```cpp
class Layer {
private:
    int type;                     // 图层类型（1-点，2-线，3-面，4-标记）
    bool editing;                 // 是否可编辑
    bool visible;                 // 是否可见
    CString name;                 // 图层名称
public:
    Layer();                      // 默认构造函数
    Layer(int Ltype, bool edit, bool view, CString Lname);  // 带参数构造函数
    Layer(const Layer& l);        // 拷贝构造函数

    // 属性访问
    int GetType();                // 获取图层类型
    bool IsEditing();             // 是否可编辑
    bool IsVisible();             // 是否可见
    CString GetName();            // 获取图层名称

    // 属性设置
    void SetType(int t);          // 设置图层类型
    void SetEdit(bool e);         // 设置编辑状态
    void SetVisible(bool v);      // 设置可见性
    void SetName(CString str);    // 设置图层名称

    // 批量设置
    void SetEx(int Ltype, bool edit, bool view, CString Lname);  // 批量设置属性

    // 赋值运算符
    Layer& operator =(const Layer& l);  // 赋值运算符
};
```

**Basepic（底图类）：**
```cpp
class Basepic {
private:
    bool visible;                 // 是否可见
    CString fname;                // 文件名
    double ltx, lty;              // 左上角坐标
    double rbx, rby;              // 右下角坐标
    int w, h;                     // 图片尺寸
public:
    Basepic();                    // 默认构造函数
    Basepic(bool view, CString Lname);  // 带参数构造函数
    Basepic(const Basepic& l);    // 拷贝构造函数

    // 属性访问
    bool IsVisible();             // 是否可见
    CString GetName();            // 获取文件名
    int Height();                 // 获取高度
    int Width();                  // 获取宽度

    // 属性设置
    void SetVisible(bool v);      // 设置可见性
    void SetName(CString str);    // 设置文件名
    void SetBM(int nw, int nh);   // 设置图片尺寸
    void SetCordin(double lt_x, double lt_y, double rb_x, double rb_y);  // 设置坐标范围

    // 批量设置
    void SetEx(bool view, CString Lname, int nw, int nh);  // 批量设置属性

    // 坐标转换
    CPoint GetCP(double x, double y);  // 地理坐标转屏幕坐标

    // 赋值运算符
    Basepic& operator =(const Basepic& l);  // 赋值运算符

public:
    double ltx, lty, rbx, rby;      // 坐标范围（公有成员）
};
```

**TreeItems（图层集合管理）：**
```cpp
class TreeItems {
private:
    std::list<Layer> v;         // 图层列表
public:
    TreeItems();                // 默认构造函数
    TreeItems(TreeItems& tree); // 拷贝构造函数
    ~TreeItems();               // 析构函数

    // 图层操作
    bool add(Layer& l);         // 添加图层
    bool remove(int n);         // 删除图层（按索引）
    void remove(Layer* p);      // 删除图层（按指针）
    bool change(int n, Layer& l);  // 修改图层
    void clear();               // 清空图层列表

    // 图层查询
    bool empty();               // 是否为空
    int size();                 // 获取图层数量
    Layer* LayerPtr(int n);     // 获取图层指针
    Layer* GetLayerByName(CString name);  // 按名称获取图层

    // 图层排序
    void up(Layer* p);          // 上移图层
    void down(Layer* p);        // 下移图层

private:
    bool add_as(int n, Layer& l);  // 在指定位置添加图层
};
```

**ImageItems（影像集合管理）：**
```cpp
class ImageItems {
private:
    std::list<Basepic> v;       // 底图列表
public:
    ImageItems();               // 默认构造函数
    ImageItems(const ImageItems& ii);  // 拷贝构造函数
    ~ImageItems();              // 析构函数

    // 底图操作
    bool add(Basepic& l);       // 添加底图
    bool remove(int n);         // 删除底图（按索引）
    void remove(Basepic* p);    // 删除底图（按指针）
    bool change(int n, Basepic& l);  // 修改底图
    void clear();               // 清空底图列表

    // 底图查询
    bool empty();               // 是否为空
    int size();                 // 获取底图数量
    Basepic* PicPtr(int n);     // 获取底图指针
};
```

### 3.8 其他核心模块

**DataTools（数据工具）：**
- 提供数据处理工具函数
- 坐标计算辅助函数

**GraphStyles（图形样式）：**
- 定义图形样式常量
- 颜色、线型等样式管理

**DjAnalyse（叠置分析）：**
- 线与多边形叠置分析
- 多边形与多边形叠置分析
- 缓冲区分析

---

## 4. 使用说明

### 4.1 首次使用

#### 步骤1：启动应用程序
1. 双击 `IGisMapEditor.exe` 启动应用程序
2. 等待主窗口加载完成

#### 步骤2：连接数据库
1. 点击菜单栏的"数据库" → "连接数据库"
2. 在弹出的对话框中输入数据库连接信息：
   - 服务器地址（如：localhost）
   - 数据库名称（如：GISDatabase）
   - 用户名和密码（如果需要）
3. 点击"连接"按钮
4. 等待连接成功提示

#### 步骤3：创建新图层
1. 点击菜单栏的"图层" → "新建图层"
2. 在弹出的对话框中选择图层类型：
   - 点图层
   - 线图层
   - 面图层
   - 标记图层
3. 输入图层名称
4. 点击"确定"按钮

#### 步骤4：开始绘制
1. 在工具栏中选择绘图工具：
   - 点工具：绘制点要素
   - 线工具：绘制线要素
   - 面工具：绘制面要素
   - 标记工具：添加文本标记
2. 在地图视图区域点击鼠标左键开始绘制
3. 根据需要继续点击或拖动鼠标
4. 点击鼠标右键结束绘制

#### 步骤5：保存项目
1. 点击菜单栏的"文件" → "保存"或"另存为"
2. 选择保存位置
3. 输入项目文件名（*.igp）
4. 点击"保存"按钮

### 4.2 基本操作

#### 绘制要素

**绘制点要素：**
1. 在工具栏中选择"点工具"
2. 在地图视图区域点击鼠标左键
3. 点要素自动保存到数据库

**绘制线要素：**
1. 在工具栏中选择"线工具"
2. 在地图视图区域点击鼠标左键确定起点
3. 移动鼠标，继续点击鼠标左键添加线段点
4. 点击鼠标右键结束绘制
5. 线要素自动保存到数据库

**绘制面要素：**
1. 在工具栏中选择"面工具"
2. 在地图视图区域点击鼠标左键确定第一个顶点
3. 移动鼠标，继续点击鼠标左键添加多边形顶点
4. 点击鼠标右键结束绘制（自动闭合多边形）
5. 面要素自动保存到数据库

**添加文本标记：**
1. 在工具栏中选择"标记工具"
2. 在地图视图区域点击鼠标左键确定标记位置
3. 在弹出的对话框中输入标记文本
4. 设置字体样式（可选）
5. 点击"确定"按钮

#### 编辑要素

**选择要素：**
1. 在工具栏中选择"选择工具"
2. 在地图视图区域点击要编辑的要素
3. 被选中的要素会闪烁显示

**移动要素：**
1. 选择要移动的要素
2. 在工具栏中选择"移动工具"
3. 按住鼠标左键拖动要素
4. 释放鼠标左键完成移动

**修改要素属性：**
1. 选择要修改的要素
2. 点击鼠标右键，在弹出的菜单中选择"属性"
3. 在属性对话框中修改要素属性
4. 点击"确定"按钮保存修改

**删除要素：**
1. 选择要删除的要素
2. 点击鼠标右键，在弹出的菜单中选择"删除"
3. 在确认对话框中点击"是"按钮

**编辑线段/多边形顶点：**
1. 选择要编辑的线段或多边形
2. 在工具栏中选择"编辑顶点工具"
3. 点击要编辑的顶点
4. 拖动顶点到新位置
5. 释放鼠标完成编辑

**添加线段/多边形顶点：**
1. 选择要编辑的线段或多边形
2. 在工具栏中选择"添加顶点工具"
3. 在线段上点击鼠标左键添加新顶点
4. 新顶点自动插入到线段中

**删除线段/多边形顶点：**
1. 选择要编辑的线段或多边形
2. 在工具栏中选择"删除顶点工具"
3. 点击要删除的顶点
4. 在确认对话框中点击"是"按钮

#### 查询要素

**矩形范围查询：**
1. 在工具栏中选择"矩形查询工具"
2. 在地图视图区域按住鼠标左键拖动绘制矩形
3. 释放鼠标左键执行查询
4. 查询结果显示在查询窗口中

**圆形范围查询：**
1. 在工具栏中选择"圆形查询工具"
2. 在地图视图区域点击鼠标左键确定圆心
3. 拖动鼠标调整圆形半径
4. 释放鼠标左键执行查询
5. 查询结果显示在查询窗口中

**多边形范围查询：**
1. 在工具栏中选择"多边形查询工具"
2. 在地图视图区域点击鼠标左键确定多边形顶点
3. 继续点击添加顶点
4. 点击鼠标右键结束绘制并执行查询
5. 查询结果显示在查询窗口中

#### 图层管理

**添加图层：**
1. 点击菜单栏的"图层" → "添加图层"
2. 在弹出的对话框中选择数据库中的图层数据表
3. 点击"确定"按钮
4. 图层自动添加到图层树中

**删除图层：**
1. 在左侧图层树中选择要删除的图层
2. 点击鼠标右键，在弹出的菜单中选择"删除图层"
3. 在确认对话框中点击"是"按钮

**控制图层可见性：**
1. 在左侧图层树中找到要控制的图层
2. 点击图层前面的复选框
   - 勾选：显示图层
   - 取消勾选：隐藏图层

**设置图层编辑状态：**
1. 在左侧图层树中选择要设置的图层
2. 点击鼠标右键，在弹出的菜单中选择"设为可编辑"
3. 当前编辑图层会被高亮显示

**调整图层顺序：**
1. 在左侧图层树中选择要移动的图层
2. 点击鼠标右键，在弹出的菜单中选择：
   - "上移"：将图层向上移动
   - "下移"：将图层向下移动
3. 图层顺序会影响显示叠加效果

#### 地图浏览

**放大地图：**
1. 在工具栏中选择"放大工具"
2. 在地图视图区域点击鼠标左键
3. 地图以点击位置为中心放大

**缩小地图：**
1. 在工具栏中选择"缩小工具"
2. 在地图视图区域点击鼠标左键
3. 地图以点击位置为中心缩小

**平移地图：**
1. 在工具栏中选择"平移工具"
2. 在地图视图区域按住鼠标左键拖动
3. 释放鼠标左键完成平移

**使用鼠标滚轮缩放：**
1. 将鼠标指针放在要缩放的位置
2. 向上滚动滚轮：放大地图
3. 向下滚动滚轮：缩小地图

**查看坐标：**
1. 在状态栏中可以看到当前鼠标位置的坐标
2. 坐标格式为：X, Y

### 4.3 项目文件操作

#### 新建项目
1. 点击菜单栏的"文件" → "新建"
2. 系统会提示保存当前项目（如果有未保存的修改）
3. 创建新的空白项目

#### 打开项目
1. 点击菜单栏的"文件" → "打开"
2. 在文件选择对话框中找到 *.igp 项目文件
3. 选择文件并点击"打开"按钮
4. 系统会自动加载项目配置和图层数据

#### 保存项目
1. 点击菜单栏的"文件" → "保存"
2. 如果是第一次保存，会弹出"另存为"对话框
3. 选择保存位置和文件名
4. 点击"保存"按钮

#### 另存为
1. 点击菜单栏的"文件" → "另存为"
2. 在文件选择对话框中选择保存位置
3. 输入新的文件名
4. 点击"保存"按钮

### 4.4 数据库操作

#### 连接数据库
1. 点击菜单栏的"数据库" → "连接数据库"
2. 在连接对话框中输入：
   - 服务器地址（如：localhost 或 192.168.1.100）
   - 数据库名称
   - 用户名（如果需要）
   - 密码（如果需要）
3. 点击"连接"按钮
4. 等待连接成功提示

#### 断开数据库
1. 点击菜单栏的"数据库" → "断开连接"
2. 在确认对话框中点击"是"按钮
3. 数据库连接被断开

#### 创建数据库
1. 点击菜单栏的"数据库" → "创建数据库"
2. 在创建对话框中输入：
   - 服务器地址
   - 数据库名称
   - 数据库文件路径
   - 用户名和密码（如果需要）
3. 点击"创建"按钮
4. 等待创建成功提示

#### 查看数据库信息
1. 点击菜单栏的"数据库" → "数据库信息"
2. 在弹出的对话框中查看：
   - 连接状态
   - 数据库名称
   - 服务器地址
   - 图层数量
   - 数据表列表

### 4.5 空间分析

#### 线与多边形叠置分析
1. 确保已加载线图层和多边形图层
2. 点击菜单栏的"空间分析" → "线与多边形叠置"
3. 在对话框中选择：
   - 线图层
   - 多边形图层
   - 结果表名
4. 点击"执行"按钮
5. 等待分析完成
6. 结果会自动添加到图层树中

#### 多边形与多边形叠置分析
1. 确保已加载两个多边形图层
2. 点击菜单栏的"空间分析" → "多边形与多边形叠置"
3. 在对话框中选择：
   - 第一个多边形图层
   - 第二个多边形图层
   - 结果表名
4. 点击"执行"按钮
5. 等待分析完成
6. 结果会自动添加到图层树中

#### 缓冲区分析
1. 选择要分析的要素（点、线或面）
2. 点击菜单栏的"空间分析" → "缓冲区分析"
3. 在对话框中输入：
   - 缓冲区半径
   - 结果表名
4. 点击"执行"按钮
5. 等待分析完成
6. 缓冲区结果会自动添加到图层树中

### 4.6 常见问题

**Q: 无法连接到数据库怎么办？**
A: 请检查：
1. 服务器地址是否正确
2. 数据库名称是否正确
3. 用户名和密码是否正确
4. SQL Server 服务是否正在运行
5. 防火墙是否允许连接

**Q: 绘制的要素没有显示出来？**
A: 请检查：
1. 图层是否可见（图层树中的复选框是否勾选）
2. 图层顺序是否正确（可能被其他图层遮挡）
3. 缩放级别是否合适（可能地图缩放太小或太大）
4. 要素是否在当前视图范围内

**Q: 如何修改要素的颜色和样式？**
A:
1. 选择要修改的要素
2. 点击鼠标右键，选择"属性"
3. 在属性对话框中修改颜色、大小、线型等属性
4. 点击"确定"保存修改

**Q: 如何导入底图？**
A:
1. 点击菜单栏的"图层" → "添加底图"
2. 选择底图图片文件
3. 设置底图的地理坐标范围
4. 点击"确定"按钮

**Q: 项目文件损坏了怎么办？**
A:
1. 尝试使用文本编辑器打开 *.igp 文件
2. 检查文件格式是否正确
3. 如果文件内容损坏，可以手动修复
4. 如果无法修复，需要重新创建项目

**Q: 如何提高查询性能？**
A:
1. 减少查询范围
2. 减少图层数量
3. 为数据库表添加索引
4. 使用更快的计算机
5. 升级到更高版本的 SQL Server

---

## 附录

### A. 快捷键

| 快捷键 | 功能 |
|--------|------|
| Ctrl+N | 新建项目 |
| Ctrl+O | 打开项目 |
| Ctrl+S | 保存项目 |
| Ctrl+Z | 撤销 |
| Ctrl+Y | 重做 |
| Delete | 删除选中要素 |
| Esc | 取消当前操作 |
| F1 | 帮助 |

### B. 图层类型说明

| 类型编号 | 类型名称 | 说明 |
|----------|----------|------|
| 1 | 点图层 | 存储点状地理要素 |
| 2 | 线图层 | 存储线状地理要素 |
| 3 | 面图层 | 存储面状地理要素 |
| 4 | 标记图层 | 存储文本标记 |

### C. 颜色格式说明

本软件使用 RGB 格式表示颜色：
- 红色：RGB(255, 0, 0) = 0x00FF0000
- 绿色：RGB(0, 255, 0) = 0x0000FF00
- 蓝色：RGB(0, 0, 255) = 0x000000FF
- 白色：RGB(255, 255, 255) = 0x00FFFFFF
- 黑色：RGB(0, 0, 0) = 0x00000000

### D. 线型说明

| 线型 | 说明 |
|------|------|
| PS_SOLID | 实线 |
| PS_DASH | 虚线 |
| PS_DOT | 点线 |
| PS_DASHDOT | 点划线 |
| PS_DASHDOTDOT | 双点划线 |

### E. 技术支持

如遇到问题或有建议，请通过以下方式联系：
- GitHub Issues：https://github.com/xrui94/IGisMapEditor/issues
- Email：[你的邮箱地址]

---

**文档版本：** 1.0  
**最后更新：** 2026年3月5日  
**维护者：** xrui94
