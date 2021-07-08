## 💻 服务器设置 API

下面这些API提供了自定义某些服务器设置的接口

### 设置服务器Motd字符串  

`mc.setMotd(motd)`

- 参数：
  - motd : `String`  
    目标Motd字符串  
- 返回值：是否设置成功
- 返回值类型：`Boolean`

<br>

### 设置服务器自定义在线人数 

`mc.setOnlinePlayer(nowCount,maxCount)`

- 参数：
  - nowCount : `Integer`  
    当前在线人数  
  - maxCount : `Integer`  
    最大在线人数  
- 返回值：是否设置成功
- 返回值类型：`Boolean`

<br>