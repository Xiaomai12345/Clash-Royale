#ifndef __ENEMY_AI_SYSTEM_H__
#define __ENEMY_AI_SYSTEM_H__

#include "cocos2d.h"
#include"ManaSystem.h"
#include"CardManager.h"
#include <vector>

class BattleManager;
class Battlefield;
class EnemyManaSystem;

class EnemyAISystem : public cocos2d::Node
{
public:
    CREATE_FUNC(EnemyAISystem);

    static EnemyAISystem* getInstance();
    static void destroyInstance();
    virtual bool init() override;
    virtual void update(float dt) override;


    // 外部依赖注入
    void setBattleManager(BattleManager* manager);
    void setBattlefield(Battlefield* battlefield);

    // AI 控制
    void startAI();
    void stopAI();

private:
    // ======================
    // AI 核心逻辑
    // ======================
    void tryDeployTroop();
    int  selectUnitByStrategy() const;
    cocos2d::Vec2 getRandomDeployPosition() const;

private:
    // ======================
    // AI 状态
    // ======================
    bool  _isActive = false;
    float _thinkTimer = 0.0f;
    float _nextThinkInterval = -0.1f;

    // ======================
    // 系统依赖
    // ======================
    static EnemyAISystem* _instance;
    BattleManager* _battleManager = nullptr;
    Battlefield* _battlefield = nullptr;
    ManaSystem* _enemyMana = nullptr;
    CardManager* _enemyCard;
    // ======================
    // 敌方卡池（基础）
    // ======================

};

#endif // __ENEMY_AI_SYSTEM_H__
