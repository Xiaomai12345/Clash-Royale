#ifndef ENEMY_MANA_SYSTEM_H
#define ENEMY_MANA_SYSTEM_H

#include "cocos2d.h"
#include <unordered_map>

class EnemyManaSystem
{
public:
    enum class ManaMode
    {
        NORMAL = 0,
        DOUBLE = 1,
        SUDDEN_DEATH = 2
    };

public:
    EnemyManaSystem();          // ? public
    ~EnemyManaSystem();

    // 初始化
    void init(float initialMana = 5.0f,
        float maxMana = 10.0f,
        float regenRate = 0.8f);

    // 每帧更新
    void update(float dt);

    // 消耗 / 判断
    bool consumeMana(float amount);
    bool hasEnoughMana(float cost) const;

    // 数值接口
    float getCurrentMana() const { return _currentMana; }
    float getMaxMana() const { return _maxMana; }
    float getManaPercentage() const;

    // 模式
    void setManaMode(ManaMode mode);

private:
    float _currentMana = 0.0f;
    float _maxMana = 0.0f;
    float _baseRegenRate = 0.0f;
    float _currentRegenRate = 0.0f;

    ManaMode _currentMode = ManaMode::NORMAL;
    bool _isEnabled = false;

    std::unordered_map<ManaMode, float> _modeMultipliers;
};

#endif // ENEMY_MANA_SYSTEM_H
