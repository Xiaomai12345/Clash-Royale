# Clash-Royale
同济大学2025年程序设计范式大作业——皇室战争

一.结构目录
```
ClashRoyale_UE5/
├── Source/
│   ├── ClashRoyale.Target.cs
│   ├── ClashRoyaleEditor.Target.cs
│   └── ClashRoyale/
│       ├── ClashRoyale.Build.cs
│       ├── ClashRoyale.cpp
│       ├── ClashRoyale.h
│       ├── Public/
│       │   ├── Game/
│       │   │   ├── CRGameModeBase.h
│       │   │   ├── CRGameState.h
│       │   │   ├── CRPlayerController.h
│       │   │   └── CRPlayerState.h
│       │   ├── Actors/
│       │   │   ├── Units/
│       │   │   │   ├── CRUnitBase.h
│       │   │   │   ├── MeleeUnit.h
│       │   │   │   ├── RangedUnit.h
│       │   │   │   ├── FlyingUnit.h
│       │   │   │   └── GiantUnit.h
│       │   │   ├── Buildings/
│       │   │   │   ├── TowerBase.h
│       │   │   │   ├── KingTower.h
│       │   │   │   ├── PrincessTower.h
│       │   │   │   └── SpawnBuilding.h
│       │   │   └── Projectiles/
│       │   │       ├── ProjectileBase.h
│       │   │       └── ArrowProjectile.h
│       │   ├── Components/
│       │   │   ├── Combat/
│       │   │   │   ├── HealthComponent.h
│       │   │   │   ├── AttackComponent.h
│       │   │   │   └── TargetFinderComponent.h
│       │   │   ├── Cards/
│       │   │   │   ├── CardComponent.h
│       │   │   │   ├── DeckComponent.h
│       │   │   │   └── HandComponent.h
│       │   │   ├── UI/
│       │   │   │   ├── WaterComponent.h
│       │   │   │   └── HUDWidget.h
│       │   │   └── Movement/
│       │   │       └── LaneMovementComponent.h
│       │   ├── Data/
│       │   │   ├── CardData.h
│       │   │   ├── UnitData.h
│       │   │   └── GameplayTags.h
│       │   ├── UI/
│       │   │   ├── CRHUD.h
│       │   │   ├── CardSlotWidget.h
│       │   │   ├── WaterBarWidget.h
│       │   │   └── MainMenuWidget.h
│       │   ├── AI/
│       │   │   ├── UnitAIController.h
│       │   │   └── BehaviorTree/
│       │   │       ├── BTService_FindTarget.h
│       │   │       └── BTTask_Attack.h
│       │   └── Systems/
│       │       ├── CardSystem.h
│       │       ├── CombatSystem.h
│       │       ├── WaterSystem.h
│       │       └── SpawnSystem.h
│       └── Private/
│           ├── Game/
│           │   ├── CRGameModeBase.cpp
│           │   ├── CRGameState.cpp
│           │   └── CRPlayerController.cpp
│           ├── Actors/
│           │   ├── Units/
│           │   ├── Buildings/
│           │   └── Projectiles/
│           ├── Components/
│           ├── UI/
│           ├── AI/
│           └── Systems/
├── Content/
│   ├── Maps/
│   │   ├── MainMenu.umap
│   │   └── BattleMap.umap
│   ├── Data/
│   │   ├── DataTables/
│   │   │   ├── DT_Cards.udatatable
│   │   │   └── DT_Units.udatatable
│   │   └── JSON/
│   │       ├── cards.json
│   │       └── units.json
│   └── Assets/
│       ├── Meshes/
│       ├── Materials/
│       ├── Textures/
│       └── Audio/
└── Config/
```
1. 游戏核心（Game/）
   
CRGameModeBase.h

```
cpp
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CRGameModeBase.generated.h"

UCLASS()
class CLASHROYALE_API ACRGameModeBase : public AGameModeBase
{
    GENERATED_BODY()
    
public:
    ACRGameModeBase();
    
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    
    // 游戏流程控制
    void StartMatch();
    void EndMatch(ETeam Winner);
    void CheckWinCondition();
    
    // 圣水管理
    void AddWater(ETeam Team, float Amount);
    bool ConsumeWater(ETeam Team, int32 Amount);
    
private:
    UPROPERTY()
    class UWaterSystem* WaterSystem;
    
    UPROPERTY()
    class UCombatSystem* CombatSystem;
    
    float MatchDuration;
    float CurrentTime;
};
```
CRGameState.h
```
cpp
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CRTypes.h"
#include "CRGameState.generated.h"

UCLASS()
class CLASHROYALE_API ACRGameState : public AGameStateBase
{
    GENERATED_BODY()
    
public:
    // 皇冠计数
    int32 GetCrownCount(ETeam Team) const { return CrownCounts[Team]; }
    void AddCrown(ETeam Team, int32 Count);
    
    // 塔状态
    TArray<class ATowerBase*> GetTeamTowers(ETeam Team) const;
    
    // 游戏时间
    float GetRemainingTime() const { return RemainingTime; }
    
private:
    TMap<ETeam, int32> CrownCounts;
    TMap<ETeam, TArray<ATowerBase*>> TeamTowers;
    float RemainingTime;
};
```
2. 单位系统（Actors/Units/）

CRUnitBase.h
```
cpp
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CRTypes.h"
#include "CRUnitBase.generated.h"

UCLASS(abstract)
class CLASHROYALE_API ACRUnitBase : public ACharacter
{
    GENERATED_BODY()
    
public:
    ACRUnitBase();
    
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    
    // 战斗接口
    virtual void Attack(AActor* Target);
    virtual void TakeDamage(float Damage, EDamageType DamageType);
    
    // 移动接口
    void SetTargetLane(ELane Lane);
    void MoveToLocation(const FVector& Location);
    
    // 属性获取
    ETeam GetTeam() const { return Team; }
    EUnitType GetUnitType() const { return UnitType; }
    bool IsAlive() const { return CurrentHealth > 0; }
    
protected:
    // 组件
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UHealthComponent* HealthComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UAttackComponent* AttackComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UTargetFinderComponent* TargetFinderComponent;
    
    // 基础属性
    UPROPERTY(EditDefaultsOnly, Category = "Unit")
    ETeam Team;
    
    UPROPERTY(EditDefaultsOnly, Category = "Unit")
    EUnitType UnitType;
    
    UPROPERTY(EditDefaultsOnly, Category = "Unit")
    float MoveSpeed;
    
    UPROPERTY(EditDefaultsOnly, Category = "Unit")
    float AttackRange;
    
    UPROPERTY(EditDefaultsOnly, Category = "Unit")
    float AttackSpeed;
    
    float CurrentHealth;
    AActor* CurrentTarget;
    ELane CurrentLane;
};
```
3. 卡牌系统（Systems/CardSystem.h）
```
cpp
#pragma once
#include "CoreMinimal.h"
#include "CardData.h"
#include "CardSystem.generated.h"

UCLASS()
class CLASHROYALE_API UCardSystem : public UObject
{
    GENERATED_BODY()
    
public:
    void Initialize();
    
    // 卡牌管理
    FCardData* GetCardData(FName CardID);
    TArray<FCardData> GetDeckCards(int32 DeckID);
    
    // 卡牌使用
    bool CanPlayCard(ETeam Team, FName CardID);
    void PlayCard(ETeam Team, FName CardID, const FVector& Location);
    
    // 卡组循环
    void DrawNextCard(ETeam Team);
    TArray<FCardData> GetCurrentHand(ETeam Team);
    
private:
    UPROPERTY()
    class UDataTable* CardDataTable;
    
    TMap<ETeam, TArray<FCardData>> TeamDecks;
    TMap<ETeam, TArray<FCardData>> TeamHands;
    
    void LoadCardData();
    void InitializeDecks();
};
```
4. 战斗系统（Systems/CombatSystem.h）
```
cpp
#pragma once
#include "CoreMinimal.h"
#include "CombatSystem.generated.h"

UCLASS()
class CLASHROYALE_API UCombatSystem : public UObject
{
    GENERATED_BODY()
    
public:
    // 伤害计算
    static float CalculateDamage(float BaseDamage, EDamageType DamageType, 
                                 EArmorType ArmorType, float RangeMultiplier = 1.0f);
    
    // 范围伤害
    static void ApplyAreaDamage(const FVector& Origin, float Radius, 
                                float Damage, ETeam InstigatorTeam, 
                                EDamageType DamageType);
    
    // 目标查找
    static AActor* FindBestTarget(const FVector& Origin, float Range, 
                                  ETeam SeekingTeam, ETargetPriority Priority);
    
    // 碰撞检测
    static bool CheckLineOfSight(const AActor* Source, const AActor* Target);
    
    // 攻击预测
    static FVector PredictTargetLocation(const AActor* Target, float ProjectileSpeed, 
                                         const FVector& LaunchLocation);
};
```
5. UI 系统（UI/）
   
CRHUD.h
```
cpp
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CRHUD.generated.h"

UCLASS()
class CLASHROYALE_API ACRHUD : public AHUD
{
    GENERATED_BODY()
    
public:
    virtual void BeginPlay() override;
    
    // 创建UI
    void CreateMainMenu();
    void CreateBattleHUD();
    void ShowGameOverScreen(ETeam Winner);
    
    // 更新UI
    void UpdateWaterBar(ETeam Team, float Current, float Max);
    void UpdateCardSlots(ETeam Team, const TArray<FCardData>& HandCards);
    void UpdateTimer(float RemainingTime);
    void UpdateCrowns(ETeam Team, int32 Count);
    
private:
    // Widget引用
    UPROPERTY()
    class UMainMenuWidget* MainMenuWidget;
    
    UPROPERTY()
    class UBattleHUDWidget* BattleHUDWidget;
    
    UPROPERTY()
    class UCardSlotWidget* CardSlotWidgets[4]; // 4张手牌
    
    UPROPERTY()
    class UWaterBarWidget* WaterBarWidget;
};
```
CardSlotWidget.h
```
cpp
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Input/DragAndDrop.h"
#include "CardSlotWidget.generated.h"

UCLASS()
class CLASHROYALE_API UCardSlotWidget : public UUserWidget
{
    GENERATED_BODY()
    
public:
    void SetCardData(const FCardData& CardData);
    void UpdateCooldown(float Progress);
    void SetAvailable(bool bAvailable);
    
protected:
    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, 
                                          const FPointerEvent& InMouseEvent) override;
    virtual void NativeOnDragDetected(const FGeometry& InGeometry, 
                                     const FPointerEvent& InMouseEvent, 
                                     UDragDropOperation*& OutOperation) override;
    
private:
    FCardData CurrentCardData;
    bool bIsOnCooldown;
    float CooldownProgress;
    
    // UI元素绑定
    class UTextBlock* CardNameText;
    class UTextBlock* WaterCostText;
    class UImage* CardIcon;
    class UProgressBar* CooldownBar;
};
```
成员A：游戏核心和框架
负责模块：
```
GameMode, GameState, PlayerController

游戏规则和流程控制

圣水系统

网络同步（如需）

核心文件：

CRGameModeBase.h/cpp

CRGameState.h/cpp

WaterSystem.h/cpp

CRTypes.h（所有枚举和数据结构）
```
成员B：单位和战斗系统
负责模块：
```
所有Unit类（CRUnitBase及派生类）

战斗系统（CombatSystem）

伤害计算和碰撞

AI控制器

核心文件：

CRUnitBase.h/cpp

CombatSystem.h/cpp

HealthComponent.h/cpp

UnitAIController.h/cpp
```
成员C：卡牌和UI系统
负责模块：
```
卡牌系统（CardSystem）

数据管理（DataTables）

UMG Widgets（纯C++实现）

输入和拖拽系统

核心文件：

CardSystem.h/cpp

CardData.h

CRHUD.h/cpp

所有Widget类（.h/cpp）
```
