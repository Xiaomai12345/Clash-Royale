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
	float AttackRangeClam = 25.0f;//攻击范围
	float AttackClam = 1.0;//攻击间隔
	float AttackDamageClam = 100;//攻击伤害

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
        AttackRangeClam,   // 攻击范围
        AttackClam,   // 攻击间隔
        AttackDamageClam      // 伤害值
    );
    setAttackComponent(attack);

    // =========================
    // 2. 添加图片作为士兵表现
    // =========================

    // 创建士兵的Sprite（骑士的图片）
    // 使用 Animations 文件夹下的图片作为初始图
    _sprite = Sprite::create("Images/troops/Animations/KnightMove.png"); 
    
    // 基础缩放值 (参考 Giant 设置为 0.8f，假设资源规格一致)
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

    // =========================
    // 3. 绑定动画组件
    // =========================
    auto anim = new AnimationComponent();
    setAnimationComponent(anim);
    
    // 设置默认贴图
    anim->setDefaultTexture("Images/troops/Animations/KnightMove.png");

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
        
        // 加载移动帧 (路径改为 Animations 下)
        auto tex1 = Director::getInstance()->getTextureCache()->addImage("Images/troops/Animations/KnightMove.png");
        auto tex2 = Director::getInstance()->getTextureCache()->addImage("Images/troops/Animations/KnightMove2.png");
        
        if (tex1 && tex2)
        {
            auto frame1 = SpriteFrame::createWithTexture(tex1, Rect(0, 0, tex1->getContentSize().width, tex1->getContentSize().height));
            auto frame2 = SpriteFrame::createWithTexture(tex2, Rect(0, 0, tex2->getContentSize().width, tex2->getContentSize().height));
            
            walkFrames.pushBack(frame1);
            walkFrames.pushBack(frame2);
            
            // 创建动画: 每帧 0.2 秒
            auto walkAnim = Animation::createWithSpriteFrames(walkFrames, 0.5f);
            anim->addAnimation(State::FOLLOWING, walkAnim);
        }
        else
        {
             CCLOG("KnightTroop: Failed to load move animation textures.");
        }
    }

    // 3. 攻击动画 (ATTACKING): 攻击1、2循环
    // ----------------------------------------------------------------
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
            
            // 动态计算帧间隔：总攻击间隔 / 帧数
            // 骑士攻击间隔 1.0s，2帧 -> 每帧 0.5s
            float attackInterval = 1.0f; // 与 MeleeAttackComponent 中的值保持一致
            float delayPerUnit = attackInterval / 2.0f;
            
            auto attackAnim = Animation::createWithSpriteFrames(attackFrames, delayPerUnit);
            anim->addAnimation(State::ATTACKING, attackAnim);
        }
        else
        {
            CCLOG("KnightTroop: Failed to load attack animation textures.");
        }
    }

    // 血条初始化（确保只初始化一次）
    return true;
}

void KnightTroop::update(float dt)
{
    // 1. 获取移动前位置
    Vec2 oldPos = getPosition();

    // 2. 调用基类更新（处理移动、AI、攻击等）
    TroopBase::update(dt);

    // 3. 获取移动后位置并计算方向
    Vec2 newPos = getPosition();
    Vec2 moveDir = newPos - oldPos;

    // 4. 如果有移动，则更新朝向
    if (moveDir.lengthSquared() > 0.1f) // 阈值防抖
    {
        moveDir.normalize();
        
        // 计算角度：
        // Vec2::getAngle() 返回的是数学角度（弧度），X轴正向为0，逆时针为正。
        // Sprite::setRotation() 接受的是角度（度数），顺时针为正。
        // 我们的素材是朝向Y轴正向（上）的。
        
        // 数学角度转度数
        float angleDeg = CC_RADIANS_TO_DEGREES(moveDir.getAngle());
        
        // 转换公式：TargetRotation = 90 - MathAngle
        float rotation = 90.0f - angleDeg;
        
        if (_sprite)
        {
            _sprite->setRotation(rotation);
        }
    }
}
