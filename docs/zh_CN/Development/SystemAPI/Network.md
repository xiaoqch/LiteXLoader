## 🌏 网络接口 API

下面这些API为脚本提供了基本的网络接口。  
如果有更复杂的需求，可以使用各自语言平台的网络库来完成任务  

### 发送一个异步HTTP(s) Get请求  

`network.httpGet(url,callback)`

- 参数：
  - url : `String`  
    请求的目标地址（包括 Get 请求附带的参数）
  - callback : `Function`  
    当请求返回时执行的回调函数，用于回传HTTP(s)响应结果。
- 返回值：是否成功发送请求
- 返回值类型：`Boolean`

注：参数callback的回调函数原型：`function(status,result)`  

- status : `Integer`    
  返回的HTTP(s)响应码，如200代表请求成功
- result : `String`  
  返回的具体数据

如果请求执行失败，status值将为 -1

<br>

### 发送一个异步HTTP(s) Post请求  

`network.httpPost(url,data,type,callback)`

- 参数：
  - url : `String`  
    请求的目标地址
  - data : `String`  
    发送的数据
  - type : `String`  
    发送的 Post 数据类型，形如 `text/plain` `application/x-www-form-urlencoded` 等
  - callback : `Function`  
    当请求返回时执行的回调函数，用于回传HTTP(s)响应结果。
- 返回值：是否成功发送请求
- 返回值类型：`Boolean`

注：参数callback的回调函数原型：`function(status,result)`  

- status : `Integer`    
  返回的HTTP(s)响应码，如200代表请求成功
- result : `String`  
  返回的具体数据

如果请求执行失败，status值将为 -1

<br>