# LiteXLoader - NBT 文档

> **NBT（二进制命名标签**，**N**amed **B**inary **T**ags**）** 格式为Minecraft中用于向文件中存储数据的一种存储格式。   
> NBT格式以树形结构并配以许多*标签* 的形式存储数据。所有的标签都有一个独立的ID和名称。
>
> --- Minecraft Wiki

这里为脚本提供了操作 **NBT** 数据类型的能力。 **NBT** 接口的支持，极大的提高了游戏的可扩展性

游戏中，使用被称为 **NBT标签 **的节点来标识一项NBT数据。NBT标签中可以储存普通数据、List、Compound 等多种数据类型。   

在LXL引擎中，每种NBT数据类型都有其对应的数据类型，我们统称他们为「NBT对象」。  
LXL引擎类型与NBT数据类型的对照如下：

| NBT数据类型 | 对应的NBT对象类型 | 类型说明（来自MC Wiki）       |
| ----------- | ----------------- | ----------------------------- |
| `End`       | `NbtEnd`          | 用于标记标签的结尾            |
| `Byte`      | `NbtByte`         | 有符号字节 或 布尔值（8位）   |
| `Short`     | `NbtShort`        | 有符号短整型（16位）          |
| `Int`       | `NbtInt`          | 有符号整形（32位）            |
| `Long`      | `NbtLong`         | 有符号长整型（64位）          |
| `Float`     | `NbtFloat`        | 单精度浮点数                  |
| `Double`    | `NbtDouble`       | 双精度浮点数                  |
| `ByteArray` | `NbtByteBuffer`   | 字节数组                      |
| `String`    | `NbtString`       | UTF-8 字符串                  |
| `List`      | `NbtList`         | NBT 列表（类似于 数组）       |
| `Compound`  | `NbtCompound`     | NBT 标签（类似于 键值对列表） |

每种数据类型可能有略微不同的使用接口。下面对他们作分别介绍

<br>

## 🎈  NBT 对象通用接口

每一个 NBT 对象都包含一些可以执行的成员函数（成员方法）  
对于任何种类的NBT对象，都有下面这些通用的接口。以名为`nbt`的某个NBT对象为例：

#### 获取NBT对象储存的数据类型

`nbt.getType()`

- 返回值：此NBT对象储存的数据类型
- 返回值类型：`Enum`

可能的返回值有：`NBT.End` `NBT.Byte` `NBT.Short` `NBT.Int` `NBT.Long`   
`NBT.Float` `NBT.Double` `NBT.ByteArray` `NBT.String`  
`NBT.List` `NBT.Compound`

<br>

#### 将NBT对象转换为Json字符串

`nbt.toString([space])`

- 参数
  - space : `Integer`  
    （可选参数）如果要格式化输出的字符串，则传入此参数  
    代表每个缩进的空格数量，这样生成的字符串更适合人阅读  
    此参数默认为0，即不对输出字符串进行格式化
- 返回值：对应的Json字符串
- 返回值类型：`String`

提示：如果这个NBT对象储存的是`List`或者`Compound`类型，将递归展开为`Array`或`Object`  
如果这个NBT对象储存的是`ByteArray`类型，输出的字节串将先进行base64编码后再输出

> 上述函数输出的字符串符合Json标准格式，但是无法进行反序列化。  
> 如果有反序列化的需求，请使用 NBT标签类 提供的的 **SNBT** 接口

<br>

## 📋 NBT - 普通数据类型

我们把除了 `NbtList` 和 `NbtCompound` 类型以外的其他NBT对象统称为 **普通数据** 类型。这些对象只储存了简单的数据，内部不含有其他复杂结构。

### 获取一个 NBT 普通数据对象

#### 从现有的NBT对象中解析获取

详见 [NbtList - 列表类型](zh_CN/Development/NbtAPI/NBTList.md) 和 [NbtCompound - 标签类型](zh_CN/Development/NbtAPI/NBTCompound.md) 文档

#### 创建新的NBT数据对象

[Js]  
`new NbtEnd()`  
`new NbtByte([data])`  
`new NbtShort([data])`  
`new NbtInt([data])`  
`new NbtLong([data])`  
`new NbtFloat([data])`  
`new NbtDouble([data])`   
`new NbtByteArray([data])`  
`new NbtString([data])`  
[Lua]  
`NbtEnd()`  
`NbtByte([data])`  
`NbtShort([data])`  
`NbtInt([data])`  
`NbtLong([data])`  
`NbtFloat([data])`  
`NbtDouble([data])`   
`NbtByteArray([data])`  
`NbtString([data])`  

- 参数：
  - data: `Number` / `Float` / `Double` / `ByteBuffer` / `String`（可选参数）  
    根据你要创建的对象类型设置初始数据，传入的数据类型需要能够初始化对应类型的数据  
    可以不传入此参数
- 返回值：生成的NBT对象
- 返回值类型：`NbtEnd` / `NbtByte` / `NbtShort` / `NbtInt` / `NbtLong` / `NbtFloat` / `NbtDouble` / `NbtByteBuffer` / `NbtString` ，取决于你选择的数据类型
  - 如果创建失败，将抛出异常

<br>

对于一个储存普通数据类型的NBT对象 `value`，有如下成员函数

#### 设置对象的数据

`value.set(data)`

- 参数：
  - data : `Number` / `Float` / `Double` / `ByteBuffer` / `String`  
    根据这个NBT对象的数据类型，写入对应类型的数据  
    如对于一个`Float`类型的 NBT 对象，你需要传入一个`Float`类型的值
- 返回值：是否成功写入
- 返回值类型：`Boolean`

<br>

#### 读取对象的数据

`value.get()`

- 返回值：对象中储存的数据
- 返回值类型：``Number` / `Float` / `Double` / `ByteBuffer` / `String`，以储存的实际数据类型为准

<br>