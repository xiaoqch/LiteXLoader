## 📡 系统调用 API

下面这些API提供了执行一些系统调用的接口

### 调用shell执行指定系统命令

`system.cmd(cmd,callback[,timeLimit])`

- 参数：
  - cmd : `String`  
    执行的系统命令
  - callback : `Function`  
    命令执行的进程结束之后返回数据使用的回调函数
  - timeLimit : `Integer`  
    （可选参数）运行命令进程运行的最长时间，单位为毫秒  
    默认为`-1`，即不限制最长运行时间
- 返回值：是否成功启动命令
- 返回值类型：`Boolean`

注：参数callback的回调函数原型：`function(exitcode,output)`  

- exitcode : `Integer`    
  进程退出码
- output : `Integer`  
  进程标准输出和标准错误输出的内容

注意！这里执行的不是MC命令系统的命令  
此函数不会等待系统执行完后再返回，而是命令执行完后由引擎自动调用给出的回调函数来实现返回结果，即所谓的异步执行

<br>