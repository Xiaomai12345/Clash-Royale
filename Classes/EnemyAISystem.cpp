#include "EnemyAISystem.h"
#include "BattleManager.h"
#include "Battlefield.h"
#include "UnitType.h"
#include "cocos2d.h"

USING_NS_CC;

bool EnemyAISystem::init()
{
    if (!Node::init())
        return false;

    // ======================
    // 初始化敌方卡池
    // ======================
    _availableUnitTypes =
    {
        UNIT_KNIGHT,
        UNIT_ARCHER,
        UNIT_GIANT,
        UNIT_VALKYRIE,
        UNIT_DRAGONBABY,
        UNIT_SKELETON,
        UNIT_SKELETON_LEGION
    };

    _currentMana = 5.0f;   // 初始圣水
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
    _isActive = true;
    _spawnTimer = 0.0f;
    _nextSpawnInterval = 3.0f;
}

void EnemyAISystem::stopAI()
{
    _isActive = false;
}

void EnemyAISystem::update(float dt)
{
    if (!_isActive || !_battleManager || !_battlefield)
        return;

    // 1️⃣ 圣水回复
    updateMana(dt);

    // 2️⃣ AI 行为计时
    _spawnTimer += dt;
    if (_spawnTimer >= _nextSpawnInterval)
    {
        tryDeployTroop();
    }
}

void EnemyAISystem::updateMana(float dt)
{
    if (_currentMana >= _maxMana)
        return;

    _currentMana += _manaRegenRate * dt;
    if (_currentMana > _maxMana)
        _currentMana = _maxMana;
}

void EnemyAISystem::tryDeployTroop()
{
    // ======================
    // 圣水检测
    // ======================
    const float kDeployCost = 4.0f;
    if (_currentMana < kDeployCost)
        return;

    // ======================
    // 随机单位
    // ======================
    int unitType = getRandomCardId();

    // ======================
    // 随机部署点
    // ======================
    Vec2 deployPos = getRandomDeployPosition();
    if (deployPos == Vec2::ZERO)
        return;

    // ======================
    // 执行部署
    // 阵营 2：敌方
    // ======================
    _battleManager->deployUnit(unitType, deployPos, 2);

    // ======================
    // 扣费 & 重置节奏
    // ======================
    _currentMana -= kDeployCost;
    _spawnTimer = 0.0f;
    _nextSpawnInterval = RandomHelper::random_real(2.0f, 5.0f);

    CCLOG("[EnemyAI] Deploy unit %d at (%.1f, %.1f)",
        unitType, deployPos.x, deployPos.y);
}

int EnemyAISystem::getRandomCardId()
{
    if (_availableUnitTypes.empty())
        return UNIT_KNIGHT;

    int index = RandomHelper::random_int(
        0,
        static_cast<int>(_availableUnitTypes.size()) - 1
    );

    return _availableUnitTypes[index];
}

Vec2 EnemyAISystem::getRandomDeployPosition()
{
    // ===== 1. 防御性检查 =====
    if (!_battlefield)
        return cocos2d::Vec2::ZERO;

    // ===== 2. 获取敌方区域（一定要先声明）=====
    const std::vector<Area>& enemyAreas = _battlefield->getEnemyarea();
    if (enemyAreas.empty())
        return cocos2d::Vec2::ZERO;

    // ===== 3. 随机一个区域索引 =====
    int areaIndex = cocos2d::RandomHelper::random_int(
        0,
        static_cast<int>(enemyAreas.size()) - 1
    );

    // ===== 4. 绑定区域（先声明再用）=====
    const Area& area = enemyAreas[areaIndex];

    // ===== 5. 区域内随机格子 =====
    int row = cocos2d::RandomHelper::random_int(
        area.leftBottom.y,
        area.rightTop.y
    );

    int col = cocos2d::RandomHelper::random_int(
        area.leftBottom.x,
        area.rightTop.x
    );

    // ===== 6. 转世界坐标 =====
    return _battlefield->gridToWorld(row, col);
}


