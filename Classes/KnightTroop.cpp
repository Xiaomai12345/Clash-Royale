#include "KnightTroop.h"
#include "SimpleTroopAIComponent.h"
#include "GroundMoveComponent.h"
#include "MeleeAttackComponent.h"
#include "AnimationComponent.h"
#include "DataManager.h"

KnightTroop::KnightTroop()
{
    // 在构造函数中给基类和成员变量赋值
    _moveSpeed = 100.0f;  
    _maxHp = 100;         
    _alertRange = 200.f;   
    _bodyRadius = 15.f;    
    _camp = ECamp::LEFT;   

    _attacktype = AttackType::Both; 
    _moveAttack = MoveAttack::Ground;  
    _moveAttacked = MoveAttack::Both; 
    _moveType = MoveType::Ground;    

    _isDying = false;  
}

KnightTroop::~KnightTroop()
{
    if (_ai) delete _ai;
    if (_move) delete _move;
    if (_attack) delete _attack;
}

bool KnightTroop::init()
{
    if (!TroopBase::init())  
        return false;

    // =========================
    // 0. 从 DataManager 获取数据
    // =========================
    float attackRange = 25.0f;
    float attackInterval = 1.0f;
    int attackDamage = 100;

    auto data = DataManager::getInstance()->getCardDataById(4);
    if (!data.empty())
    {
        // 覆盖基础属性
		CCLOG("骑士的数据加载成功，开始设置属性...");
        if (data.count("health")) {
            _maxHp = data["health"].asInt();
            _hp = _maxHp; // 重置当前血量
        }
        
        if (data.count("moveSpeed")) {
            // JSON 1.2 -> Game ~100. Multiplier 80.
            _moveSpeed = data["moveSpeed"].asFloat() * 40.5; 
            resetMoveSpeed();
        }
        
        if (data.count("viewRange")) {
            // JSON 5.5 -> Game ~220. Multiplier 40.
            _alertRange = data["viewRange"].asFloat() * 40.5f;
        }

        // 攻击属性
        if (data.count("attackRange")) {
            // JSON 1.2 -> Game ~25. Multiplier 20.
            attackRange = data["attackRange"].asFloat() * 20.25;
        }
        if (data.count("attackSpeed")) {
            attackInterval = data["attackSpeed"].asFloat();
        }
        if (data.count("attackPower")) {
            attackDamage = data["attackPower"].asInt();
        }

        CCLOG("KnightTroop Data Loaded: HP=%d, DMG=%d, Speed=%.1f", _maxHp, attackDamage, _moveSpeed);
    }
    else
    {
        CCLOG("KnightTroop Data Load Failed! Using defaults.");
    }

    auto ai = new SimpleTroopAIComponent();
    setAIComponent(ai);

    auto move = new GroundMoveComponent();
    setMoveComponent(move);

    auto attack = new MeleeAttackComponent(
        attackRange,   // 攻击范围
        attackInterval,   // 攻击间隔
        attackDamage      // 伤害值
    );
    setAttackComponent(attack);

    _sprite = Sprite::create("Images/troops/Animations/KnightMove.png"); 
    
    // 基础缩放
    const float baseScale = 0.8f; 

    if (_sprite)
    {
		CCLOG("KnightTroop: Sprite loaded successfully.");
        addChild(_sprite);  // 将图片添加到当前节点
        _sprite->setScale(baseScale);  // 保持缩放
    }
    else
    {
        CCLOG("KnightTroop: Sprite load failed!");
    }

    auto anim = new AnimationComponent();
    setAnimationComponent(anim);
    
    anim->setDefaultTexture("Images/troops/Animations/KnightMove.png");

    auto breath = Sequence::create(
        ScaleTo::create(1.0f, baseScale * 1.05f), 
        ScaleTo::create(1.0f, baseScale * 0.95f),
        nullptr
    );
    anim->addAction(State::IDLE, RepeatForever::create(breath));

    {
        Vector<SpriteFrame*> walkFrames;
        
        auto tex1 = Director::getInstance()->getTextureCache()->addImage("Images/troops/Animations/KnightMove.png");
        auto tex2 = Director::getInstance()->getTextureCache()->addImage("Images/troops/Animations/KnightMove2.png");
        
        if (tex1 && tex2)
        {
            auto frame1 = SpriteFrame::createWithTexture(tex1, Rect(0, 0, tex1->getContentSize().width, tex1->getContentSize().height));
            auto frame2 = SpriteFrame::createWithTexture(tex2, Rect(0, 0, tex2->getContentSize().width, tex2->getContentSize().height));
            
            walkFrames.pushBack(frame1);
            walkFrames.pushBack(frame2);
            
            auto walkAnim = Animation::createWithSpriteFrames(walkFrames, 0.5f);
            anim->addAnimation(State::FOLLOWING, walkAnim);
        }
        else
        {
             CCLOG("KnightTroop: Failed to load move animation textures.");
        }
    }

    {
        Vector<SpriteFrame*> attackFrames;
        
        // 加载攻击帧 (路径改为 Animations 下)
        auto texAtt1 = Director::getInstance()->getTextureCache()->addImage("Images/troops/Animations/KnightAttack1.png");
        auto texAtt2 = Director::getInstance()->getTextureCache()->addImage("Images/troops/Animations/KnightAttack2.png");
        
        if (texAtt1 && texAtt2)
        {
            auto att1 = SpriteFrame::createWithTexture(texAtt1, Rect(0, 0, texAtt1->getContentSize().width, texAtt1->getContentSize().height));
            auto att2 = SpriteFrame::createWithTexture(texAtt2, Rect(0, 0, texAtt2->getContentSize().width, texAtt2->getContentSize().height));
        
            attackFrames.pushBack(att1);
            attackFrames.pushBack(att2);
            
            float delayPerUnit = attackInterval / 2.0f;
            
            auto attackAnim = Animation::createWithSpriteFrames(attackFrames, delayPerUnit);
            anim->addAnimation(State::ATTACKING, attackAnim);
        }
        else
        {
            CCLOG("KnightTroop: Failed to load attack animation textures.");
        }
    }

    return true;
}

void KnightTroop::update(float dt)
{
    //  获取移动前位置
    Vec2 oldPos = getPosition();

    //  调用基类更新
    TroopBase::update(dt);

    //  获取移动后位置并计算方向
    Vec2 newPos = getPosition();
    Vec2 moveDir = newPos - oldPos;

    //  如果有移动，则更新朝向
    if (moveDir.lengthSquared() > 0.1f) // 阈值防抖
    {
        moveDir.normalize();
        // 数学角度转度数
        float angleDeg = CC_RADIANS_TO_DEGREES(moveDir.getAngle());
        
        // 转换公式：TargetRotation = 90 - MathAngle
        float targetRotation = 90.0f - angleDeg;
        
        if (_sprite)
        {
            // 平滑旋转 (Lerp)
            float currentRotation = _sprite->getRotation();
            
            // 处理角度突变 (如 350 -> 10)
            float diff = targetRotation - currentRotation;
            while (diff > 180) diff -= 360;
            while (diff < -180) diff += 360;

            // 插值系数
            float alpha = 10.0f * dt;
            if (alpha > 1.0f) alpha = 1.0f;

            _sprite->setRotation(currentRotation + diff * alpha);
        }
    }
}
