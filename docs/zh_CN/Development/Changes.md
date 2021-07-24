# LiteXLoader - 重大更改页面

这里列出了LXL引擎对 **API** 进行的一些可能会造成 **不向下兼容** 的重大更改。  
如果你的插件在更新版本后发生问题，请首先查阅本章节的相关内容，检查是否由API变更导致。  
更改内容按时间顺序由新到旧展示。

#### 20210723 - v0.0.9

- 将`mc.runcmdEx`返回值对象中的`res.result`字段更名为`res.success`

#### 20210720 - v 0.0.9

- 将`onJoin`事件重命名为`onPreJoin`，同时新增好用的真正的`onJoin`事件

#### 20210719 - v 0.0.9

- `mc.newForm`更名为`mc.newCustomForm`，增加 `SimpleForm` 普通表单构建器
- `onUseItem`事件删除 block 参数，如果有需要监听点击方块请使用 `onBlockInteracted` 事件  
  useitem 事件不再会不停的刷了

#### 20210717 - v 0.0.8

- 适配 1.17.10
- 表单函数回调函数的参数内容发生较大变化

#### 20210714 - v 0.0.7

- 将事件`onOpenChest`和`onOpenBarrel`合并为`onOpenContainer`
- 将事件`onCloseChest`和`onCloseBarrel`合并为`onCloseContainer`
- 将事件`onChangeSlot`重命名为`onContainerChangeSlot`
- 将接口`file.read`重命名为`file.readFrom`，将接口`file.write`重命名为`file.writeTo`
- 通用日志接口的 日志输出等级 发生了较大变化
- 将事件`onServerCmd`重命名为`onConsoleCmd`

#### 20210713 - v 0.0.7

- `lxl.getVerion` 返回值类型变为`Object`
- `pl.sendSimpleForm` 函数增加 `images` 参数

#### 20210709 - v 0.0.7

- 删除注册自定义命令回调参数args中的第一个前缀项（即原来的args[0]）。  
  args数组从此只包含所注册的命令分割出的参数  
  具体详见命令系统API相关文档说明
- 将`IntPos`与`FloatPos`类型中的`dim`变量重命名为`dimid`，表意维度ID更加清晰