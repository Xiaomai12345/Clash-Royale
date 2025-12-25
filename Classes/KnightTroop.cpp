#include "KnightTroop.h"
#include "SimpleTroopAIComponent.h"
#include "GroundMoveComponent.h"
#include "MeleeAttackComponent.h"
#include "AnimationComponent.h"

KnightTroop::KnightTroop()
{
    // 在构造函数中给基类和成员变量赋值
    _moveSpeed = 100.0f;  // 设定骑士的基础移动速度
    _maxHp = 100;         // 设定骑士的最大血量
    _alertRange = 200.f;   // 设置警戒范围
    _bodyRadius = 15.f;    // 设置碰撞半径
    _camp = ECamp::LEFT;   // 设置阵营为左侧

    _attacktype = AttackType::Both; // 
    _moveAttack = MoveAttack::Ground;  // 设置可攻击地面和空中的目标
    _moveAttacked = MoveAttack::Both; // 设置可被攻击地面和空中的目标
    _moveType = MoveType::Ground;    // 设置为地面移动

    _isDying = false;  // 初始化死亡状态
}

KnightTroop::~KnightTroop()
{
    // 如果有创建的组件，手动清理（这里使用了 new 创建组件，所以要 delete）
    if (_ai) delete _ai;
    if (_move) delete _move;
    if (_attack) delete _attack;
}

bool KnightTroop::init()
{
    if (!TroopBase::init())  // 初始化基类 (TroopBase)
        return false;

    // =========================
    // 1. 创建并绑定组件
    // =========================

    // 创建并绑定AI组件（简单的AI逻辑）
    auto ai = new SimpleTroopAIComponent();
    setAIComponent(ai);

    // 创建并绑定地面移动组件（用于控制骑士的移动）
    auto move = new GroundMoveComponent();
    setMoveComponent(move);

    // 创建并绑定近战攻击组件（骑士的近战攻击）
    auto attack = new MeleeAttackComponent(
        25.0f,   // 攻击范围
        1.0f,     // 攻击间隔
        100      // 伤害值
    );
    setAttackComponent(attack);

    // =========================
    // 2. 添加图片作为士兵表现
    // =========================

    // 创建士兵的Sprite（骑士的图片）
    _sprite = Sprite::create("Images/troops/Knight.png"); // 使用 PNG 系列
    if (_sprite)
    {
		CCLOG("KnightTroop: Sprite loaded successfully.");
        addChild(_sprite);  // 将图片添加到当前节点
        _sprite->setScale(1.f);  // 保持缩放
    }
    else
    {
        CCLOG("KnightTroop: Sprite load failed!");
    }

    // =========================
    // 3. 绑定动画组件
    // =========================
    auto anim = new AnimationComponent();
    setAnimationComponent(anim);
    
    // 设置默认贴图
    anim->setDefaultTexture("Images/troops/Knight.png");

    // 基础缩放值
    const float baseScale = 1.f; 

    // 1. 待机动画 (IDLE): 呼吸
    // ----------------------------------------------------------------
    auto breath = Sequence::create(
        ScaleTo::create(1.0f, baseScale * 1.05f), 
        ScaleTo::create(1.0f, baseScale * 0.95f),
        nullptr
    );
    // 待机时只做呼吸，不旋转
    anim->addAction(State::IDLE, RepeatForever::create(breath));


    // 2. 移动动画 (FOLLOWING): 骑士移动图循环
    // ----------------------------------------------------------------
    {
        Vector<SpriteFrame*> walkFrames;
        
        auto tex1 = Director::getInstance()->getTextureCache()->addImage("Images/troops/Knight.png");
        auto tex2 = Director::getInstance()->getTextureCache()->addImage("Images/troops/KnightMove.png");
        
        if (tex1 && tex2)
        {
            auto frame1 = SpriteFrame::createWithTexture(tex1, Rect(0, 0, tex1->getContentSize().width, tex1->getContentSize().height));
            auto frame2 = SpriteFrame::createWithTexture(tex2, Rect(0, 0, tex2->getContentSize().width, tex2->getContentSize().height));
            
            walkFrames.pushBack(frame1);
            walkFrames.pushBack(frame2);
            
            // 创建动画: 每帧 0.2 秒
            auto walkAnim = Animation::createWithSpriteFrames(walkFrames, 0.2f);
            anim->addAnimation(State::FOLLOWING, walkAnim);
        }
    }

    // 3. 攻击动画 (ATTACKING): 攻击1、2循环
    // ----------------------------------------------------------------
    {
        Vector<SpriteFrame*> attackFrames;
        
        auto texAtt1 = Director::getInstance()->getTextureCache()->addImage("Images/troops/KnightAttack1.png");
        auto texAtt2 = Director::getInstance()->getTextureCache()->addImage("Images/troops/KnightAttack2.png");
        
        if (texAtt1 && texAtt2)
        {
            auto att1 = SpriteFrame::createWithTexture(texAtt1, Rect(0, 0, texAtt1->getContentSize().width, texAtt1->getContentSize().height));
            auto att2 = SpriteFrame::createWithTexture(texAtt2, Rect(0, 0, texAtt2->getContentSize().width, texAtt2->getContentSize().height));
        
            attackFrames.pushBack(att1);
            attackFrames.pushBack(att2);
            
            // 攻击速度稍微快一点，0.15秒一帧
            auto attackAnim = Animation::createWithSpriteFrames(attackFrames, 0.15f);
            anim->addAnimation(State::ATTACKING, attackAnim);
        }
    }

    // 血条初始化（确保只初始化一次）
    return true;
}
