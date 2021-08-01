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

#### `"onProjectileHitEntity"` - 实体被弹射物击中

- 监听函数原型
  `function(entity,pos,source)`
- 参数：
  - entity: `Entity`  
    被击中的实体对象
  - source : `Entity`  
    发射的弹射物实体（如箭）
- 拦截事件：不可以拦截

<br>

#### "onWitherBossDestroy" - 凋零破坏方块

- 监听函数原型
  `function(witherBoss,AAbb,aaBB)`
- 参数：
  - witherBoss: `Entity`  
    凋零的实体对象

  - AAbb: `IntPos`
  
    凋零将破坏的区域（长方体），对角点A坐标
  
  - aaBB: `IntPos`
  
    凋零将破坏的区域（长方体），对角点B坐标
  
- 拦截事件：函数返回`false`

注意，此事件不包括凋零爆炸的破坏。

<br>

#### "onRide" - 生物骑乘

- 监听函数原型
  `function(entity1,entity2)`
- 参数：
  - entity1 : `Entity`  
    尝试骑乘的玩家对象
  - entity2 : `Entity`  
    被骑乘的实体对象
- 拦截事件：函数返回`false`

注：骑乘包括坐矿车、坐船、骑马、骑猪等。

<br>

#### "onStepOnPressurePlate" - 生物踩压力板

- 监听函数原型
  `function(entity,pressurePlate)`
- 参数：
  - entity : `Entity`  
    踩压力板的实体对象
  - pressurePlate : `Block`  
    被踩的压力板方块对象
- 拦截事件：函数返回`false`

注：生物踩压力板时，将会反复多次触发此事件。

<br>

#### "onSplashPotionHitEffect" - 实体喷溅药水效果影响

- 监听函数原型
  `function(entity1,entity2)`
- 参数：
  - entity1 : `Entity`  
    被喷溅药水效果影响的实体对象
  - entity2: `Entity`  
    投掷喷溅药水的实体对象
- 拦截事件：函数返回`false`

<br>

#### "onProjectileShoot" - 实体发射弹射物

- 监听函数原型
  `function(shooter,projectiler)`
- 参数：
  - shooter : `Entity`  
    发射弹射物的的实体对象
  - projectiler : `Entity`  
    被发射的实体对象
- 拦截事件：函数返回 `false`

注：丢药水也会触发此事件，但无法拦截。拦截丢药水请使用 `onSplashPotionHitEffect`

<br>