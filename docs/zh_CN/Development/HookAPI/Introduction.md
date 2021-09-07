# LiteXLoader - 脚本层底层调用 文档

> 这里API使你可以在脚本中对 **底层C/C++接口** 进行操作与监听

由于LXL实际上对底层接口进行了一定程度的封装，所以，在实际开发的过程中，难免遇到有尚未被封装的功能或者符号需要被使用。此时，除了向项目维护者提出API请求之外，你也可以自行使用此处的接口进行底层调用，大大拓展了插件系统的灵活性与自由性。

注意，这一主题需要开发者对如下概念有深刻的理解与掌握：

1. C++ 语言及底层原理，指针的灵活使用，函数调用原理，函数 Hook 原理
2. Windows 部分操作系统原理，动态链接库机制
3. 简单的逆向工程实操（使用 IDA Pro 分析 BDS 符号文件）

下面，对整个脚本层底层调用系统作介绍

<br>

## 📕 脚本层类型声明系统

为了在弱类型的脚本系统中对强类型的 C / C++ 底层进行操作，需要在脚本中建立一套声明式的类型系统，对底层接口的原型进行声明，便于引擎解析以及进行相关调用。

LXL使用 Native 类封装相关的类型声明以及底层操作接口。下表列出了LXL支持的底层数据类型，以及相应的类型声明

### 基础类型

| Native类型声明          | 底层数据类型（C/C++） | 对应的LXL数据类型 | 备注                           |
| ----------------------- | --------------------- | ----------------- | ------------------------------ |
| Native.Void             | `void`                | Null              | 空                             |
| Native.Bool             | `bool`                | Boolean           | 布尔型                         |
| Native.Char             | `char`                |                   | 字符型                         |
| Native.UnsignedChar     | `unsigned char`       |                   | 无符号字符型                   |
| Native.Short            | `short`               |                   | 短整型                         |
| Native.UnsignedShort    | `unsigned short`      |                   | 无符号短整型                   |
| Native.Int              | `int`                 |                   | 整型                           |
| Native.UnsignedInt      | `unsigned int`        |                   | 无符号整型                     |
| Native.Long             | `long`                |                   | 长整型                         |
| Native.UnsignedLong     | `unsigned long`       |                   | 无符号长整形                   |
| Native.LongLong         | `long long`           |                   | 64位长整型                     |
| Native.UnsignedLongLong | `unsigned long long`  |                   | 无符号64位长整形               |
| Native.Float            | `float`               |                   | 单精度浮点型                   |
| Native.Double           | `double`              |                   | 双精度浮点型                   |
| Native.Pointer          | `void* (any*)`        |                   | 任意类型指针（数组，结构体等） |
| Native.Int8             | `int8_t`              |                   | 8位数值类型                    |
| Native.Int16            | `int16_t`             |                   | 16位数值类型                   |
| Native.Int32            | `int32_t`             |                   | 32位数值类型                   |
| Native.Int64            | `int64_t`             |                   | 64位数值类型                   |

### C  常用类型

| Native类型声明     | 底层数据类型（C/C++） | 备注               |
| ------------------ | --------------------- | ------------------ |
| Native.CString     | `char[]`              | \0结尾的字符数组   |
| Native.CWideString | `wchar_t[]`           | \0结尾的宽字符数组 |

### C ++ 标准库类型

| Native类型声明    | 底层数据类型（C/C++） | 备注               |
| ----------------- | --------------------- | ------------------ |
| Native.String     | `std::string`         | 标准库字符串       |
| Native.WideString | `std::wstring`        | 标准库宽字符串     |
| Native.Vector     | `std::vector`         | 标准库动态数组容器 |
| Native.Map        | `std::map`            | 标准库有序关联容器 |

### Windows 操作系统类型

| Native类型声明 | 底层数据类型（C/C++） | 备注             |
| -------------- | --------------------- | ---------------- |
| Native.HANDLE  | `HANDLE`              | Windows 句柄类型 |
| Native.DWORD   | `DWORD`               | Windows 32位整型 |
| Native.LPSTR   | `LPSTR (char*)`       | Windows 字符串   |
| Native.LPWSTR  | `LPWSTR (wchar_t*)`   | Windows 宽字符串 |

### BDS 数据类型

| Native类型声明     | 底层数据类型（C/C++） | 备注               |
| ------------------ | --------------------- | ------------------ |
| Native.Player      | `Player*`             | 玩家指针           |
| Native.Block       | `Block*`              | 方块（类）指针     |
| Native.Entity      | `Actor*`              | 实体指针           |
| Native.Actor       | `Actor*`              | 实体指针           |
| Native.ItemStack   | `ItemStack*`          | 物品堆指针         |
| Native.BlockEntity | `BlockActor*`         | 方块实体指针       |
| Native.Container   | `Container*`          | 容器指针           |
| Native.Objective   | `Objective*`          | 计分项对象指针     |
| Native.BlockPos    | `BlockPos*`           | 三维整数坐标位置   |
| Native.Vec3        | `Vec3*`               | 三维浮点数坐标位置 |

在声明函数原型的过程中，上述类型声明符号将成为重要的工具之一