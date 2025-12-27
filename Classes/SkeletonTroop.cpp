#include "SkeletonTroop.h"
#include "SimpleTroopAIComponent.h"
#include "GroundMoveComponent.h"
#include "MeleeAttackComponent.h"
#include "AnimationComponent.h"

SkeletonTroop::SkeletonTroop()
{
    // 在构造函数中给基类和成员变量赋值
    _moveSpeed = 90.0f;  // 设定骷髅的基础移动速度（较慢）
    _maxHp = 80;         // 设定骷髅的最大血量
    _alertRange = 150.f;  // 设置警戒范围
    _bodyRadius = 5.f;   // 设置碰撞半径
    _camp = ECamp::LEFT;  // 设置阵营为左侧

    _attacktype = AttackType::Both;  // 骷髅可以攻击地面和空中目标
    _moveAttack = MoveAttack::Ground; // 设置为地面攻击
    _moveAttacked = MoveAttack::Both; // 设置可被攻击地面和空中的目标
    _moveType = MoveType::Ground;    // 设置为地面移动

    _isDying = false;  // 初始化死亡状态
}

SkeletonTroop::~SkeletonTroop()
{
    // 如果有创建的组件，手动清理（这里使用了 new 创建组件，所以要 delete）
    if (_ai) delete _ai;
    if (_move) delete _move;
    if (_attack) delete _attack;
}

bool SkeletonTroop::init()
{
    if (!TroopBase::init())  // 初始化基类 (TroopBase)
        return false;

    setupComponents();  // 初始化组件

    CCLOG("SkeletonTroop 初始化完成，位置：(%.0f, %.0f)", getPositionX(), getPositionY());

    return true;
}

void SkeletonTroop::setupComponents()
{
    float AttackRangeClam = 20.0f;//攻击范围
    float AttackClam = 1.0;//攻击间隔
    float AttackDamageClam = 10;//攻击伤害



    // 创建并绑定AI组件（简单的AI逻辑）
    auto ai = new SimpleTroopAIComponent();
    setAIComponent(ai);

    // 创建并绑定地面移动组件（用于控制骷髅的移动）
    auto move = new GroundMoveComponent();
    setMoveComponent(move);

    // 创建并绑定近战攻击组件（骷髅的近战攻击）
    auto attack = new MeleeAttackComponent(
        AttackRangeClam,   // 攻击范围
        AttackClam,     // 攻击间隔
        AttackDamageClam        // 伤害值
    );
    setAttackComponent(attack);

    // =========================
    // 2. 添加图片作为士兵表现
    // =========================
    
    // 使用 Animations 文件夹下的图片作为初始图
    _sprite = Sprite::create("Images/troops/Animations/SkeletonMove1.png");
    
    // 基础缩放值 (参考 Giant/Knight，假设资源规格一致)
    const float baseScale = 0.8f; 
    
    if (_sprite)
    {
        addChild(_sprite);
        _sprite->setScale(baseScale);  // 根据需要调整缩放
    }
    else
    {
        CCLOG("SkeletonTroop: Sprite load failed!");  // 加载图片失败时输出日志
    }

    // =========================
    // 3. 绑定动画组件
    // =========================
    auto anim = new AnimationComponent();
    setAnimationComponent(anim);
    
    // 设置默认贴图
    anim->setDefaultTexture("Images/troops/Animations/SkeletonMove1.png");

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
        
        auto tex1 = Director::getInstance()->getTextureCache()->addImage("Images/troops/Animations/SkeletonMove1.png");
        auto tex2 = Director::getInstance()->getTextureCache()->addImage("Images/troops/Animations/SkeletonMove2.png");
        auto tex3 = Director::getInstance()->getTextureCache()->addImage("Images/troops/Animations/SkeletonMove3.png");
        
        if (tex1 && tex2 && tex3)
        {
            auto frame1 = SpriteFrame::createWithTexture(tex1, Rect(0, 0, tex1->getContentSize().width, tex1->getContentSize().height));
            auto frame2 = SpriteFrame::createWithTexture(tex2, Rect(0, 0, tex2->getContentSize().width, tex2->getContentSize().height));
            auto frame3 = SpriteFrame::createWithTexture(tex3, Rect(0, 0, tex3->getContentSize().width, tex3->getContentSize().height));
            
            walkFrames.pushBack(frame1);
            walkFrames.pushBack(frame2);
            walkFrames.pushBack(frame3);
            
            // 创建动画: 每帧 0.2 秒
            auto walkAnim = Animation::createWithSpriteFrames(walkFrames, 0.5f);
            anim->addAnimation(State::FOLLOWING, walkAnim);
        }
        else
        {
             CCLOG("SkeletonTroop: Failed to load move animation textures.");
        }
    }

    // 3. 攻击动画 (ATTACKING): 攻击循环
    // ----------------------------------------------------------------
    {
        Vector<SpriteFrame*> attackFrames;
        
        auto texAtt1 = Director::getInstance()->getTextureCache()->addImage("Images/troops/Animations/SkeletonAttack1.png");
        auto texAtt2 = Director::getInstance()->getTextureCache()->addImage("Images/troops/Animations/SkeletonAttack2.png");
        auto texAtt3 = Director::getInstance()->getTextureCache()->addImage("Images/troops/Animations/SkeletonAttack3.png");
        
        if (texAtt1 && texAtt2 && texAtt3)
        {
            auto att1 = SpriteFrame::createWithTexture(texAtt1, Rect(0, 0, texAtt1->getContentSize().width, texAtt1->getContentSize().height));
            auto att2 = SpriteFrame::createWithTexture(texAtt2, Rect(0, 0, texAtt2->getContentSize().width, texAtt2->getContentSize().height));
            auto att3 = SpriteFrame::createWithTexture(texAtt3, Rect(0, 0, texAtt3->getContentSize().width, texAtt3->getContentSize().height));
        
            attackFrames.pushBack(att1);
            attackFrames.pushBack(att2);
            attackFrames.pushBack(att3);
            
            // 动态计算帧间隔：总攻击间隔 / 帧数
            // 攻击间隔 1.0s，3帧
            float attackInterval = AttackClam;
            float delayPerUnit = attackInterval / 3.0f;
            
            auto attackAnim = Animation::createWithSpriteFrames(attackFrames, delayPerUnit);
            anim->addAnimation(State::ATTACKING, attackAnim);
        }
        else
        {
            CCLOG("SkeletonTroop: Failed to load attack animation textures.");
        }
    }
}

void SkeletonTroop::update(float dt)
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
        
        // 转换公式：TargetRotation = 90 - MathAngle
        float rotation = 90.0f - angleDeg;
        
        if (_sprite)
        {
            _sprite->setRotation(rotation);
        }
    }
}
