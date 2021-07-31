## 📦 方块相关事件

#### `"onBlockInteracted"` - 方块接受玩家互动

- 监听函数原型
  `function(player,block)`
- 参数：
  - player : `Player`  
    与方块互动的玩家对象

  - block : `Block`  
    被互动的方块对象

- 拦截事件：函数返回`false`

只有可以被互动的方块才会触发此事件，如木桶、信标、制图台、磨石等  
拦截事件对箱子、潜影盒、工作台无效

<br>

#### `"onBlockExploded"` - 方块被爆炸破坏

- 监听函数原型
  `function(block,source)`
- 参数：
  - block : `Block`  
    被爆炸破坏的方块对象
  - source : `Entity`  
    爆炸来源的实体对象
- 拦截事件：不可以拦截

<br>

#### `"onCmdBlockExecute"` - 命令方块执行命令

- 监听函数原型
  `function(cmd,pos)`
- 参数：
  - cmd : `String`  
    执行的命令

  - pos : `IntPos`  
    执行命令的命令方块坐标

- 拦截事件：函数返回`false`

<br>

#### `"onContainerChange"` - 容器被放入 / 取出物品

- 监听函数原型
  `function(player,container,slotNum,isPutIn,item)`

- 参数：
  - player : `Player`  
    操作容器的玩家对象
  - container : `Block`  
    被操作的容器的方块对象
  - slotNum : `Integer`  
    操作容器的格子位置（第slotNum个格子）
  - isPutIn : `Boolean`  
    是否为放入物品
    - 为 `true` 表示正在放入物品
    - 为 `false` 表示正在取出物品
  - item : `Item`  
    被放入 / 取出的物品对象

- 拦截事件：不可以拦截

<br>

#### `"onProjectileHit"` - 方块被弹射物击中

- 监听函数原型
  `function(block,pos,source)`
- 参数：
  - block : `Block`  
    被击中的方块对象
  - source : `Entity`  
    弹射物来源的实体对象
- 拦截事件：不可以拦截

<br>

#### `"onHopperSearchItem"` - 漏斗（漏斗矿车）检测可否吸取物品

- 监听函数原型
  `function(pos)`
- 参数：
  - pos : `FloatPos`  
    漏斗（漏斗矿车）所在的位置
- 拦截事件：函数返回`false`

注：在放置漏斗之后，会在服务端反复不断触发这个事件  
当你拦截事件之后，漏斗就会无法吸取这个物品

<br>

#### `"onHopperPushOut"` - 漏斗（漏斗矿车）输出物品

- 监听函数原型
  `function(pos)`
- 参数：
  - pos : `FloatPos`  
    漏斗（漏斗矿车）所在的位置
- 拦截事件：函数返回`false`

<br>

#### `"onPistonPush"` - 活塞推动

- 监听函数原型
  `function(pistonPos,block)`
- 参数：
  - pistonPos : `IntPos`  
    正在工作的活塞坐标
  - block : `Block`  
    被推动的方块对象
- 拦截事件：函数返回`false`

<br>

#### `"onFarmLandDecay"` - 耕地退化

- 监听函数原型
  `function(pos,entity)`
- 参数：
  - pos : `IntPos`  
    退化的耕地的坐标
  - entity : `Entity`  
    造成耕地退化的实体
- 拦截事件：函数返回`false`

<br>

#### "onUseFrameBlock" - 操作物品展示框

- 监听函数原型
  `function(player,block)`
- 参数：
  - player : `Player`  
    操作物品展示框的玩家对象
  - block : `Block`  
    被操作的物品展示框方块对象
- 拦截事件：函数返回`false`

注：操作包括放置物品、取下物品、旋转物品。