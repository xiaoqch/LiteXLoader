## 🏃‍♂️ 玩家对象 API

在LXL中，使用「玩家对象」来操作和获取某一个玩家的相关信息。

### 获取一个玩家对象

获取玩家对象有三种方式：

1. 通过注册**事件监听**函数，获取到BDS给出的与相关事件有关的玩家对象  
   详见[事件监听文档 - EventAPI](EventApi.md)

   

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

<br>

### 玩家对象 - 属性

每一个玩家对象都包含一些固定的对象属性。对于某个特定的玩家对象`pl`，有以下这些属性

| 属性         | 含义                                                     | 类型       |
| ------------ | -------------------------------------------------------- | ---------- |
| pl.name      | 玩家名                                                   | `String`   |
| pl.pos       | 玩家所在坐标                                             | `FloatPos` |
| pl.realName  | 玩家的真实名字（即使改名后也不变）                       | `String`   |
| pl.xuid      | 玩家Xuid                                                 | `String`   |
| pl.uuid      | 玩家Uuid                                                 | `String`   |
| pl.ip        | 玩家设备的IP地址，格式类似`12.34.567.89:1111`            | `String`   |
| pl.permLevel | 玩家的操作权限等级                                       | `Integer`  |
| pl.gameMode  | 玩家的游戏模式（0 - 生存，1 - 创造，2 - 极限，3 - 旁观） | `Integer`  |
| pl.maxHealth | 玩家最大生命值                                           | `Integer`  |
| pl.health    | 玩家当前生命值                                           | `Integer`  |
| pl.inAir     | 玩家当前是否悬空                                         | `Boolean`  |
| pl.sneaking  | 玩家当前是否正在潜行                                     | `Boolean`  |

这些对象属性都是只读的，无法被修改  
其中，**操作权限等级**属性的对照表如下：

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

#### 以这个玩家身份执行一条命令 

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

- 参数：
  - pos : `FloatPos`  
    目标位置坐标 
- 返回值：是否成功传送
- 返回值类型：`Boolean`

```clike
[Js]
//对于一个玩家对象pl，一个位置坐标对象pos
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

#### 查询玩家背包  

`pl.getPack()`

- 返回值：玩家背包中所有的物品对象
- 返回值类型：`Array<Item,Item,...>`
  - 返回值数组中物品对象的排列顺序和背包中的顺序一致

```clike
[Js]
//对于一个玩家对象pl
var pack = pl.getPack();
log(pack[0].name);
[Lua]

```

#### 获取玩家主手物品  

`pl.getHand()`

- 返回值：玩家当前主手持有的物品对象
- 返回值类型：`Item`

```clike
[Js]
//对于一个玩家对象pl
var hand = pl.getHand();
log(hand.name);
[Lua]

```

#### 获取玩家副手物品  

`pl.getOffHand()`

- 返回值：玩家当前副手持有的物品对象
- 返回值类型：`Item`

```clike
[Js]
//对于一个玩家对象pl
var offhand = pl.getOffHand();
log(offhand.name);
[Lua]

```

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

#### 获取玩家计分板值  

`pl.getScore(name)`

- 参数：
  - name : `String`  
    计分板名称  
- 返回值：计分板上的数值
- 返回值类型：`Integer`

```clike
[Js]
//对于一个玩家对象pl
log("You have money:",pl.getScore("money"));
[Lua]

```

#### 设置玩家计分板值  

`pl.setScore(name,value)`

- 参数：
  - name : `String`  
    计分板名称  

  - value : `Integer`  
    设置的数值  
- 返回值：是否设置成功
- 返回值类型：`Boolean`

如果你想删除这个计分板项，请在value参数传入`Null`

```clike
[Js]
//对于一个玩家对象pl
pl.setScore("money",10000);
[Lua]

```

#### 给玩家计分板项加分 

`pl.addScore(name,value)`

- 参数：
  - name : `String`  
    计分板名称  

  - value : `Integer`  
    要增加的数值  
- 返回值：是否设置成功
- 返回值类型：`Boolean`

```clike
[Js]
//对于一个玩家对象pl
pl.addScore(100);
[Lua]

```

#### 移除玩家计分板项

`pl.removeScore(name)`

- 参数：
  - name : `String`  
    计分板名称  

- 返回值：是否移除成功
- 返回值类型：`Boolean`

```clike
[Js]
//对于一个玩家对象pl
pl.removeScore("what");
[Lua]

```

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

------

## 