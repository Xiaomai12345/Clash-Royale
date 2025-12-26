#include "EnemyAISystem.h"
#include "BattleManager.h"
#include "Battlefield.h"
#include "UnitType.h"
#include<random>
USING_NS_CC;

EnemyAISystem* EnemyAISystem::_instance = nullptr;
EnemyAISystem* EnemyAISystem::getInstance()
{
    if (!_instance)
    {
        _instance = new EnemyAISystem();
    }
    return _instance;
}

void EnemyAISystem::destroyInstance()
{
    if (_instance)
    {
        delete _instance;
        _instance = nullptr;
    }
}

bool EnemyAISystem::init()
{
    if (!Node::init())
        return false;
    CCLOG("[EnemyAI] init %p", this);
    _enemyMana = ManaSystem::getEnemyInstance();
    _enemyCard = CardManager::getEnemyInstance();
    _enemyMana->init();
    _enemyCard->init();
    //scheduleUpdate();
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
    _isActive = false;
}

void EnemyAISystem::update(float dt)
{

    if (!_isActive || !_battleManager || !_battlefield || !_enemyMana)
        return;
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

    int index = selectUnitByStrategy();

    Vec2 deployPos = getRandomDeployPosition();

    if (deployPos == Vec2::ZERO)
        return;
    Card* card = _enemyCard->getHandCards()[index];

    
    _enemyMana->consumeMana(card->getManaCost());

    // 真正部署单位（关键）
    bool success = card->use(deployPos, /* enemyId */ 2);
    if (!success)
        return;

    // ③ 从手牌移除 / 循环
    _enemyCard->useCard(card);


    _thinkTimer = 0.0f;
    _nextThinkInterval = RandomHelper::random_real(2.0f, 5.0f);

}

int EnemyAISystem::selectUnitByStrategy() const
{
    // 随机打乱牌组
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<> dist(0, 3);  // 生成0-3的随机整数
    int index = dist(g);
    return index;
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
