## 💰 经济系统 API

在很多服务器中，经济系统是非常关键的一环。  
为了解决传统使用计分板经济系统的种种问题，LXL提供了对接LLMoney经济系统的接口，可以与其他各系列插件数据互通。  
LLMoney除了拥有传统经济系统的能力之外，还有查询金额变动历史、操作离线玩家经济等额外能力。  
当然，在使用经济系统接口前，你需要提前在LiteLoader**安装好**LLMoney插件。否则，这些接口都将无法正常工作。  

注意：为了可以操作离线玩家的钱包，经济系统接口统一使用Xuid来作为玩家的统一标识符，而非其他地方通用的`Player`玩家指针。对于某个玩家指针`pl`，你可以使用`pl.xuid`来获取他的xuid字符串，并将其作为参数传入。

### 设置玩家的存款金额

`money.set(xuid,money)`

- 参数：
  - xuid : `String`  
    要操作的玩家的Xuid标识符
  - money : `Integer`  
    要设置的金额  
- 返回值：是否设置成功
- 返回值类型：`Boolean`

<br>

### 获取玩家的存款金额

`money.get(xuid)`

- 参数：
  - xuid : `String`  
    要读取的玩家的Xuid标识符
- 返回值：玩家的资金数值
- 返回值类型：`Integer`

<br>

### 增加玩家的存款

`money.add(xuid,money)`

- 参数：
  - xuid : `String`  
    要操作的玩家的Xuid标识符
  - money : `Integer`  
    要增加的金额  
- 返回值：是否设置成功
- 返回值类型：`Boolean`

<br>

### 减少玩家的存款

`money.reduce(xuid,money)`

- 参数：
  - xuid : `String`  
    要操作的玩家的Xuid标识符
  - money : `Integer`  
    要减小的金额  
- 返回值：是否设置成功
- 返回值类型：`Boolean`

<br>

### 进行一笔转账

`money.trans(xuid1,xuid2,money[,note])`

- 参数：
  - xuid1 : `String`  
    付款的玩家的Xuid标识符
  - money : `Integer`  
    要支付的金额  
  - xuid2 : `String`  
    收款的玩家的Xuid标识符
  - note : `String`  
    （可选参数）给这笔转账附加一些文字说明
- 返回值：是否转账成功
- 返回值类型：`Boolean`

<br>

### 查询历史账单

`money.getHistory(xuid,time)`

- 参数：
  - xuid : `String`  
    要操作的玩家的Xuid标识符
  - time : `Integer`  
    查询从现在开始往前time秒的记录
- 返回值：查询结果对象
- 返回值类型：`Boolean`

<br>

### 删除账单历史记录

`money.clearHistory(time)`

- 参数：
  - time : `Integer`  
    删除从现在开始往前time秒的记录
- 返回值：是否删除成功
- 返回值类型：`Boolean`

<br>