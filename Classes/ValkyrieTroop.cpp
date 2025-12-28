#include "ValkyrieTroop.h"
#include "DataManager.h"
#include "SimpleTroopAIComponent.h"
#include "GroundMoveComponent.h"
#include "MeleeAreaAttackComponent.h"
#include "AnimationComponent.h"
#include "cocos2d.h"

USING_NS_CC;

ValkyrieTroop::ValkyrieTroop()
{
    // =========================
    // 基础属性
    // =========================
    _moveSpeed = 90.0f;    // 比骑士略慢
    _maxHp = 800;      // 女武神血厚
    _alertRange = 220.f;
    _bodyRadius = 10.f;

    _camp = ECamp::LEFT;

    _attacktype = AttackType::Both;
    _moveAttack = MoveAttack::Ground;
    _moveAttacked = MoveAttack::Both;
    _moveType = MoveType::Ground;

    _isDying = false;
}

ValkyrieTroop::~ValkyrieTroop()
{
    if (_ai)     delete _ai;
    if (_move)   delete _move;
    if (_attack) delete _attack;
}

bool ValkyrieTroop::init()
{
    if (!TroopBase::init())
        return false;

    // =========================
    // 0. DataManager 加载数据
    // =========================
    float attackRange = 45.0f; // AOE Radius
    float attackInterval = 1.2f;
    float attackDamage = 200.0f;

    auto data = DataManager::getInstance()->getCardDataByName("valkyrie");
    if (!data.empty())
    {
        if (data.count("health")) {
            _maxHp = data["health"].asInt();
            _hp = _maxHp;
        }
        if (data.count("moveSpeed")) {
            _moveSpeed = data["moveSpeed"].asFloat() * 40.25;
            resetMoveSpeed();
        }
        if (data.count("viewRange")) {
            _alertRange = data["viewRange"].asFloat() * 40.5f;
        }
        if (data.count("attackRange")) {
            attackRange = data["attackRange"].asFloat() * 30.25;
        }
        if (data.count("attackSpeed")) {
            attackInterval = data["attackSpeed"].asFloat();
        }
        if (data.count("attackPower")) {
            attackDamage = data["attackPower"].asFloat();
        }
        CCLOG("Valkyrie Data Loaded: HP=%d, DMG=%.1f", _maxHp, attackDamage);
    }

    // =========================
    // 1. 组件绑定
    // =========================

    // AI
    auto ai = new SimpleTroopAIComponent();
    setAIComponent(ai);

    // 地面移动
    auto move = new GroundMoveComponent();
    setMoveComponent(move);

    // 近战范围攻击（核心区别）
    auto attack = new MeleeAreaAttackComponent(
        attackRange,   // 攻击范围（AOE 半径）
        attackInterval,    // 攻击间隔
        attackDamage      // 单次伤害
    );
    setAttackComponent(attack);

    // =========================
    // 2. 表现（Sprite）
    // =========================

    // 使用 Animations 文件夹下的图片作为初始图
    _sprite = Sprite::create("Images/troops/Animations/ValkyrieMove1.png");
    
    // 基础缩放值 (统一标准)
    const float baseScale = 0.8f;

    if (_sprite)
    {
        CCLOG("ValkyrieTroop: Sprite loaded successfully.");
        addChild(_sprite);
        _sprite->setScale(baseScale);
    }
    else
    {
        CCLOG("ValkyrieTroop: Sprite load failed!");
    }

    // =========================
    // 3. 绑定动画组件
    // =========================
    auto anim = new AnimationComponent();
    setAnimationComponent(anim);
    
    // 设置默认贴图
    anim->setDefaultTexture("Images/troops/Animations/ValkyrieMove1.png");

    // 1. 待机动画 (IDLE): 呼吸
    // ----------------------------------------------------------------
    auto breath = Sequence::create(
        ScaleTo::create(1.0f, baseScale * 1.05f), 
        ScaleTo::create(1.0f, baseScale * 0.95f),
        nullptr
    );
    anim->addAction(State::IDLE, RepeatForever::create(breath));

    // 2. 移动动画 (FOLLOWING): 移动图循环
    // ----------------------------------------------------------------
    {
        Vector<SpriteFrame*> walkFrames;
        
        auto tex1 = Director::getInstance()->getTextureCache()->addImage("Images/troops/Animations/ValkyrieMove1.png");
        auto tex2 = Director::getInstance()->getTextureCache()->addImage("Images/troops/Animations/ValkyrieMove2.png");
        auto tex3 = Director::getInstance()->getTextureCache()->addImage("Images/troops/Animations/ValkyrieMove3.png");
        
        if (tex1 && tex2 && tex3)
        {
            auto frame1 = SpriteFrame::createWithTexture(tex1, Rect(0, 0, tex1->getContentSize().width, tex1->getContentSize().height));
            auto frame2 = SpriteFrame::createWithTexture(tex2, Rect(0, 0, tex2->getContentSize().width, tex2->getContentSize().height));
            auto frame3 = SpriteFrame::createWithTexture(tex3, Rect(0, 0, tex3->getContentSize().width, tex3->getContentSize().height));
            
            walkFrames.pushBack(frame1);
            walkFrames.pushBack(frame2);
            walkFrames.pushBack(frame3);
            
            // 创建动画: 每帧 0.2 秒
            auto walkAnim = Animation::createWithSpriteFrames(walkFrames, 0.2f);
            anim->addAnimation(State::FOLLOWING, walkAnim);
        }
        else
        {
             CCLOG("ValkyrieTroop: Failed to load move animation textures.");
        }
    }

    // 3. 攻击动画 (ATTACKING): 攻击循环
    // ----------------------------------------------------------------
    {
        Vector<SpriteFrame*> attackFrames;
        
        auto texAtt1 = Director::getInstance()->getTextureCache()->addImage("Images/troops/Animations/ValkyrieAttack1.png");
        auto texAtt2 = Director::getInstance()->getTextureCache()->addImage("Images/troops/Animations/ValkyrieAttack2.png");
        auto texAtt3 = Director::getInstance()->getTextureCache()->addImage("Images/troops/Animations/ValkyrieAttack3.png");
        auto texAtt4 = Director::getInstance()->getTextureCache()->addImage("Images/troops/Animations/ValkyrieAttack4.png");
        
        if (texAtt1 && texAtt2 && texAtt3 && texAtt4)
        {
            auto att1 = SpriteFrame::createWithTexture(texAtt1, Rect(0, 0, texAtt1->getContentSize().width, texAtt1->getContentSize().height));
            auto att2 = SpriteFrame::createWithTexture(texAtt2, Rect(0, 0, texAtt2->getContentSize().width, texAtt2->getContentSize().height));
            auto att3 = SpriteFrame::createWithTexture(texAtt3, Rect(0, 0, texAtt3->getContentSize().width, texAtt3->getContentSize().height));
            auto att4 = SpriteFrame::createWithTexture(texAtt4, Rect(0, 0, texAtt4->getContentSize().width, texAtt4->getContentSize().height));
        
            attackFrames.pushBack(att1);
            attackFrames.pushBack(att2);
            attackFrames.pushBack(att3);
            attackFrames.pushBack(att4);
            
            // 动态计算帧间隔：总攻击间隔 / 帧数
            // 攻击间隔 1.2s，4帧
            // float attackInterval = 1.2f; 
            float delayPerUnit = attackInterval / 4.0f;
            
            auto attackAnim = Animation::createWithSpriteFrames(attackFrames, delayPerUnit);
            anim->addAnimation(State::ATTACKING, attackAnim);
        }
        else
        {
            CCLOG("ValkyrieTroop: Failed to load attack animation textures.");
        }
    }

    return true;
}

void ValkyrieTroop::update(float dt)
{
    // 1. 获取移动前位置
    Vec2 oldPos = getPosition();

    // 2. 调用基类更新
    TroopBase::update(dt);

    // 3. 获取移动后位置并计算方向
    Vec2 newPos = getPosition();
    Vec2 moveDir = newPos - oldPos;

    // 4. 如果有移动，则更新朝向
    if (moveDir.lengthSquared() > 0.1f) // 阈值防抖
    {
        moveDir.normalize();
        
        // 计算角度
        float angleDeg = CC_RADIANS_TO_DEGREES(moveDir.getAngle());
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
