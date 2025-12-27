#include "KingdomTower.h"
#include "BuildingAttackComponent.h"
#include "SimpleBuildingAI.h"
#include"BattleManager.h"
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

    // 国王塔不需要旋转
    _shouldRotate = false;
    _rooted = true;

    setupComponents();
    return true;
}

void KingdomTower::setupComponents()
{
    // 国王塔没有攻击组件，也没有AI（不会主动寻找目标）
    // 仅负责显示
    updateAssets();
}

void KingdomTower::setCamp(ECamp camp)
{
    ECamp oldCamp = _camp;
    BuildingBase::setCamp(camp);

    if (oldCamp != camp && _sprite != nullptr)
    {
        updateAssets();
    }
}

void KingdomTower::updateAssets()
{
    // 清理旧资源
    if (_sprite) {
        _sprite->removeFromParent();
        _sprite = nullptr;
    }
    if (_kingSprite) {
        _kingSprite->removeFromParent();
        _kingSprite = nullptr;
    }

    // 1. 塔身
    std::string towerImage = (_camp == ECamp::LEFT)
        ? "Images/troops/Animations/BlueKingTower.png"
        : "Images/troops/Animations/KingTower.png"; // 假设 KingTower.png 是红色的

    _sprite = Sprite::create(towerImage);
    if (_sprite)
    {
        addChild(_sprite, 0); // 底层
        _sprite->setScale(1.0f);
    }
    else
    {
        CCLOG("ERROR: Could not load king tower image: %s", towerImage.c_str());
    }

    // 2. 国王
    std::string kingImage = (_camp == ECamp::LEFT)
        ? "Images/troops/Animations/BlueKing.png"
        : "Images/troops/Animations/RedKing.png";

    _kingSprite = Sprite::create(kingImage);
    if (_kingSprite)
    {
        addChild(_kingSprite, 1); // 上层
        _kingSprite->setScale(1.0f);
        
        // 调整国王位置 (塔顶)
        // 根据素材情况，可能需要微调
        _kingSprite->setPosition(Vec2(0, 30)); 
    }
}

void KingdomTower::die()
{
    if (_isDying)
        return;

    _isDying = true;

    int playerID = (getCamp() == ECamp::LEFT) ? 0 : 1;
    float x = getPositionX();

    if (playerID == 0)
    {
        BattleManager::getInstance()->setMyKingAlive(false);
    }
    else
    {
        BattleManager::getInstance()->setEnemyKingAlive(false);
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