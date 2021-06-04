# LiteXLoader - LXL安装与使用指南

[<< 返回起始页](../)

## 🎨 简介

`LiteXLoader`（以下简称**LXL**）是一个基岩版官方服务端 `Bedrock Delicated Server`（以下简称**BDS**）的插件框架，提供强大的跨语言脚本插件支持能力和稳定的开发API支持。  

## 💻 安装

`LiteXLoader` 主要发布于**MineBBS**。在安装 `LiteXLoader` 之前，你需要先配置好他依赖的 `LiteLoader` 加载器环境。

- 请[移步MineBBS](https://www.minebbs.com/)下载对应版本的 `LiteLoader` ，按要求解压之后运行rodb.exe生成相关数据

在完成上述操作，并确认安装成功后，接下来安装`LiteXLoader` 

1. [移步MineBBS](https://www.minebbs.com/)下载对应版本的`LiteXLoader`，解压
2. 把全部内容复制到 **BDS** 的`plugins`目录中。如果复制时提示文件有冲突，选择覆盖即可。

安装完毕！接下来，你就可以安装你想要的 **LXL** 插件了

## 🎯 加载插件

1. 下载你需要加载的脚本插件
2. 把插件直接放置到`plugins`目录中
3. 开服！

> 没错，，就这么简单~

## 🔨 配置文件

`LiteXLoader`的配置文件位于`BDS根目录/plugins/LiteXLoader/config.ini`  
正常情况下，这些设置并不需要改动。

```ini
[Main]
LxlLogLevel=1				# LXL日志等级，默认为1，即不显示debug信息
PluginsDir=./plugins		# LXL插件目录，默认为plugins
DependsDir=./plugins/lib	# LXL依赖库目录，默认为plugins/lib

[Advanced]
GCInterval=20				# 引擎垃圾回收周期，默认为20秒
```



## 📡 实时调试

成功安装 **LXL** 之后，在 **BDS** 后台控制台执行

- `jsdebug`  
  进入Js实时调试模式
- `luadebug`  
  进入Lua实时调试模式

实时调试模式下，标准输入会被当做对应类型的脚本语言执行，并实时输出结果。  
如果发生错误，引擎将输出错误信息与堆栈跟踪信息。  
再次输入对应的`jsdebug`或`luadebug`将退出实时调试模式。

<br>

[<< 返回起始页](../)