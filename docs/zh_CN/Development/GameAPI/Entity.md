## 🎈 实体对象 API

在LXL中，使用「实体对象」来操作和获取某一个实体的相关信息。

### 获取一个实体对象

1. 通过注册**事件监听**函数，或者调用某些**返回实体对象**的函数，来获取到BDS给出的实体对象    
   详见 [事件监听文档 - EventAPI](zh_CN/Development/EventAPI/Listen.md)      

2. 通过 **命名空间名称** 生成生物  
   通过此函数，在指定的位置手动生成一个生物，并获取它对应的实体对象
   
   `mc.spawnMob(name,pos)`  
   `mc.spawnMob(name,x,y,z,dimid)`
   
   - 参数：
     - name : `String`  
       生物的命名空间名称，如 `creeper`
     - pos : `FloatPos`  
       生成生物的坐标对象（或者使用x, y, z, dimid来确定生成位置）
   - 返回值：生成的实体对象
   - 返回值类型：`Entity`
     - 如返回值为 `Null` 则表示生成失败
   
3. 注意：不要**长期保存**一个实体对象  
   当实体对象对应的实体被销毁时，对应的实体对象将变得无效。因此，如果有长期操作某个实体的需要，请通过上述途径获取实时的实体对象

<br>


### 实体对象 - 属性

每一个实体对象都包含一些固定的对象属性。对于某个特定的实体对象`en`，有以下这些属性

| 属性         | 含义             | 类型       |
| ------------ | ---------------- | ---------- |
| en.name      | 实体名称         | `String`   |
| en.type      | 实体标准类型名   | `String`   |
| en.id        | 实体的游戏内id   | `Integer`  |
| en.pos       | 实体所在坐标     | `FloatPos` |
| en.maxHealth | 实体最大生命值   | `Integer`  |
| en.health    | 实体当前生命值   | `Integer`  |
| en.inAir     | 实体当前是否悬空 | `Boolean`  |
| en.speed     | 实体当前速度     | `Float`    |

这些对象属性都是只读的，无法被修改

<br>

### 实体对象 - 函数

每一个实体对象都包含一些可以执行的成员函数（成员方法）。对于某个特定的实体对象`en`，可以通过以下这些函数对这个实体进行一些操作

#### 传送实体至指定位置

`en.teleport(pos)`  
`en.teleport(x,y,z,dimid)`

- 参数：
  - pos : `FloatPos`  
    目标位置坐标（或者使用x, y, z, dimid来确定实体位置）
- 返回值：是否成功传送
- 返回值类型：`Boolean`

<br>

#### 杀死指定实体  

`en.kill()`

- 返回值：是否成功执行
- 返回值类型：`Boolean`

<br>

#### 为实体增加一个Tag

`en.addTag(tag)`

- 参数：
  - tag: `String`  
    要增加的tag字符串
- 返回值：是否设置成功
- 返回值类型：`Boolean`

<br>

#### 为实体移除一个Tag

`en.removeTag(tag)`

- 参数：
  - tag: `String`  
    要移除的tag字符串
- 返回值：是否移除成功
- 返回值类型：`Boolean`

<br>

#### 返回实体拥有的所有Tag列表

`en.getAllTags()`

- 返回值：实体所有的 tag 字符串列表
- 返回值类型：`Array<String,String,...>`

<br>

#### 获取实体对应的NBT对象

`en.getNbt()`

- 返回值：实体的NBT对象
- 返回值类型：`NbtCompound`

关于NBT对象的更多使用，请参考 [NBT接口文档](zh_CN/Development/NbtAPI/NBT.md)

<br>

#### 判断一个实体对象是不是玩家

`en.isPlayer()`

- 返回值：当前实体对象是不是玩家
- 返回值类型：`Boolean`

<br>

#### 将实体对象转换玩家对象

`en.toPlayer()`

- 返回值：转换成的玩家对象
- 返回值类型：`Player`
  - 如果此实体对象指向的不是某个玩家，或者转换失败，则返回 `Null`

如果当前实体对象指向的是一个玩家，可以使用此函数将实体对象转换为玩家对象，以使用更多的玩家相关 API

<br>

#### 使指定实体着火

`en.setOnFire(time)`

- 参数：
  - time : `Number`  
    着火时长，单位秒
- 返回值：是否成功着火
- 返回值类型：`Boolean`

<br>