# LiteXLoader - GUI API
> 这里是一些在游戏中创建、修改或者影响界面时会用到的函数  

## 玩家相关 API

### 设置玩家自定义侧边栏计分板  
`setScoreBoard(player,title,data)`
- 参数：
    1. player : `Pointer`  
    玩家指针
    2. title : `String`  
    侧边栏标题  
    3. data : `Array<String,String,...>`  
    列表字符串数组  
- 返回值：是否成功设置
- 返回值类型：`Boolean`  
<br>

### 移除玩家自定义侧边栏计分板  
`setScoreBoard(player)`
- 参数：
    1. player : `Pointer`  
    玩家指针
- 返回值：是否成功移除
- 返回值类型：`Boolean`  
<br>

### 向玩家发送简单表单  
`sendSimpleForm(player,title,content,buttons,callback)`
- 参数：
    1. player : `Pointer`  
    玩家指针
    2. title : `String`  
    表单标题  
    3. content : `String`  
    表单内容
    4. buttons : `Array<String,String,...>`  
    各个按钮文本的字符串数组  
    5. callback : `Function`  
    玩家选择按钮之后被调用的回调函数。  
    函数原型：
- 返回值：发送的表单ID  
获取被选择的按扭项请使用`OnSelectFrom`事件监听
- 返回值类型：`Number`  
<br>

### 向玩家发送模式对话框  
`sendModelForm(player,title,content,button1,button2,callback)`
- 参数：
    1. player : `Pointer`  
    玩家指针
    2. title : `String`  
    表单标题  
    3. content : `String`  
    表单内容
    4. button1 : `String`  
    按钮1文本的字符串  
    5. button2 : `String`  
    按钮2文本的字符串  
    6. callback : `Function`  
    玩家选择按钮之后被调用的回调函数。  
    函数原型：
- 返回值：发送的表单ID  
获取被选择的按扭项请使用`OnSelectFrom`事件监听
- 返回值类型：`Number`  
<br>

### 向玩家发送自定义表单  
`sendModelForm(player,json,callback)`
- 参数：
    1. player : `Pointer`  
    玩家指针
    2. json : `String`  
    自定义表单json字符串  
    3. callback : `Function`  
    玩家选择按钮之后被调用的回调函数。  
    函数原型：
- 返回值：发送的表单ID  
获取被选择的按扭项请使用`OnSelectFrom`事件监听
- 返回值类型：`Number`  
<br>

### 取消之前发送的表单  
`giveUpForm(id)`
- 参数：
    1. id : `Number`  
    上述这些函数返回的表单ID
- 返回值：是否成功取消  
- 返回值类型：`Boolean`  
<br>

### 为玩家设置自定义Boss血条  
`setBossBar(player,title,percent)`
- 参数：
    1. player : `Pointer`  
    玩家指针
    2. title : `String`  
    自定义血条标题  
    3. percent : `Number`  
    血条中的血量百分比，范围为0~100。0为空血条，100为满
- 返回值：是否成功设置
- 返回值类型：`Boolean`  
<br>

### 移除玩家的自定义Boss血条  
`removeBossBar(player)`
- 参数：
    1. player : `Pointer`  
    玩家指针
- 返回值：是否成功移除
- 返回值类型：`Boolean`  
<br>

## 表单协助 API
> 这些函数可以协助开发者方便地构造一个表单，不再需要为修改Json麻烦而头疼