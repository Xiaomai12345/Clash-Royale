#include "EnemyManaSystem.h"

USING_NS_CC;

EnemyManaSystem::EnemyManaSystem()
{
    _modeMultipliers[ManaMode::NORMAL] = 1.0f;
    _modeMultipliers[ManaMode::DOUBLE] = 2.0f;
    _modeMultipliers[ManaMode::SUDDEN_DEATH] = 2.0f;
}

EnemyManaSystem::~EnemyManaSystem()
{
}

void EnemyManaSystem::init(float initialMana, float maxMana, float regenRate)
{
    _currentMana = initialMana;
    _maxMana = maxMana;
    _baseRegenRate = regenRate;
    _currentRegenRate = regenRate;
    _currentMode = ManaMode::NORMAL;
    _isEnabled = true;
}

void EnemyManaSystem::update(float dt)
{
    if (!_isEnabled)
        return;

    _currentMana += _currentRegenRate * dt;

    if (_currentMana > _maxMana)
        _currentMana = _maxMana;
}

bool EnemyManaSystem::consumeMana(float amount)
{
    if (_currentMana < amount)
        return false;

    _currentMana -= amount;
    return true;
}

bool EnemyManaSystem::hasEnoughMana(float cost) const
{
    return _currentMana >= cost;
}

float EnemyManaSystem::getManaPercentage() const
{
    return _maxMana > 0 ? _currentMana / _maxMana : 0.0f;
}

void EnemyManaSystem::setManaMode(ManaMode mode)
{
    _currentMode = mode;

    auto it = _modeMultipliers.find(mode);
    if (it != _modeMultipliers.end())
    {
        _currentRegenRate = _baseRegenRate * it->second;
    }
}
