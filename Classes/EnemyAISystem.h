#ifndef ENEMY_AI_SYSTEM_H
#define ENEMY_AI_SYSTEM_H

#include "cocos2d.h"
#include <vector>

class BattleManager;
class Battlefield;

/**
 * 敌方 AI 系统
 * 职责：
 * 1. 管理敌方圣水
 * 2. 定时尝试出兵
 * 3. 从卡池中随机选择单位
 * 4. 在敌方可部署区域随机落点
 */
class EnemyAISystem : public cocos2d::Node
{
public:
    CREATE_FUNC(EnemyAISystem);

    virtual bool init() override;
    virtual void update(float dt) override;

    void setBattleManager(BattleManager* manager);
    void setBattlefield(Battlefield* battlefield);

    void startAI();
    void stopAI();

private:
    // ======================
    // 内部逻辑
    // ======================
    void updateMana(float dt);
    void tryDeployTroop();

    cocos2d::Vec2 getRandomDeployPosition();
    int getRandomCardId();

private:
    // 外部系统引用
    BattleManager* _battleManager = nullptr;
    Battlefield* _battlefield = nullptr;

    // ======================
    // 圣水系统
    // ======================
    float _currentMana = 0.0f;
    float _maxMana = 10.0f;
    float _manaRegenRate = 1.0f;   // 每秒回复 1 点圣水

    // ======================
    // AI 行为节奏
    // ======================
    float _spawnTimer = 0.0f;
    float _nextSpawnInterval = 3.0f;
    bool  _isActive = false;

    // ======================
    // 敌方卡池（单位 ID）
    // ======================
    std::vector<int> _availableUnitTypes;
};

#endif // ENEMY_AI_SYSTEM_H
