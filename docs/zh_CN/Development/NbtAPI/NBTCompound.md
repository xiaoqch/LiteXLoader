## 📒 NbtCompound - 标签类型

一个`NbtCompound `代表一个完整的标签，里面储存了一系列键值对，类似于对象 / 表这样的数据结构。  
对于某个一个`NbtCompound`类型的对象`comp`，有如下这些接口

#### 获取所有的键

`comp.getKeys()`

- 返回值：Compound 中所有的键
- 返回值类型：`Array<String,String,...>`

<br>

#### 获取键对应的值的数据类型

`comp.getTypeOf(key)`

- 参数：
  - key: `String`  
    要查询的键名
- 返回值：对应的值的数据类型
- 返回值类型：`Enum`

可能的返回值有：`NBT.End` `NBT.Byte` `NBT.Short` `NBT.Int` `NBT.Long`   
`NBT.Float` `NBT.Double` `NBT.ByteArray` `NBT.String`  
`NBT.List` `NBT.Compound`

<br>

#### 获取键对应的值的具体数据（方便函数）

`comp.setEnd(key)`  
`comp.setByte(key,data)`  
`comp.setShort(key,data)`  
`comp.setInt(key,data)`  
`comp.setLong(key,data)`  
`comp.setFloat(key,data)`  
`comp.setDouble(key,data)`  
`comp.setString(key,data)`    

- 参数：
  - key: `String`   
    要操作的键名
  - data: `上述类型`  
    要写入的具体数据  
    写入数据类型必须和键对应的值储存的数据类型一致，键名可以不存在
- 返回值：是否成功写入
- 返回值类型：`Boolean`

<br>

#### 设置键对应的NBT对象

`comp.setTag(key,tag)`

- 参数：
  - key: `String`   
    要操作的键名
  - tag: `NbtValue` 或 `NbtList` 或 `NbtCompound`  
    要写入的 NBT 对象（它承载着具体的NBT数据）  
    写入数据类型必须和键对应的值储存的数据类型一致，键名可以不存在
- 返回值：是否成功写入
- 返回值类型：`Boolean`

<br>

#### 读取键对应的值的具体数据（方便函数）

`comp.getData(key)`  

- 参数：
  - key: `String`   
    要操作的键名
- 返回值：键对应的值的具体数据
- 返回值类型：`任意类型`，以储存的数据类型为准
  - 如果目标位置储存的是`List`类型 NBT，将返回一个`NbtList`对象；如果目标位置储存的是`Compound`类型 NBT，将返回一个`NbtCompound`对象

<br>

#### 读取键对应的NBT对象

`comp.getTag(key)`

- 参数：
  - key: `String`   
    要操作的键名
- 返回值：键对应的NBT对象
- 返回值类型：`NbtValue` 或 `NbtList` 或 `NbtCompound` 

<br>

#### 将Compound类型转换为Object（方便函数）

`comp.toObject()`

- 返回值：对应的对象 / 表
- 返回值类型：`Object`

将Compound转换为LXL对象 / 表，把所有能转换为LXL数据类型的数据项都转换为LXL数据类型储存于对象 / 表的对应key中，方便读取和处理  
如果Compound某一项储存的是`List`类型 NBT，将在对应位置放置一个`NbtList`对象；如果List某一项储存的是`Compound`类型 NBT，将在对应位置放置一个`NbtCompound`对象

<br>

#### 由数组读取并全部写入List类型（方便函数）

`comp.fromObject(obj)`

- 参数：
  - obj: `Object`  
    要读取的对象 / 表
- 返回值：是否成功转换
- 返回值类型：`Boolean`

由LXL对象 / 表反向生成并写入Compound，上面一个函数的逆向操作