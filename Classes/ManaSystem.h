#ifndef MANA_SYSTEM_H
#define MANA_SYSTEM_H

#include "cocos2d.h"

class ManaSystem
{
public:
    enum class ManaMode
    {
        NORMAL = 0,
        DOUBLE = 1,
        SUDDEN_DEATH = 2
    };

    // 获取单例
    static ManaSystem* getInstance();
    static void destroyInstance();

    // 初始化
    void init(float initialMana = 5.0f, float maxMana = 10.0f, float regenRate = 0.8f);

    // 更新（每帧调用）
    void update(float delta);

    // 获取当前圣水
    float getCurrentMana() const { return _currentMana; }

    // 获取最大圣水
    float getMaxMana() const { return _maxMana; }

    // 消耗圣水
    bool consumeMana(float amount);

    // 增加圣水
    void addMana(float amount);

    // 设置圣水模式
    void setManaMode(ManaMode mode);

    // 获取圣水恢复速率
    float getRegenRate() const;

    // 重置圣水系统
    void reset();

    // 是否足够圣水
    bool hasEnoughMana(float cost) const;

    // 获取圣水百分比（用于UI显示）
    float getManaPercentage() const;

    // 启用/禁用系统
    void setEnabled(bool enabled) { _isEnabled = enabled; }
    bool isEnabled() const { return _isEnabled; }

private:
    ManaSystem();
    ~ManaSystem();

    // 删除拷贝构造和赋值
    ManaSystem(const ManaSystem&) = delete;
    ManaSystem& operator=(const ManaSystem&) = delete;

private:
    static ManaSystem* _instance;

    float _currentMana;
    float _maxMana;
    float _baseRegenRate;
    float _currentRegenRate;
    ManaMode _currentMode;
    bool _isEnabled;

    std::unordered_map<ManaMode, float> _modeMultipliers;
};

#endif // MANA_SYSTEM_H