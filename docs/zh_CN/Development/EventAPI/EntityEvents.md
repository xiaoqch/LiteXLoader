## 🎈 实体相关事件

#### `"onMobDie"` - 生物死亡（包括玩家）

- 监听函数原型
  `function(mob,source)`
- 参数：
  - mob : `Entity`  
    死亡的实体对象

  - source : `Entity`  
    伤害来源的实体对象

- 拦截事件：函数返回`false`

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