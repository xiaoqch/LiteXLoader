## 🏃‍♂️ 玩家对象 API

在LXL中，使用「玩家对象」来操作和获取某一个玩家的相关信息。

### 获取一个玩家对象

获取玩家对象有三种方式：

1. 通过注册**事件监听**函数，获取到BDS给出的与相关事件有关的玩家对象  
   详见 [事件监听文档 - EventAPI](zh_CN/Development/EventAPI/Listen.md)

   

2. 通过**玩家名**或者**xuid**手动生成玩家对象  
   通过此函数来手动生成对象，注意，你要获取的玩家必须是在线状态，否则会生成失败

   `mc.getPlayer(info)`

   - 参数：
     - info : `String`  
       玩家的名字或者Xuid
   - 返回值：生成的玩家对象 
   - 返回值类型：`Player`
     - 如返回值为 `Null` 则表示获取玩家失败

   

3. 获取**所有在线玩家**的列表  
   此函数会返回一个玩家对象的数组，其中每个对象都对应了一个服务器中的玩家

   `mc.getOnlinePlayers()`

   - 返回值：在线的玩家对象列表
   - 返回值类型：`Array<Player,Player,...>`
   
   
   
4. 注意：不要**长期保存**一个玩家对象  
   当玩家退出服务器时，对应的玩家对象将变得无效。因此，如果有长期操作某个玩家的需要，请通过上述途径获取实时的玩家对象

<br>

### 玩家对象 - 属性

每一个玩家对象都包含一些固定的对象属性。对于某个特定的玩家对象`pl`，有以下这些属性

| 属性         | 含义                        | 类型       |
| ------------ | --------------------------- | ---------- |
| pl.name      | 玩家名                      | `String`   |
| pl.pos       | 玩家所在坐标                | `FloatPos` |
| pl.realName  | 玩家的真实名字              | `String`   |
| pl.xuid      | 玩家Xuid字符串              | `String`   |
| pl.uuid      | 玩家Uuid字符串              | `String`   |
| pl.ip        | 玩家设备的IP地址            | `String`   |
| pl.permLevel | 玩家的操作权限等级（0 - 4） | `Integer`  |
| pl.gameMode  | 玩家的游戏模式（0 - 3）     | `Integer`  |
| pl.maxHealth | 玩家最大生命值              | `Integer`  |
| pl.health    | 玩家当前生命值              | `Integer`  |
| pl.inAir     | 玩家当前是否悬空            | `Boolean`  |
| pl.sneaking  | 玩家当前是否正在潜行        | `Boolean`  |
| pl.speed     | 玩家当前速度                | `Float`    |
| pl.direction | 玩家当前朝向（0 - 4）       | `Integer`  |

这些对象属性都是只读的，无法被修改。其中：

- **玩家游戏模式** 属性的取值为：`0` 代表生存模式，`1` 代表创造模式，`2` 代表极限模式，`3` 代表旁观者模式 
- **玩家当前朝向** 属性记录了玩家当前面对的方向。`0`代表北方，`1`代表东方，`2`代表南方，`3`代表西方，
- **玩家真实名字** 属性储存的字符串可以被认为是可靠的，他们不会被改名而变动  
- **玩家设备IP地址** 属性储存了玩家的设备IP以及端口号，格式类似`12.34.567.89:1111`  
- **操作权限等级** 属性的对照表如下：

| 操作权限等级 | 对应操作权限    |
| ------------ | --------------- |
| 0            | 普通成员权限    |
| 1            | OP权限          |
| 4            | OP + 控制台权限 |

<br>

### 玩家对象 - 函数

每一个玩家对象都包含一些可以执行的成员函数（成员方法）。对于某个特定的玩家对象`pl`，可以通过以下这些函数对这个玩家进行一些操作

#### 判断玩家是否为OP  

`pl.isOP()`

- 返回值：玩家是否为OP
- 返回值类型：`Boolean`  

```clike
[Js]
//对于一个玩家对象pl
var open = pl.isOP();
[Lua]

```

#### 断开玩家连接  

`pl.kick([msg])`  
`pl.disconnect([msg])`

- 参数：
  - msg : `String`  
    （可选参数）被踢出玩家出显示的断开原因。  
    如果不传入，默认为“正在从服务器断开连接”  
- 返回值：是否成功断开连接
- 返回值类型：`Boolean`

```clike
[Js]
//对于一个玩家对象pl
pl.kick();
[Lua]

```

#### 发送一个文本消息给玩家  

`pl.tell(msg[,type])`  
`pl.sendText(msg[,type])`

- 参数：

  - msg : `String`  
    待发送的文本  

  - type : `Integer`  
    （可选参数）发送的文本消息类型，默认为0  

    | type参数 | 消息类型                |
    | -------- | ----------------------- |
    | 0        | 普通消息（Raw）         |
    | 1        | 聊天消息（Chat）        |
    | 5        | 物品栏上方的消息（Tip） |
    | 9        | Json格式消息（Json）    |

- 返回值：是否成功发送

- 返回值类型：`Boolean`

```clike
[Js]
//对于一个玩家对象pl
pl.tell("Welcome back ~ ",5);
[Lua]

```

#### 广播一个文本消息给所有玩家  

`mc.broadcast(msg[,type])`

- 参数：

  - msg : `String`  
    待发送的文本  

  - type : `Integer`  
    （可选参数）发送的文本消息类型，默认为0  

    | type参数 | 消息类型                |
    | -------- | ----------------------- |
    | 0        | 普通消息（Raw）         |
    | 1        | 聊天消息（Chat）        |
    | 5        | 物品栏上方的消息（Tip） |
    | 9        | Json格式消息（Json）    |

- 返回值：是否成功发送

- 返回值类型：`Boolean`

```clike
[Js]
mc.broadcast("Hello everyone ~ ");
[Lua]

```

#### 以某个玩家身份执行一条命令 

`pl.runcmd(cmd)`

- 参数：
  - cmd : `String`  
    待执行的命令  
- 返回值：是否执行成功
- 返回值类型： `Boolean`   

```clike
[Js]
//对于一个玩家对象pl
var open = pl.runcmd("tp ~ ~+50 ~");
[Lua]

```

#### 传送玩家至指定位置  

`pl.teleport(pos)`  
`pl.teleport(x,y,z,dimid)`

- 参数：
  - pos : `FloatPos`  
    目标位置坐标 （或者使用x, y, z, dimid来确定玩家位置）
- 返回值：是否成功传送
- 返回值类型：`Boolean`

```clike
[Js]
//对于一个玩家对象pl，一个坐标对象pos
pl.teleport(pos);
[Lua]

```

#### 杀死玩家  

`pl.kill()`

- 返回值：是否成功执行
- 返回值类型：`Boolean`

```clike
[Js]
//对于一个玩家对象pl
pl.kill();
[Lua]

```

#### 使指定玩家着火

`pl.setOnFire(time)`

- 参数：
  - time : `Number`  
    着火时长，单位秒
- 返回值：是否成功着火
- 返回值类型：`Boolean`

<br>

#### 重命名玩家  

`pl.rename(newname)`

- 参数：
  - newname : `String`  
    玩家的新名字  
- 返回值：是否重命名成功
- 返回值类型：`Boolean`

```clike
[Js]
//对于一个玩家对象pl
pl.rename("newname");
[Lua]

```

#### 查询玩家手中的物品  

`pl.getHand()`

- 返回值：玩家主手中的物品对象
- 返回值类型：`Item`

#### 查询玩家所有物品  

`pl.getAllItems()`

- 返回值：玩家所拥有的所有物品对象的集合
- 返回值类型：`Object<...>`

对于返回的某个物品对象集合`obj`，集合中有以下几种内容：

| 属性          | 含义                                     | 类型                   |
| ------------- | ---------------------------------------- | ---------------------- |
| obj.hand      | 玩家的主手物品                           | `Item`                 |
| obj.offHand   | 玩家的副手物品                           | `Item`                 |
| obj.inventory | 玩家物品栏中所有物品的数组（共40个物品） | `Array<Item,Item,...>` |
| obj.armor     | 玩家盔甲栏中所有物品的数组（共4个物品）  | `Array<Item,Item,...>` |
| obj.endchest  | 玩家末影箱中所有物品的数组（共30个物品） | `Array<Item,Item,...>` |

#### 修改玩家操作权限  

`pl.setPermLevel(level)`

- 参数：

  - level : `Integer`  
    目标操作权限等级  

    | 操作权限等级 | 对应操作权限    |
    | ------------ | --------------- |
    | 0            | 普通成员权限    |
    | 1            | OP权限          |
    | 4            | OP + 控制台权限 |

- 返回值：是否成功修改

- 返回值类型：`Boolean`

```clike
[Js]
//对于一个玩家对象pl
pl.setPermLevel(1);
[Lua]

```

#### 修改玩家游戏模式

`pl.setGameMode(mode)`

- 参数：

  - mode : `Integer`  
    目标游戏模式，0为生存模式，1为创造模式，2为极限模式

- 返回值：是否成功修改

- 返回值类型：`Boolean`

```clike
[Js]
//对于一个玩家对象pl
pl.setGameMode(1);
[Lua]

```

#### 提高玩家经验等级 

`pl.addLevel(count)`

- 参数：
  - count : `Integer`  
    要提升的经验等级
- 返回值：是否设置成功
- 返回值类型：`Boolean`

```clike
[Js]
//对于一个玩家对象pl
pl.addLevel(6);
[Lua]

```

#### 传送玩家至指定服务器  

`pl.transServer(server,port)`

- 参数：
  - server : `String`  
    目标服务器IP / 域名

  - port : `Integer`  
    目标服务器端口  
- 返回值：是否成功传送
- 返回值类型：`Boolean` 

```clike
[Js]
//对于一个玩家对象pl
pl.transServer("123.45.67.89",23333);
[Lua]

```

#### 使玩家客户端崩溃

`pl.crash()`

- 返回值：是否成功执行
- 返回值类型：`Boolean` 

```clike
//对于一个玩家对象pl
[Js]
pl.crash();
[Lua]
pl:crash()

```

#### 获取玩家对应的设备信息对象

`pl.getDevice()`

- 返回值：玩家对应的设备信息对象
- 返回值类型：`Device`

设备信息对象储存了与玩家设备有关的某些信息，如设备类型、网络延迟等信息。  
关于设备信息对象的其他信息请参考 [设备信息对象 API](zh_CN/Development/GameAPI/Device.md)

<br>

#### 获取玩家对应的NBT对象

`pl.getTag()`

- 返回值：玩家的NBT对象
- 返回值类型：`NbtCompound`

关于NBT对象的更多使用，请参考 [NBT接口文档](zh_CN/Development/NbtAPI/NBT.md)

<br>

#### 设置玩家自定义侧边栏

`pl.setSidebar(title,data)`

- 参数：

  - title : `String`  
    侧边栏标题  

  - data : `Object<String-Integer>`  
    侧边栏对象内容对象  
    对象中的每个键 - 值对将被设置为侧边栏内容的一行

- 返回值：是否成功设置

- 返回值类型：`Boolean`

```clike
[Js]
//对于一个玩家对象pl
pl.setSidebar("title",{"aaaa":3,"bbb":12,"cc":7});
[Lua]

```

#### 移除玩家自定义侧边栏

`pl.removeSidebar()`

- 返回值：是否成功移除
- 返回值类型：`Boolean`

```clike
[Js]
//对于一个玩家对象pl
pl.removeSidebar();
[Lua]

```

#### 设置玩家看到的自定义Boss血条  

`pl.setBossBar(title,percent)`

- 参数：
  - title : `String`  
    自定义血条标题  

  - percent : `Integer`  
    血条中的血量百分比，有效范围为0~100。0为空血条，100为满
- 返回值：是否成功设置
- 返回值类型：`Boolean`

```clike
[Js]
//对于一个玩家对象pl
pl.setBossBar("Hello ~ ",80);
[Lua]

```

#### 移除玩家的自定义Boss血条  

`pl.removeBossBar()`

- 返回值：是否成功移除
- 返回值类型：`Boolean`

```clike
[Js]
//对于一个玩家对象pl
pl.removeBossBar();
[Lua]

```

#### 删除玩家背包中的物品  

`pl.removeItem(inventoryId,count)`

- 参数：
  - inventoryId : `Integer`  
    背包格位ID  

  - count : `Integer`  
    要删除的物品数量
- 返回值：是否成功删除
- 返回值类型：`Boolean`