#include "PrincessTower.h"
#include "BuildingAttackComponent.h"
#include "SimpleBuildingAI.h"
#include"BattleManager.h"
USING_NS_CC;

PrincessTower::PrincessTower(float maxHp, float attackRange, float attackInterval, int attackDamage)
    : _attackRange(attackRange)
    , _attackInterval(attackInterval)
    , _attackDamage(attackDamage)
{
    // 构造函数中初始化
    _maxHp = maxHp;
    _hp = _maxHp;
    _bodyRadius = 25.0f;
    _camp = ECamp::LEFT;
    _moveAttack = MoveAttack::Both;
    _moveAttacked = MoveAttack::Both;
    _isDying = false;
}

bool PrincessTower::init()
{
    if (!BuildingBase::init())
    {
        CCLOG("PrincessTower::init - BuildingBase::init failed");
        return false;
    }

    // 再次确保血量正确 (防止基类覆盖)
    if (_maxHp <= 0) _maxHp = 1200;
    _hp = _maxHp;

    setupComponents();

    if (!_sprite)
    {
        CCLOG("PrincessTower::init - Sprite creation failed!");
    }
    else
    {
        CCLOG("PrincessTower created at (%.1f, %.1f), HP: %d/%d", getPositionX(), getPositionY(), _hp, _maxHp);
    }

    return true;
}


void PrincessTower::setupComponents()
{
    // 1. AI
    auto ai = new SimpleBuildingAI();
    setAIComponent(ai);

    // 2. Attack
    auto attack = new BuildingAttackComponent(
        _attackRange,
        _attackInterval,
        _attackDamage,
        500.0f
    );
    setAttackComponent(attack);

    // 3. Sprite
    _sprite = Sprite::create("Images/towers/princess_tower_red.png");
    if (_sprite)
    {
        addChild(_sprite);
        _sprite->setScale(1.0f);
    }
    else
    {
        CCLOG("ERROR: Could not load Images/towers/princess_tower_red.png");
    }
}

void PrincessTower::die()
{
    if (_isDying)
        return;

    _isDying = true;
    int playerID = (getCamp() == ECamp::LEFT) ? 0 : 1;
    float x = getPositionX();

    if (playerID == 0)
    {
        (x < 450)
            ? BattleManager::getInstance()->setMyLeftPrincessAlive(false)
            : BattleManager::getInstance()->setMyRightPrincessAlive(false);
    }
    else
    {
        (x < 450)
            ? BattleManager::getInstance()->setEnemyLeftPrincessAlive(false)
            : BattleManager::getInstance()->setEnemyRightPrincessAlive(false);
    }

    stopAllActions();
    unscheduleUpdate();

    _ai = nullptr;
    _attack = nullptr;

    runAction(Sequence::create(
        FadeOut::create(0.3f),
        RemoveSelf::create(true),
        nullptr
    ));
}