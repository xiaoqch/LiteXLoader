# LiteXLoader - GUI表单界面文档

> 这里API使你可以在游戏中创建、修改或者影响 **GUI界面**

很明显，清晰直观的UI界面有助于玩家快速上手插件的使用，提供良好的用户体验。

## 📊 表单相关 API

对于某个特定的玩家对象`pl`，有以下这些表单接口可用

### 向玩家发送简单表单  

`pl.sendSimpleForm(title,content,buttons,callback)`

- 参数：

  - title : `String`  
    表单标题  
  - content : `String`  
    表单内容
  - buttons : `Array<String,String,...>`  
    各个按钮文本的字符串数组  
  - callback : `Function`  
    玩家选择按钮之后被调用的回调函数。  
- 返回值：发送的表单ID  
- 返回值类型：`Integer`
  - 如果返回值为`Null`，则代表发送失败

注：参数callback的回调函数原型：`function(data)`  

- data : `String`    
  表单返回的互动结果字符串

<br>

### 向玩家发送模式对话框  

`pl.sendModalForm(title,content,button1,button2,callback)`

- 参数：
  - title : `String`  
    表单标题  
  - content : `String`  
    表单内容
  - button1 : `String`  
    按钮1文本的字符串  
  - button2 : `String`  
    按钮2文本的字符串  
  - callback : `Function`  
    玩家选择按钮之后被调用的回调函数。  
- 返回值：发送的表单ID  
- 返回值类型：`Integer`
  - 如果返回值为`Null`，则代表发送失败

注：参数callback的回调函数原型：`function(data)`  

- data : `String`    
  表单返回的互动结果字符串

<br>

### 向玩家发送自定义表单（Json格式）  

`pl.sendForm(json,callback)`

- 参数：
  - json : `String`  
    自定义表单json字符串  
  - callback : `Function`  
    玩家与表单元素互动之后被调用的回调函数。  
- 返回值：发送的表单ID  
- 返回值类型：`Integer`  
  - 如果返回值为`Null`，则代表发送失败

注：参数callback的回调函数原型：`function(data)`  

- data : `String`    
  表单返回的互动结果字符串

<br>

### 取消之前发送的表单  

`mc.cancelForm(id)`

- 参数：
  - id : `Integer`  
    上述这些函数返回的表单ID

- 返回值：是否成功取消  

- 返回值类型：`Boolean`  

<br>