#include "KingdomTower.h"
#include "BuildingAttackComponent.h"
#include "SimpleBuildingAI.h"

USING_NS_CC;

KingdomTower::KingdomTower(float maxHp, float attackRange, float attackInterval, int attackDamage)
    : _attackRange(attackRange)
    , _attackInterval(attackInterval)
    , _attackDamage(attackDamage)
{
    _maxHp = maxHp;
    _hp = _maxHp;
    _bodyRadius = 30.0f;
    _camp = ECamp::LEFT;
    _moveAttack = MoveAttack::Both;
    _moveAttacked = MoveAttack::Both;
    _isDying = false;
}

bool KingdomTower::init()
{
    if (!BuildingBase::init())
        return false;

    if (_maxHp <= 0) _maxHp = 2000;
    _hp = _maxHp;

    setupComponents();

    return true;
}

void KingdomTower::setupComponents()
{
    auto ai = new SimpleBuildingAI();
    setAIComponent(ai);

    auto attack = new BuildingAttackComponent(
        _attackRange,
        _attackInterval,
        _attackDamage,
        500.0f
    );
    setAttackComponent(attack);

    _sprite = Sprite::create("Images/towers/king_tower_red.png");
    if (_sprite)
    {
        addChild(_sprite);
        _sprite->setScale(1.0f);
    }
    else
    {
        CCLOG("ERROR: Could not load Images/towers/king_tower_red.png");
    }
}
