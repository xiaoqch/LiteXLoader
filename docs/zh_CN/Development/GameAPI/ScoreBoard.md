## 📝 计分板 API

下面这些API，提供了从代码层面原生操作计分板的接口

#### 计分板系统 - 新增计分项

`mc.newScoreObjective(name,displayName)`

- 参数：
  -  name : `String`  
     计分项名称  
  -  displayName : `String`  
     计分项显示名称  
- 返回值：是否设置成功
- 返回值类型：`Boolean`

此接口的作用类似命令 `/scoreboard objectives add <name> <displayName> dummy`

<br>

#### 计分板系统 - 移除计分项

`mc.removeScoreObjective(name)`

- 参数：
  -  name : `String`  
     计分项名称  
  
- 返回值：是否移除成功
- 返回值类型：`Boolean`

此接口的作用类似命令 `/scoreboard objectives remove <name>`

<br>

#### 计分板系统 - 获取所有计分项

`mc.listAllScoreObjective()`

- 返回值：计分板系统记录的所有计分项信息
- 返回值类型：`Object<String-String>`

此接口的作用类似命令 `/scoreboard objectives list`

对于返回的所有计分项信息`obj`，对象中有以下这些内容：

| 属性            | 含义             | 类型     |
| --------------- | ---------------- | -------- |
| obj.name        | 计分项的名称     | `String` |
| obj.displayName | 计分项的显示名称 | `String` |

<br>

#### 设置玩家计分项的值

`pl.setScore(name,value)`

- 参数：
  - name : `String`  
    计分项名称  

  - value : `Integer`  
    设置的数值  
- 返回值：是否设置成功
- 返回值类型：`Boolean`

使用前，必须保证对应的计分项已经存在

```clike
[Js]
//对于一个玩家对象pl
pl.setScore("money",10000);
[Lua]

```

#### 获取玩家计分项的值

`pl.getScore(name)`

- 参数：
  - name : `String`  
    计分项名称  
- 返回值：计分板上的数值
- 返回值类型：`Integer`

使用前，必须保证对应的计分项已经存在

```clike
[Js]
//对于一个玩家对象pl
log("You have money:",pl.getScore("money"));
[Lua]

```

#### 给玩家计分项加分 

`pl.addScore(name,value)`

- 参数：
  - name : `String`  
    计分项名称  

  - value : `Integer`  
    要增加的数值  
- 返回值：是否设置成功
- 返回值类型：`Boolean`

使用前，必须保证对应的计分项已经存在

```clike
[Js]
//对于一个玩家对象pl
pl.addScore(100);
[Lua]

```

#### 玩家停止跟踪计分项

`pl.removeScore(name)`

- 参数：
  - name : `String`  
    计分项名称  
- 返回值：是否移除成功
- 返回值类型：`Boolean`

使用前，必须保证对应的计分项已经存在

```clike
[Js]
//对于一个玩家对象pl
pl.removeScore("what");
[Lua]

```

