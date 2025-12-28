#include "Minions.h"
#include "DataManager.h"
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

    // =========================
    // 0. DataManager 加载数据
    // =========================
    float attackRange = 20.0f;
    float attackInterval = 1.5f;
    float attackDamage = 50.0f;

    auto data = DataManager::getInstance()->getCardDataByName("minions");
    if (!data.empty())
    {
        if (data.count("health")) {
            _maxHp = data["health"].asInt();
            _hp = _maxHp;
        }
        if (data.count("moveSpeed")) {
            _moveSpeed = data["moveSpeed"].asFloat() * 40.5f;
            resetMoveSpeed();
        }
        if (data.count("viewRange")) {
            _alertRange = data["viewRange"].asFloat() * 40.5f;
        }
        if (data.count("attackRange")) {
            // JSON 1.6 -> 32 (if * 20).
            attackRange = data["attackRange"].asFloat() * 20.25f;
        }
        if (data.count("attackSpeed")) {
            attackInterval = data["attackSpeed"].asFloat();
        }
        if (data.count("attackPower")) {
            attackDamage = data["attackPower"].asFloat();
        }
        CCLOG("Minions Data Loaded: HP=%d, DMG=%.1f", _maxHp, attackDamage);
    }

    // AI
    auto ai = new SimpleTroopAIComponent();
    setAIComponent(ai);

    // 飞行移动
    auto move = new AirMoveComponent();
    setMoveComponent(move);

    // 近战攻击
    auto attack = new MeleeAttackComponent(
        attackRange,   // 攻击范围
        attackInterval,    // 攻击间隔
        attackDamage       // 伤害
    );
    setAttackComponent(attack);

    // =========================
    // 2. Sprite
    // =========================

    // 使用 Animations 文件夹下的图片作为初始图
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
        
        // 注意：这里需要确保文件名正确，假设之前的文件名是正确的
        auto tex1 = Director::getInstance()->getTextureCache()->addImage("Images/troops/Animations/MinionMove(1).png");
        auto tex2 = Director::getInstance()->getTextureCache()->addImage("Images/troops/Animations/MinionMove(2).png");
        
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
        else
        {
             CCLOG("Minions: Failed to load move animation textures.");
        }
    }

    // 3. 攻击动画 (ATTACKING): 攻击循环
    // ----------------------------------------------------------------
    {
        Vector<SpriteFrame*> attackFrames;
        
        auto texAtt1 = Director::getInstance()->getTextureCache()->addImage("Images/troops/Animations/MinionAttack(1).png");
        auto texAtt2 = Director::getInstance()->getTextureCache()->addImage("Images/troops/Animations/MinionAttack(2).png");
        auto texAtt3 = Director::getInstance()->getTextureCache()->addImage("Images/troops/Animations/MinionAttack(3).png");
        auto texAtt4 = Director::getInstance()->getTextureCache()->addImage("Images/troops/Animations/MinionAttack(4).png");
        
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
            float delayPerUnit = attackInterval / 4.0f;
            
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
