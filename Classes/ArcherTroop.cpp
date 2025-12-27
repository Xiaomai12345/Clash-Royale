#include "ArcherTroop.h"

// 组件
#include "SimpleTroopAIComponent.h"
#include "GroundMoveComponent.h"
#include "RangedAttackComponent.h"
#include "AnimationComponent.h"

#include "cocos2d.h"

USING_NS_CC;

ArcherTroop::ArcherTroop()
{

    _moveSpeed = 90.0f;
    _maxHp = 70;
    _alertRange = 300.f;
    _bodyRadius = 12.f;
    _camp = ECamp::LEFT;

    _attacktype = AttackType::Both;
    _moveAttack = MoveAttack::Both;
    _moveAttacked = MoveAttack::Both;
    _moveType = MoveType::Ground;

    _isDying = false;
}

ArcherTroop::~ArcherTroop()
{
    if (_ai)     delete _ai;
    if (_move)   delete _move;
    if (_attack) delete _attack;
}

bool ArcherTroop::init()
{
    if (!TroopBase::init())
        return false;

    // =========================
    // 1. 创建并绑定组件
    // =========================

    float AttackRangeClam = 100.0f;//攻击范围
    float AttackClam = 1.5;//攻击间隔
    float AttackDamageClam = 100;//攻击伤害
	float AttackSpeedClam = 500.0f;//弹道速度
    // AI
    auto ai = new SimpleTroopAIComponent();
    setAIComponent(ai);

    // 地面移动
    auto move = new GroundMoveComponent();
    setMoveComponent(move);

    // 远程攻击
    auto attack = new RangedAttackComponent(
        AttackRangeClam,  // 攻击范围
        AttackClam,    // 攻击间隔
        AttackDamageClam,      // 伤害
        AttackSpeedClam   // 弹道速度
    );
    setAttackComponent(attack);

    // =========================
    // 2. 添加图片
    // =========================

    // 使用 Animations 下的图片作为初始图
    _sprite = Sprite::create("Images/troops/Animations/ArcherMove1.png");
    
    // 基础缩放值 (参考 Giant/Knight，假设资源规格一致)
    const float baseScale = 0.8f;

    if (_sprite)
    {
        addChild(_sprite);
        _sprite->setScale(baseScale);
    }
    else
    {
        CCLOG("ArcherTroop: sprite load failed");
    }

    // =========================
    // 3. 绑定动画组件
    // =========================
    auto anim = new AnimationComponent();
    setAnimationComponent(anim);

    // 设置默认贴图
    anim->setDefaultTexture("Images/troops/Animations/ArcherMove1.png");

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

        auto tex1 = Director::getInstance()->getTextureCache()->addImage("Images/troops/Animations/ArcherMove1.png");

        if (tex1 && tex2)
        {
            auto frame1 = SpriteFrame::createWithTexture(tex1, Rect(0, 0, tex1->getContentSize().width, tex1->getContentSize().height));

            walkFrames.pushBack(frame1);
 

            // 创建动画: 每帧 0.2 秒
            auto walkAnim = Animation::createWithSpriteFrames(walkFrames, 0.2f);
            anim->addAnimation(State::FOLLOWING, walkAnim);
        }
        else
        {
            CCLOG("ArcherTroop: Failed to load move animation textures.");
        }
    }

    // 3. 攻击动画 (ATTACKING): 攻击循环
    // ----------------------------------------------------------------
    {
        Vector<SpriteFrame*> attackFrames;

        auto texAtt2 = Director::getInstance()->getTextureCache()->addImage("Images/troops/Animations/ArcherAttack2.png");
        auto texAtt4 = Director::getInstance()->getTextureCache()->addImage("Images/troops/Animations/ArcherAttack4.png");

        if (texAtt2  && texAtt4)
        {
            auto att2 = SpriteFrame::createWithTexture(texAtt2, Rect(0, 0, texAtt2->getContentSize().width, texAtt2->getContentSize().height));
            auto att4 = SpriteFrame::createWithTexture(texAtt4, Rect(0, 0, texAtt4->getContentSize().width, texAtt4->getContentSize().height));

            attackFrames.pushBack(att2);
            attackFrames.pushBack(att4);


            float attackInterval = AttackClam;
            float delayPerUnit = attackInterval / 2.0f;

            auto attackAnim = Animation::createWithSpriteFrames(attackFrames, delayPerUnit);
            anim->addAnimation(State::ATTACKING, attackAnim);
        }
        else
        {
            CCLOG("ArcherTroop: Failed to load attack animation textures.");
        }
    }

    return true;
}

void ArcherTroop::update(float dt)
{
    // 1. 获取移动前位置
    Vec2 oldPos = getPosition();

    // 2. 调用基类更新
    TroopBase::update(dt);

    // 3. 获取移动后位置并计算方向
    Vec2 newPos = getPosition();
    Vec2 moveDir = newPos - oldPos;

    // 4. 如果有移动，则更新朝向
    if (moveDir.lengthSquared() > 0.1f)
    {
        moveDir.normalize();

        // 计算角度
        float angleDeg = CC_RADIANS_TO_DEGREES(moveDir.getAngle());

        // 转换公式：TargetRotation = 90 - MathAngle
        float rotation = 90.0f - angleDeg;

        if (_sprite)
        {
            _sprite->setRotation(rotation);
        }
    }
}
