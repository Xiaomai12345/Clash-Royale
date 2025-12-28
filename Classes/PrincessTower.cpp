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
    _shouldRotate = false; // 塔本身不旋转
    _rooted = true;
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

    updateAssets();
}

void PrincessTower::setCamp(ECamp camp)
{
    ECamp oldCamp = _camp;
    BuildingBase::setCamp(camp); // 调用基类更新数据

	// 如果阵营改变了则刷新外观,是为了解决初始化时调用setCamp导致资源未加载的问题
    if (oldCamp != camp && _sprite != nullptr)
    {
        updateAssets();
    }
}

void PrincessTower::updateAssets()
{
    // 清理旧资源
    if (_sprite) {
        _sprite->removeFromParent();
        _sprite = nullptr;
    }
    if (_princessSprite) {
        _princessSprite->removeFromParent();
        _princessSprite = nullptr;
    }
    // 注意：AnimationComponent 会在析构时释放，这里 setAnimationComponent 会覆盖旧的

    // 塔身
    std::string towerImage = (_camp == ECamp::LEFT) 
        ? "Images/troops/Animations/BluePrincessTower.png" 
        : "Images/troops/Animations/RedPrincessTower.png";

    _sprite = Sprite::create(towerImage);
    if (_sprite)
    {
        addChild(_sprite, 0); // 塔身在下层
        _sprite->setScale(1.0f);
    }
    else
    {
        CCLOG("ERROR: Could not load tower image: %s", towerImage.c_str());
        // Fallback
        _sprite = Sprite::create("Images/towers/princess_tower_red.png");
        addChild(_sprite, 0);
    }

     //公主
    std::string princessImage = (_camp == ECamp::LEFT)
        ? "Images/troops/Animations/BluePrincess.png"
        : "Images/troops/Animations/RedPrincess.png";
    
    _princessSprite = Sprite::create(princessImage);
    if (_princessSprite)
    {
        addChild(_princessSprite, 1); // 公主在上层
        _princessSprite->setScale(1.0f);
        
        // 调整公主位置到塔顶
        _princessSprite->setPosition(Vec2(0, 40)); 

        
        auto anim = new AnimationComponent();
        anim->setTargetSprite(_princessSprite);
        setAnimationComponent(anim); // 这会替换掉旧的组件，BuildingBase 会处理旧组件的 delete

        // 添加攻击动画
        std::string attackPrefix = (_camp == ECamp::LEFT) ? "BluePrincessAttack" : "RedPrincessAttack";
        
        auto animation = Animation::create();
        for(int i = 1; i <= 2; ++i)
        {
            std::string frameName = StringUtils::format("Images/troops/Animations/%s(%d).png", attackPrefix.c_str(), i);
            animation->addSpriteFrameWithFile(frameName);
        }
        animation->setDelayPerUnit(0.1f);
        
        auto animate = Animate::create(animation);
        auto reset = CallFunc::create([this, princessImage](){
             if(_princessSprite) _princessSprite->setTexture(princessImage);
        });
        
        auto attackSeq = Sequence::create(
            animate,
            reset,
            DelayTime::create(MAX(0, _attackInterval - 0.2f)),
            nullptr
        );
        
        anim->addAction(State::ATTACKING, RepeatForever::create(attackSeq));
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