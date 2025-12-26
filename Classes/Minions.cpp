#include "Minions.h"
#include "AirMoveComponent.h"
#include "MeleeAttackComponent.h"
#include "SimpleTroopAIComponent.h"
#include "AnimationComponent.h"

USING_NS_CC;

Minions::Minions()
{
    // =========================
    // 基础属性初始化
    // =========================
    _moveSpeed = 180.0f;     // 亡灵移动速度（比飞龙快）
    _maxHp = 90;             // 亡灵血量较低
    _alertRange = 220.f;     // 警戒范围
    _bodyRadius = 12.f;      // 碰撞半径偏小
    _camp = ECamp::LEFT;     // 左侧阵营

    _attacktype = AttackType::Both; // 可攻击建筑和单位
    _moveAttack = MoveAttack::Both; // 可攻击空中和地面
    _moveAttacked = MoveAttack::Air; // 只能被对空攻击
    _moveType = MoveType::Air;      // 飞行单位

    _isDying = false;
}

Minions::~Minions()
{
    // 组件是 new 出来的，必须手动 delete
    if (_ai)     delete _ai;
    if (_move)   delete _move;
    if (_attack) delete _attack;
}

bool Minions::init()
{
    if (!TroopBase::init())
        return false;

    float AttackRangeClam = 20.0;//攻击范围
    float AttackClam = 1.5;//攻击间隔
    float AttackDamageClam = 50;//攻击伤害

    // AI
    auto ai = new SimpleTroopAIComponent();
    setAIComponent(ai);

    // 飞行移动
    auto move = new AirMoveComponent();
    setMoveComponent(move);

    // 近战攻击
    auto attack = new MeleeAttackComponent(
        AttackRangeClam,   // 攻击范围（很近）
        AttackClam,    // 攻击间隔（快攻）
        AttackDamageClam       // 伤害
    );
    setAttackComponent(attack);

    // =========================
    // 2. Sprite
    // =========================

    // 使用 Animations 文件夹下的图片作为初始图
    // 注意：文件名格式为 MinionMove(1).png，与 ls 结果 MinionMove1.png 不完全一致，
    // 需要根据实际文件名调整。根据 ls 结果：
    // MinionMove(1).png, MinionMove(2).png
    // MinionAttack(1).png ~ (4).png
    
    // 检查目录 ls 结果，发现文件名是:
    // MinionMove(1).png
    // MinionMove(2).png
    // MinionAttack(1).png ... (4).png
    
    _sprite = Sprite::create("Images/troops/Animations/MinionMove(1).png");
    
    // 基础缩放值 (统一标准)
    const float baseScale = 0.8f;
    
    if (_sprite)
    {
        CCLOG("Minions: Sprite loaded successfully.");
        addChild(_sprite);
        _sprite->setScale(baseScale);
    }
    else
    {
        CCLOG("Minions: Sprite load failed!");
    }

    // =========================
    // 3. 绑定动画组件
    // =========================
    auto anim = new AnimationComponent();
    setAnimationComponent(anim);
    
    // 设置默认贴图
    anim->setDefaultTexture("Images/troops/Animations/MinionMove(1).png");

    // 1. 待机动画 (IDLE): 呼吸
    // ----------------------------------------------------------------
    auto breath = Sequence::create(
        ScaleTo::create(1.0f, baseScale * 1.05f), 
        ScaleTo::create(1.0f, baseScale * 0.95f),
        nullptr
    );
    anim->addAction(State::IDLE, RepeatForever::create(breath));

    // 2. 移动动画 (FOLLOWING): 亡灵移动图循环
    // ----------------------------------------------------------------
    {
        Vector<SpriteFrame*> walkFrames;
        
        auto tex1 = Director::getInstance()->getTextureCache()->addImage("Images/troops/Animations/MinionMove1.png");
        
        if (tex1 && tex2)
        {
            auto frame1 = SpriteFrame::createWithTexture(tex1, Rect(0, 0, tex1->getContentSize().width, tex1->getContentSize().height));
            
            walkFrames.pushBack(frame1);
            
            // 创建动画: 每帧 0.2 秒
            auto walkAnim = Animation::createWithSpriteFrames(walkFrames, 0.5f);
            anim->addAnimation(State::FOLLOWING, walkAnim);
        }
        else
        {
             CCLOG("Minions: Failed to load move animation textures.");
        }
    }

    // 3. 攻击动画 (ATTACKING): 攻击循环
    // ----------------------------------------------------------------
    {
        Vector<SpriteFrame*> attackFrames;
        
        auto texAtt1 = Director::getInstance()->getTextureCache()->addImage("Images/troops/Animations/MinionAttack1.png");
        auto texAtt2 = Director::getInstance()->getTextureCache()->addImage("Images/troops/Animations/MinionAttack2.png");
        
        if (texAtt1 && texAtt2 )
        {
            auto att1 = SpriteFrame::createWithTexture(texAtt1, Rect(0, 0, texAtt1->getContentSize().width, texAtt1->getContentSize().height));
            auto att2 = SpriteFrame::createWithTexture(texAtt2, Rect(0, 0, texAtt2->getContentSize().width, texAtt2->getContentSize().height));
        
            attackFrames.pushBack(att1);
            attackFrames.pushBack(att2);
            
            // 动态计算帧间隔：总攻击间隔 / 帧数
      
            float attackInterval = AttackClam;
            float delayPerUnit = attackInterval / 2.0f;
            
            auto attackAnim = Animation::createWithSpriteFrames(attackFrames, delayPerUnit);
            anim->addAnimation(State::ATTACKING, attackAnim);
        }
        else
        {
            CCLOG("Minions: Failed to load attack animation textures.");
        }
    }

    return true;
}

void Minions::update(float dt)
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
