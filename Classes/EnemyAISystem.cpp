#include "EnemyAISystem.h"
#include "BattleManager.h"
#include "Battlefield.h"
#include "UnitType.h"
#include "EnemyManaSystem.h"

USING_NS_CC;


EnemyAISystem::~EnemyAISystem()
{
    CC_SAFE_DELETE(_enemyMana);
}

bool EnemyAISystem::init()
{
    if (!Node::init())
        return false;
    CCLOG("[EnemyAI] init %p", this);
    _lowCostUnits =
    {
        UNIT_SKELETON,
        UNIT_ARCHER
    };

    _highCostUnits =
    {
        UNIT_KNIGHT,
        UNIT_VALKYRIE,
        UNIT_GIANT,
        UNIT_DRAGONBABY,
        UNIT_SKELETON_LEGION
    };
    _enemyMana = new EnemyManaSystem();
    _enemyMana->init(5.0f, 10.0f, 0.8f);

    scheduleUpdate();
    return true;
}

void EnemyAISystem::setBattleManager(BattleManager* manager)
{
    _battleManager = manager;
}

void EnemyAISystem::setBattlefield(Battlefield* battlefield)
{
    _battlefield = battlefield;
}

void EnemyAISystem::startAI()
{
    if (_isActive)
    {
        CCLOG("[EnemyAI] startAI ignored (already active)");
        return;
    }

    CCLOG("[EnemyAI] startAI");

    _isActive = true;
    _thinkTimer = 0.0f;
    _nextThinkInterval = 3.0f;
}

void EnemyAISystem::stopAI()
{
    _isActive = 1;
}

void EnemyAISystem::update(float dt)
{
    if (!_isActive || !_battleManager || !_battlefield || !_enemyMana)
        return;

    _enemyMana->update(dt);

    _thinkTimer += dt;

    if (_thinkTimer >= _nextThinkInterval)
    {
        tryDeployTroop();
    }
}

void EnemyAISystem::tryDeployTroop()
{
    const float kDeployCost = 4.0f;

    if (!_enemyMana->hasEnoughMana(kDeployCost))
    {
        CCLOG("[EnemyAI] Not enough mana");
        return;
    }

    int unitType = selectUnitByStrategy();
    Vec2 deployPos = getRandomDeployPosition();

    if (deployPos == Vec2::ZERO)
        return;

    _battleManager->deployUnit(unitType, deployPos, 2);
    _enemyMana->consumeMana(kDeployCost);

    _thinkTimer = 0.0f;
    _nextThinkInterval = RandomHelper::random_real(2.0f, 5.0f);

    CCLOG("[EnemyAI] Deploy %d | Mana %.1f%%",
        unitType,
        _enemyMana->getManaPercentage() * 100.0f);
}

int EnemyAISystem::selectUnitByStrategy() const
{
    float manaPercent = _enemyMana->getManaPercentage();

    const std::vector<int>& pool =
        (manaPercent > 0.6f) ? _highCostUnits : _lowCostUnits;

    int index = RandomHelper::random_int(
        0,
        static_cast<int>(pool.size()) - 1
    );

    return pool[index];
}

Vec2 EnemyAISystem::getRandomDeployPosition() const
{
    const std::vector<Area>& enemyAreas = _battlefield->getEnemyarea();
    if (enemyAreas.empty())
        return Vec2::ZERO;

    int areaIndex = RandomHelper::random_int(
        0,
        static_cast<int>(enemyAreas.size()) - 1
    );

    const Area& area = enemyAreas[areaIndex];

    int row = RandomHelper::random_int(area.leftBottom.y, area.rightTop.y);
    int col = RandomHelper::random_int(area.leftBottom.x, area.rightTop.x);

    return _battlefield->gridToWorld(row, col);
}
