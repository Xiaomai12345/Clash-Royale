#include "Giant.h"
#include "SimpleTroopAIComponent.h"
#include "GroundMoveComponent.h"
#include "MeleeAttackComponent.h"
#include "AnimationComponent.h"

Giant::Giant()
{
    // 在构造函数中给基类和成员变量赋值
    _moveSpeed = 100.0f;   // 设定巨人的基础移动速度（相对较慢）
    _maxHp = 500;         // 设定巨人的最大血量
    _alertRange = 250.f;   // 设置警戒范围
    _bodyRadius = 30.f;    // 设置碰撞半径（比骑士大）
    _camp = ECamp::LEFT;   // 设置阵营为左侧

    _attacktype = AttackType::Building;  // 巨人可以攻击地面和空中的目标
    _moveAttack = MoveAttack::Ground;  // 设置巨人可以攻击地面目标
    _moveAttacked = MoveAttack::Both; // 设置巨人可被地面和空中攻击
    _moveType = MoveType::Ground;    // 设置为地面移动

    _isDying = false;  // 初始化死亡状态
}

Giant::~Giant()
{
    // 如果有创建的组件，手动清理（这里使用了 new 创建组件，所以要 delete）
    if (_ai) delete _ai;
    if (_move) delete _move;
    if (_attack) delete _attack;
}

bool Giant::init()
{

    float AttackRangeClam = 50.0f;//攻击范围
    float AttackClam = 1.5;//攻击间隔
    float AttackDamageClam = 200;//攻击伤害

    if (!TroopBase::init())  // 初始化基类 (TroopBase)
        return false;

    // =========================
    // 1. 创建并绑定组件
    // =========================

    // 创建并绑定AI组件（简单的AI逻辑）
    auto ai = new SimpleTroopAIComponent();
    setAIComponent(ai);

    // 创建并绑定地面移动组件（用于控制巨人的移动）
    auto move = new GroundMoveComponent();
    setMoveComponent(move);

    // 创建并绑定近战攻击组件（巨人的近战攻击）
    auto attack = new MeleeAttackComponent(
        AttackRangeClam,   
        AttackClam,    
        AttackDamageClam      
    );
    setAttackComponent(attack);

    // =========================
    // 2. 添加图片作为士兵表现
    // =========================

    // 创建士兵的Sprite（巨人的图片）
    _sprite = Sprite::create("Images/troops/Animations/GiantMove2.png");  // 使用巨人的图片（需替换为正确路径）
    
    // 基础缩放值 (保留原有逻辑，但提取为变量)
    const float baseScale = 0.8f; 

    if (_sprite)
    {
        CCLOG("Giant: Sprite loaded successfully.");
        addChild(_sprite);  // 将图片添加到当前节点
        _sprite->setScale(baseScale);  // 根据需要调整缩放
    }
    else
    {
        CCLOG("Giant: Sprite load failed!");  // 加载图片失败时输出日志
    }

    // =========================
    // 3. 绑定动画组件
    // =========================
    auto anim = new AnimationComponent();
    setAnimationComponent(anim);
    
    // 设置默认贴图
    anim->setDefaultTexture("Images/troops/Animations/GiantMove2.png");

    // 1. 待机动画 (IDLE): 呼吸
    // ----------------------------------------------------------------
    auto breath = Sequence::create(
        ScaleTo::create(1.5f, baseScale * 1.05f), 
        ScaleTo::create(1.5f, baseScale * 0.95f),
        nullptr
    );
    // 待机时只做呼吸，不旋转
    anim->addAction(State::IDLE, RepeatForever::create(breath));

    // 2. 移动动画 (FOLLOWING): 巨人移动图循环
    // ----------------------------------------------------------------
    {
        Vector<SpriteFrame*> walkFrames;
        
        // 尝试加载移动帧
        auto tex1 = Director::getInstance()->getTextureCache()->addImage("Images/troops/Animations/GiantMove1.png");
        auto tex2 = Director::getInstance()->getTextureCache()->addImage("Images/troops/Animations/GiantMove2.png");
        auto tex3 = Director::getInstance()->getTextureCache()->addImage("Images/troops/Animations/GiantMove3.png");
        
        if (tex1 && tex2 && tex3)
        {
            auto frame1 = SpriteFrame::createWithTexture(tex1, Rect(0, 0, tex1->getContentSize().width, tex1->getContentSize().height));
            auto frame2 = SpriteFrame::createWithTexture(tex2, Rect(0, 0, tex2->getContentSize().width, tex2->getContentSize().height));
            auto frame3 = SpriteFrame::createWithTexture(tex3, Rect(0, 0, tex3->getContentSize().width, tex3->getContentSize().height));
            
            walkFrames.pushBack(frame2);
            walkFrames.pushBack(frame1);
            walkFrames.pushBack(frame2);
            walkFrames.pushBack(frame3);
            
            // 创建动画: 巨人移动较慢，每帧 0.5 秒
            auto walkAnim = Animation::createWithSpriteFrames(walkFrames, 0.5f);
            anim->addAnimation(State::FOLLOWING, walkAnim);
        }
        else
        {
             CCLOG("Giant: Failed to load move animation textures.");
        }
    }

    // 3. 攻击动画 (ATTACKING): 攻击1、2、3循环
    // ----------------------------------------------------------------
    {
        Vector<SpriteFrame*> attackFrames;
        
        auto texAtt1 = Director::getInstance()->getTextureCache()->addImage("Images/troops/Animations/GiantAttack1.png");
        auto texAtt2 = Director::getInstance()->getTextureCache()->addImage("Images/troops/Animations/GiantAttack2.png");
        auto texAtt3 = Director::getInstance()->getTextureCache()->addImage("Images/troops/Animations/GiantAttack3.png");
        
        if (texAtt1 && texAtt2 && texAtt3)
        {
            auto att1 = SpriteFrame::createWithTexture(texAtt1, Rect(0, 0, texAtt1->getContentSize().width, texAtt1->getContentSize().height));
            auto att2 = SpriteFrame::createWithTexture(texAtt2, Rect(0, 0, texAtt2->getContentSize().width, texAtt2->getContentSize().height));
            auto att3 = SpriteFrame::createWithTexture(texAtt3, Rect(0, 0, texAtt3->getContentSize().width, texAtt3->getContentSize().height));
        
            attackFrames.pushBack(att1);
            attackFrames.pushBack(att2);
            attackFrames.pushBack(att3);
            
            // 动态计算帧间隔：总攻击间隔 / 帧数
            // 例如：攻击间隔 1.5s，3帧 -> 每帧 0.5s
            // 这样一次完整动画正好对应一次攻击冷却
            float attackInterval = 1.5f; // 与 MeleeAttackComponent 中的值保持一致
            float delayPerUnit = attackInterval / 3.0f;
            
            // 攻击动画
            auto attackAnim = Animation::createWithSpriteFrames(attackFrames, delayPerUnit);
            anim->addAnimation(State::ATTACKING, attackAnim);
        }
        else
        {
            CCLOG("Giant: Failed to load attack animation textures.");
        }
    }

    // 血条初始化（确保只初始化一次）
    return true;
}

void Giant::update(float dt)
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
        // 例如：
        // 上 (0,1): Math=90, Rot=90-90=0 (不转)
        // 右 (1,0): Math=0, Rot=90-0=90 (顺时针转90度)
        // 下 (0,-1): Math=-90, Rot=90-(-90)=180 (转180度)
        // 左 (-1,0): Math=180, Rot=90-180=-90 (逆时针转90度)
        
        float rotation = 90.0f - angleDeg;
        
        if (_sprite)
        {
            _sprite->setRotation(rotation);
        }
    }
}
