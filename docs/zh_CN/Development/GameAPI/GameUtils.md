## 🎮 游戏实用工具 API

下面这些API提供了某些帮助游戏内容开发的实用工具接口，让开发效率可以得到一定的提高。

### 格式化代码实用工具

MC通过 **格式化代码** 符号在游戏内提供颜色文字、样式修改等格式控制能力。格式化代码分为颜色代码与格式代码两种，分别提供颜色控制和样式控制两种能力。  
此处的接口为了帮助开发者快速上手MC格式化代码，而不需要频繁查表才能使用  
注意：此处接口仅有在传递到BDS中并显示时才会产生效果

#### 颜色代码表

| 颜色代码符号            | 表示的颜色意义              | 游戏内对应的字符串 |
| ----------------------- | --------------------------- | ------------------ |
| `Text.Black`            | 接下来的文字为 **黑色**     | §0                 |
| `Text.DarkBlue`         | 接下来的文字为 **深蓝色**   | §1                 |
| `Text.DarkGreen`        | 接下来的文字为 **深绿色**   | §2                 |
| `Text.DarkAqua	`     | 接下来的文字为 **湖蓝色**   | §3                 |
| `Text.DarkRed`          | 接下来的文字为 **深红色**   | §4                 |
| `Text.DarkPurple`       | 接下来的文字为 **紫色**     | §5                 |
| `Text.Gold`             | 接下来的文字为 **金色**     | §6                 |
| `Text.Gray`             | 接下来的文字为 **灰色**     | §7                 |
| `Text.DarkGray`         | 接下来的文字为 **深灰色**   | §8                 |
| `Text.Blue`             | 接下来的文字为 **蓝色**     | §9                 |
| `Text.Green`            | 接下来的文字为 **浅绿色**   | §a                 |
| `Text.Aqua`             | 接下来的文字为 **天蓝色**   | §b                 |
| `Text.Red`              | 接下来的文字为 **浅红色**   | §c                 |
| `Text.LightPurple`      | 接下来的文字为 **浅紫色**   | §d                 |
| `Text.Yellow`           | 接下来的文字为 **浅黄色**   | §e                 |
| `Text.White`            | 接下来的文字为 **白色**     | §f                 |
| `Text.MinecoinGold	` | 接下来的文字为 **硬币金色** | §g                 |

<br>

#### 格式代码表

| 格式代码符号         | 表示的格式意义                  | 游戏内对应的字符串 |
| -------------------- | ------------------------------- | ------------------ |
| `Text.Bold`          | 接下来的文字 **加粗**           | §l                 |
| `Text.Italics`       | 接下来的文字 *斜体*             | §o                 |
| `Text.Underline`     | 接下来的文字 <u>下划线</u>      | §n                 |
| `Text.StrikeThrough` | 接下来的文字 ~~删除线~~         | §m                 |
| `Text.Random`        | 接下来的文字 随机闪烁的乱码字符 | §k                 |
| `Text.Clear`         | 接下来的文字 清除所有格式       | §r                 |

<br>

上述代码使用的方法为连接字符串，类似于：

```clike
[Js]
mc.broadcast(Text.Red + Text.Bold + "Red & Bold " + Text.DarkGreen + Text.Underline + "DarkGreen & Underline" + Text.Clear + "Clear");
[Lua]
mc.broadcast(Text.Red .. Text.Bold .. "Red & Bold " .. Text.DarkGreen .. Text.Underline .. "DarkGreen & Underline" .. Text.Clear .. "Clear")
```
