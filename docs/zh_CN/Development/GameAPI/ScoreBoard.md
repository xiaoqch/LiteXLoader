## 📝 计分板 API

下面这些API，提供了从代码层面原生操作计分板的接口

#### 计分板系统 - 计分项对象

##### 获取计分项对象

`mc.getScoreObjective(name)`

- 参数：
  - name : `String`  
    计分项名称（非显示名称）
- 返回值：该计分项的计分项对象
- 返回值类型：`Objective`

如计分项不存在则返回`null`

<br>

##### 成员常量

- `objective.name` 该计分项的名称
- `objective.displayName` 该计分项的显示名称

##### 成员函数

##### 计分项对象 - 获取分数

`objective.getScore(target)`

- 参数：
  - target : `String`  
    计分项中的目标，可以是任意字符串
- 返回值：该目标/玩家在此计分项中的分数
- 返回值类型：`Integer`

<br>

##### 计分项对象 - 修改分数

设置分数 `objective.setScore(target,score)`  
增加分数 `objective.addScore(target,score)`  
减少分数 `objective.removeScore(target,score)`

- 参数：
  - target : `String`  
    计分项中的目标，可以是任意字符串
  - score : `Integer`  
    要设置/增加/减少的分数
- 返回值：该目标在经过设置/增加/减少操作后的分数
- 返回值类型：`Integer`

如失败则返回`0`，但不保证返回`0`一定失败

<br>

##### 计分项对象 - 删除分数

`objective.deleteScore(target)`

- 参数：
  - target : `String`  
    计分项中的目标，可以是任意字符串
- 返回值：是否删除成功
- 返回值类型：`Boolean`

删除分数后，计分项中将不在有该目标的分数(与`reset`命令不同)

<br>

##### 计分项对象 - 设置显示

`objective.setDisplay(slot[,sortOrder=0])`

- 参数：
  - slot : `String`   
    显示槽位名称，可以为`sidebar`/`belowname`/`list`
  - sortOrder : `Integer`
    排序方式，可以为`0`(升序)或`1`(降序)
- 返回值：是否设置成功
- 返回值类型：`Boolean`

<br>

#### 计分板系统 - 新增计分项

`mc.newScoreObjective(name,displayName)`

- 参数：
  -  name : `String`  
     计分项名称  
  -  displayName : `String`  
     计分项显示名称  
- 返回值：新增计分项的计分项对象
- 返回值类型：`Objective`

如果新增失败则返回`null`

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

`mc.getAllScoreObjective()`

- 返回值：计分板系统记录的所有计分项对象
- 返回值类型：`Objective`

此接口的作用类似命令 `/scoreboard objectives list`

<br>

#### 计分板系统 - 获取正在显示的计分项

`mc.getDisplayObjective(slot)`

- 参数：
  - slot : `String`   
    显示槽位名称，可以为`sidebar`/`belowname`/`list`
- 返回值：正在`slot`槽位显示的计分项
- 返回值类型：`Objective`

如无正在对应槽位显示的计分项则返回`null`

<br>

#### 计分板系统 - 清除正在显示的计分项

`mc.clearDisplayObjective(slot)`

- 参数：
  - slot : `String`   
    显示槽位名称，可以为`sidebar`/`belowname`/`list`

- 返回值：是否清除成功
- 返回值类型：`Boolean`

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

