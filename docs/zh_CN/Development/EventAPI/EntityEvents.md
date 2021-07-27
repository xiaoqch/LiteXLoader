## 🎈 实体相关事件

#### `"onMobDie"` - 生物死亡

- 监听函数原型
  `function(mob,source)`
- 参数：
  - mob : `Entity`  
    死亡的实体对象

  - source : `Entity`  
    伤害来源的实体对象
- 拦截事件：不可以拦截

注意，当玩家死亡时，除了触发`onPlayerDie`事件，这个事件同样也会被触发一次

<br>

#### `"onMobHurt"` - 生物受伤（包括玩家）

- 监听函数原型
  `function(mob,source,damage)`
- 参数：
  - mob : `Entity`  
    受伤的实体对象
  - source : `Entity`  
    伤害来源的实体对象
  - damage : `Integer`  
    受到的伤害数值

- 拦截事件：函数返回`false`

<br>

#### `"onExplode"` - 发生爆炸

- 监听函数原型
  `function(source,pos)`
- 参数：
  - source : `Entity`  
    爆炸来源的实体对象

  - pos : `FloatPos`  
    爆炸发生的坐标

- 拦截事件：函数返回`false`

<br>

#### "onWitherBossDestroy" - 凋零破坏方块

- 监听函数原型
  `function(block)`
- 参数：
  - block : `Block`  
    将要破坏的方块对象

- 拦截事件：函数返回`false`

注意，此事件包括凋零之首的破坏。另外，此事件的方块对象无法取得坐标。

<br>