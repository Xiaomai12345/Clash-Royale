#include "Cannon.h"
#include "BuildingAttackComponent.h"
#include "SimpleBuildingAI.h"

USING_NS_CC;

Cannon::Cannon(float maxHp, float attackRange, float attackInterval, int attackDamage)
    : _attackRange(attackRange)
    , _attackInterval(attackInterval)
    , _attackDamage(attackDamage)
{
    _maxHp = maxHp;
    _hp = _maxHp;
    _bodyRadius = 20.0f;
    _camp = ECamp::LEFT;
    _moveAttack = MoveAttack::Ground; // 只能攻击地面
    _moveAttacked = MoveAttack::Both;
    _isDying = false;
}

bool Cannon::init()
{
    if (!BuildingBase::init())
        return false;

    // 确保血量正确
    if (_maxHp <= 0) _maxHp = 1500;
    _hp = _maxHp;

    setupComponents();

    CCLOG("Cannon 初始化完成，位置：(%.0f, %.0f) HP: %d", getPositionX(), getPositionY(), _hp);

    return true;
}

void Cannon::setupComponents()
{
    // 设置AI组件
    auto ai = new SimpleBuildingAI();
    setAIComponent(ai);

    // 设置攻击组件
    auto attack = new BuildingAttackComponent(
        _attackRange,    // 攻击范围
        _attackInterval, // 攻击间隔
        _attackDamage,   // 攻击伤害 (使用成员变量)
        500.0f           // 弹道速度
    );
    setAttackComponent(attack);

    // 设置外观
    _sprite = Sprite::create("Images/Buildings/Cannon.jpg");
    if (_sprite)
    {
        addChild(_sprite);
        _sprite->setScale(0.07f);
    }
    else
    {
        CCLOG("ERROR: Cannon 图片加载失败 Images/Buildings/Cannon.jpg");
    }
}
