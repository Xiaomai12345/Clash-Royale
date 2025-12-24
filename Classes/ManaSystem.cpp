#include "ManaSystem.h"

USING_NS_CC;

// 静态成员初始化
ManaSystem* ManaSystem::_instance = nullptr;
ManaSystem* ManaSystem::_enemyInstance = nullptr;
ManaSystem* ManaSystem::getInstance()
{
    if (!_instance)
    {
        _instance = new ManaSystem();
    }
    return _instance;
}

ManaSystem* ManaSystem::getEnemyInstance()
{
    if (!_enemyInstance)
    {
        _enemyInstance = new ManaSystem();
    }
    return _instance;
}

void ManaSystem::destroyInstance()
{
    if (_instance)
    {
        delete _instance;
        _instance = nullptr;
    }
}

void ManaSystem::destroyEnemyInstance()
{
    if (_enemyInstance)
    {
        delete _enemyInstance;
        _enemyInstance = nullptr;
    }
}
ManaSystem::ManaSystem()
    : _currentMana(50.0f)
    , _maxMana(100.0f)
    , _baseRegenRate(0.8f)
    , _currentRegenRate(0.8f)
    , _currentMode(ManaMode::NORMAL)
    , _isEnabled(false)
{
    // 初始化模式倍率
    _modeMultipliers[ManaMode::NORMAL] = 1.0f;
    _modeMultipliers[ManaMode::DOUBLE] = 2.0f;
    _modeMultipliers[ManaMode::TRIPLE] = 3.0f;
}

ManaSystem::~ManaSystem()
{
}

void ManaSystem::init(float initialMana, float maxMana, float regenRate)
{
    _currentMana = initialMana;
    _maxMana = maxMana;
    _baseRegenRate = regenRate;
    _currentRegenRate = regenRate;
    _isEnabled = true;

    CCLOG("ManaSystem initialized: %.1f/%.1f, regen: %.1f/s",
        _currentMana, _maxMana, _currentRegenRate);
}

void ManaSystem::update(float delta)
{
    if (!_isEnabled) return;

    // 恢复圣水
    float regenAmount = _currentRegenRate * delta;
    _currentMana += regenAmount;

    // 限制不超过最大值
    if (_currentMana > _maxMana)
    {
        _currentMana = _maxMana;
    }

    // 限制不低于0
    if (_currentMana < 0)
    {
        _currentMana = 0;
    }
}

bool ManaSystem::consumeMana(float amount)
{
    if (!_isEnabled) return false;

    if (_currentMana >= amount)
    {
        _currentMana -= amount;
        return true;
    }

    return false;
}

void ManaSystem::addMana(float amount)
{
    if (!_isEnabled) return;

    _currentMana += amount;

    if (_currentMana > _maxMana)
    {
        _currentMana = _maxMana;
    }
}

void ManaSystem::setManaMode(ManaMode mode)
{
    _currentMode = mode;

    auto it = _modeMultipliers.find(mode);
    if (it != _modeMultipliers.end())
    {
        _currentRegenRate = _baseRegenRate * it->second;
    }
}

float ManaSystem::getRegenRate() const
{
    return _currentRegenRate;
}

void ManaSystem::reset()
{
    _currentMana = 5.0f;
    _currentMode = ManaMode::NORMAL;
    _currentRegenRate = _baseRegenRate;
    _isEnabled = true;
}

bool ManaSystem::hasEnoughMana(float cost) const
{
    return _currentMana >= cost;
}

float ManaSystem::getManaPercentage() const
{
    return _maxMana > 0 ? _currentMana / _maxMana : 0.0f;
}