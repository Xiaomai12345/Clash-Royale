# Clash-Royale
同济大学2025年程序设计范式大作业——皇室战争

一.结构目录
```
ClashRoyaleProject/
├── Assets/
│   ├── Scripts/
│   │   ├── Core/
│   │   │   ├── GameManager.cs           # 游戏主控制器
│   │   │   ├── BattleManager.cs         # 对战管理
│   │   │   ├── WaterSystem.cs           # 圣水系统
│   │   │   └── TowerManager.cs          # 塔管理系统
│   │   ├── Cards/
│   │   │   ├── CardBase.cs              # 卡牌基类
│   │   │   ├── UnitCard.cs              # 单位卡
│   │   │   ├── BuildingCard.cs          # 建筑卡
│   │   │   ├── SpellCard.cs             # 法术卡
│   │   │   ├── CardDeck.cs              # 卡组管理
│   │   │   └── CardDatabase.cs          # 卡牌数据库
│   │   ├── Units/
│   │   │   ├── UnitBase.cs              # 单位基类
│   │   │   ├── MeleeUnit.cs             # 近战单位
│   │   │   ├── RangedUnit.cs            # 远程单位
│   │   │   ├── AirUnit.cs               # 空中单位
│   │   │   └── UnitSpawner.cs           # 单位生成器
│   │   ├── UI/
│   │   │   ├── UIManager.cs             # UI管理器
│   │   │   ├── MainMenuUI.cs            # 主菜单
│   │   │   ├── BattleUI.cs              # 对战UI
│   │   │   ├── CardSlotUI.cs            # 卡牌槽位
│   │   │   └── WaterBarUI.cs            # 圣水条
│   │   ├── Combat/
│   │   │   ├── DamageSystem.cs          # 伤害系统
│   │   │   ├── TargetFinder.cs          # 目标查找
│   │   │   ├── Pathfinding.cs           # 路径寻找
│   │   │   └── CollisionSystem.cs       # 碰撞检测
│   │   └── Utilities/
│   │       ├── AudioManager.cs          # 音频管理
│   │       ├── PoolManager.cs           # 对象池
│   │       └── Constants.cs             # 游戏常量
│   ├── Prefabs/
│   │   ├── Cards/                       # 卡牌预制体
│   │   ├── Units/                       # 单位预制体
│   │   ├── Buildings/                   # 建筑预制体
│   │   ├── Towers/                      # 塔预制体
│   │   └── UI/                          # UI预制体
│   ├── Scenes/
│   │   ├── MainMenu.unity               # 主菜单场景
│   │   └── BattleScene.unity            # 对战场景
│   └── Audio/
│       ├── Music/                       # 背景音乐
│       └── SFX/                         # 音效
├── Docs/
│   ├── 设计文档.md
│   ├── 卡牌数据表.xlsx
│   └── API文档.md
└── README.md
```
二.各源文件预期内容与任务
Core/ 核心系统

  1.GameManager.cs
  
    游戏全局状态管理 
    场景切换控制    
    游戏设置保存与加载
    
  2.BattleManager.cs
  
    对战流程控制（开始、暂停、结束）
    胜负判定逻辑
    计时器管理
    皇冠计数
    
  3.WaterSystem.cs
  
    圣水自动增长逻辑（每X秒增长1点）
    圣水上限管理（上限10点）
    圣水消耗与验证
    
  4.TowerManager.cs
  
    塔的生命值管理
    塔的摧毁判断
    公主塔与国王塔的逻辑区分
    
Cards/ 卡牌系统

  1.CardBase.cs（抽象类）
  
    卡牌通用属性：名称、消耗、描述
    使用卡牌的虚方法
    卡牌状态管理
    
  2.UnitCard.cs
  
    单位卡特定逻辑
    生成对应单位的预制体
    部署单位的坐标计算
    
  3.BuildingCard.cs
  
    建筑卡特定逻辑
    建筑持续时间管理
    建筑功能触发（如持续产出单位）
    
  4.SpellCard.cs
  
    法术卡特定逻辑
    范围伤害计算
    特效触发
    
  5.CardDeck.cs
  
    8张卡牌的卡组管理
    随机抽牌与循环逻辑
    当前手牌管理
    
  6.CardDatabase.cs
  
    所有卡牌的配置数据存储
    卡牌ID与预制体的映射
    卡牌属性配置（JSON/可配置）
    
Units/ 单位系统

  1.UnitBase.cs
  
    单位通用属性：生命值、攻击力、攻击速度、移动速度
    攻击逻辑基类
    死亡处理
    
  2.MeleeUnit.cs
  
    近战攻击逻辑
    攻击范围判定
    攻击动画控制
    
  3.RangedUnit.cs
  
    远程攻击逻辑
    投射物生成与跟踪
    对空/对地属性
    
  4.AirUnit.cs
  
    空中单位特殊逻辑
    飞行高度控制
    只受对空攻击影响
    
5.UnitSpawner.cs

    单位生成位置计算
    单位初始化配置
    单位回收管理
    
UI/ 用户界面

  1.UIManager.cs
  
    UI系统总控制
    界面切换管理
    事件响应处理
    
  2.MainMenuUI.cs
  
    主菜单按钮事件
    开始游戏流程
    退出游戏确认
    
  3.BattleUI.cs
  
    对战界面元素管理
    倒计时显示
    皇冠数量显示
    
  4.CardSlotUI.cs
  
    卡牌槽位显示
    拖拽部署逻辑
    卡牌冷却指示

  5.WaterBarUI.cs
  
    圣水条动态显示
    圣水数值更新
    颜色渐变效果
    
Combat/ 战斗系统

  1.DamageSystem.cs
  
    伤害计算公式
    暴击/特殊伤害处理
    伤害数值显示
    
  2.TargetFinder.cs
  
    自动寻找目标算法
    攻击优先级逻辑（最近/建筑优先）
    目标切换条件
    
  3.Pathfinding.cs
  
    A*或简化寻路算法
    双路线路径规划
    避障逻辑
    
  4.CollisionSystem.cs
  
    单位间碰撞检测
    攻击范围碰撞
    法术范围判定
    
Utilities/ 工具类

  1.AudioManager.cs
  
    背景音乐播放
    音效池管理
    音量控制
    
  2.PoolManager.cs
  
    游戏对象池（优化性能）
    单位/特效的复用
    内存管理
    
3.Constants.cs

    游戏常量定义
    配置参数
    枚举类型定义
    
三.建议分工

成员A：核心系统与战斗

主要负责：
  GameManager / BattleManager / TowerManager
  战斗系统（DamageSystem, TargetFinder, CollisionSystem）
  单位系统（所有Unit相关脚本）
  寻路与AI逻辑
  
辅助任务：
  协助卡牌系统的战斗逻辑
  性能优化（对象池）

成员B：卡牌系统与UI

主要负责：
  所有Card相关脚本（CardBase, UnitCard等）
  CardDeck和CardDatabase
  所有UI相关脚本（UIManager, BattleUI等）
  拖拽部署系统
  
辅助任务：
  协助单位生成逻辑
  UI特效和动画

成员C：基础框架与工具

主要负责：
  WaterSystem（圣水系统）
  音频系统（AudioManager）
  场景搭建和预制体管理
  工具类（Constants, PoolManager）
  文档维护
  
辅助任务：
  协助战斗逻辑实现
  测试和bug修复
