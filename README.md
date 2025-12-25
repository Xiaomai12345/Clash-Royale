# Clash-Royale
同济大学2025年程序设计范式大作业——皇室战争

一.结构目录
```
ClashRoyale/
├── Classes/
│   ├── Core/
│   │   ├── AppDelegate.h
│   │   ├── AppDelegate.cpp
│   │   ├── GameManager.h
│   │   ├── GameManager.cpp
│   │   ├── AudioManager.h
│   │   ├── AudioManager.cpp
│   │   └── GlobalDefines.h
│   ├── Scenes/
│   │   ├── MainMenuScene.h
│   │   ├── MainMenuScene.cpp
│   │   ├── BattleScene.h
│   │   └── BattleScene.cpp
│   ├── Entities/
│   │   ├── Card/
│   │   │   ├── Card.h
│   │   │   ├── Card.cpp
│   │   │   ├── UnitCard.h
│   │   │   ├── UnitCard.cpp
│   │   │   ├── BuildingCard.h
│   │   │   ├── BuildingCard.cpp
│   │   │   ├── SpellCard.h
│   │   │   └── SpellCard.cpp
│   │   ├── Unit/
│   │   │   ├── UnitBase.h
│   │   │   ├── UnitBase.cpp
│   │   │   ├── Knight.h
│   │   │   ├── Knight.cpp
│   │   │   ├── Archer.h
│   │   │   ├── Archer.cpp
│   │   │   ├── Giant.h
│   │   │   ├── Giant.cpp
│   │   │   ├── BabyDragon.h
│   │   │   ├── BabyDragon.cpp
│   │   │   ├── GoblinThrower.h
│   │   │   ├── GoblinThrower.cpp
│   │   │   ├── SkeletonArmy.h
│   │   │   └── SkeletonArmy.cpp
│   │   ├── Tower/
│   │   │   ├── TowerBase.h
│   │   │   ├── TowerBase.cpp
│   │   │   ├── KingTower.h
│   │   │   ├── KingTower.cpp
│   │   │   ├── PrincessTower.h
│   │   │   └── PrincessTower.cpp
│   │   ├── Building/
│   │   │   ├── BuildingBase.h
│   │   │   ├── BuildingBase.cpp
│   │   │   ├── TeslaTower.h
│   │   │   ├── TeslaTower.cpp
│   │   │   ├── BarbarianHut.h
│   │   │   └── BarbarianHut.cpp
│   │   └── Battlefield.h
│   │   └── Battlefield.cpp
│   ├── Managers/
│   │   ├── BattleManager.h
│   │   ├── BattleManager.cpp
│   │   ├── CardManager.h
│   │   ├── CardManager.cpp
│   │   ├── NetworkManager.h
│   │   ├── NetworkManager.cpp
│   │   ├── LevelManager.h
│   │   └── LevelManager.cpp
│   ├── Systems/
│   │   ├── ManaSystem.h
│   │   ├── ManaSystem.cpp
│   │   ├── CollisionSystem.h
│   │   ├── CollisionSystem.cpp
│   │   ├── PathfindingSystem.h
│   │   ├── PathfindingSystem.cpp
│   │   ├── AISystem.h
│   │   └── AISystem.cpp
│   ├── UI/
│   │   ├── Button.h
│   │   ├── Button.cpp
│   │   ├── ManaBar.h
│   │   ├── ManaBar.cpp
│   │   ├── HandCardUI.h
│   │   ├── HandCardUI.cpp
│   │   ├── CountdownLabel.h
│   │   └── CountdownLabel.cpp
│   └── Utils/
│       ├── Singleton.h
│       ├── Singleton.cpp
│       ├── ConfigLoader.h
│       ├── ConfigLoader.cpp
│       └── MathUtils.h
├── Resources/
│   ├── Images/
│   │   ├── background/
│   │   │   ├── main_menu.jpg
│   │   │   ├── battlefield.jpg
│   │   │   ├── river.png
│   │   │   ├── bridge.png
│   │   │   ├── tree.png
│   │   │   └── rock.png
│   │   ├── units/
│   │   │   ├── knight.png
│   │   │   ├── archer.png
│   │   │   ├── giant.png
│   │   │   ├── baby_dragon.png
│   │   │   ├── goblin_thrower.png
│   │   │   └── skeletons.png
│   │   ├── towers/
│   │   │   ├── king_tower_blue.png
│   │   │   ├── king_tower_red.png
│   │   │   ├── princess_tower_blue.png
│   │   │   └── princess_tower_red.png
│   │   ├── buildings/
│   │   │   ├── tesla_tower.png
│   │   │   └── barbarian_hut.png
│   │   ├── cards/
│   │   │   ├── card_back.png
│   │   │   ├── knight_card.png
│   │   │   ├── archer_card.png
│   │   │   └── ...
│   │   └── ui/
│   │       ├── button_normal.png
│   │       ├── button_pressed.png
│   │       ├── mana_background.png
│   │       ├── mana_bar.png
│   │       ├── mana_border.png
│   │       ├── mana_icon.png
│   │       ├── crown.png
│   │       └── card_back.png
│   ├── Sounds/
│   │   ├── bgm/
│   │   │   ├── menu_bgm.mp3
│   │   │   └── battle_bgm.mp3
│   │   └── sfx/
│   │       ├── deploy.wav
│   │       ├── attack.wav
│   │       ├── hit.wav
│   │       ├── explosion.wav
│   │       ├── tower_destroyed.wav
│   │       ├── game_start.wav
│   │       ├── victory.wav
│   │       └── defeat.wav
│   ├── Particles/
│   │   ├── explosion.plist
│   │   ├── mana_glow.plist
│   │   ├── damage.plist
│   │   └── heal.plist
│   ├── Fonts/
│   │   ├── arial.ttf
│   │   └── game_font.ttf
│   └── Config/
│       ├── GameConfig.plist
│       ├── CardData.json
│       ├── UnitData.json
│       ├── TowerData.json
│       └── BuildingData.json
├── proj.win32/
│   ├── ClashRoyale.vcxproj
│   ├── ClashRoyale.vcxproj.filters
│   ├── main.cpp
│   ├── main.h
│   └── Resource.rc
├── CMakeLists.txt
└── README.md
```

成员A：游戏核心和框架
负责模块：
```

游戏规则和流程控制

圣水系统

网络同步（如需）


```
成员B：单位和战斗系统
负责模块：
```

战斗系统（CombatSystem）

伤害计算和碰撞

AI控制器

```
成员C：
负责模块：
```

数据管理

Loading界面和MainMenu界面

UI系统

音乐播放器系统

```
