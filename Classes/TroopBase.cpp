#include "TroopBase.h"
#include "TroopAIComponent.h"
#include "MoveComponent.h"
#include "AttackComponent.h"
#include "AnimationComponent.h"

USING_NS_CC;

TroopBase::TroopBase()
    :  _maxHp(100)
    , _moveSpeed(300.0f)
    , _alertRange(100.f)
    , _bodyRadius(15.0f)
    , _camp(ECamp::LEFT)
    , _attacktype(AttackType::Both)//默认攻击类型
    , _moveAttack(MoveAttack::Both)//默认空地均可攻击
    , _moveAttacked(MoveAttack::Both)//默认均可被攻击
    , _moveType(MoveType::Ground)
    , _ai(nullptr)
    , _move(nullptr)
    , _attack(nullptr)
    , _anim(nullptr)
    , _sprite(nullptr)
    , _debugDraw(nullptr)
    , _showDebugBounds(0)
    , _hpBarNode(nullptr)
    , _hpBarBg(nullptr)
    , _hpBarFg(nullptr)
    , _hpBarInited(false)
    , _isDying(false)
{
}

TroopBase::~TroopBase()
{
    // 如果组件是自己 new 出来的，原则上应该 delete，
    // 但如果遵循“谁创建谁销毁”或者依赖 cocos2d 的 autorelease，需要注意。
    // 之前代码里没有 delete，这里保持一致，或者在 destroy 中处理。
    // 如果 _anim 是 new 出来的普通 C++ 类，需要手动 delete。
    CC_SAFE_DELETE(_anim);
}


bool TroopBase::init()
{
    if (!Node::init())
        return false;

    _hp = _maxHp;

    // =========================
    // Debug 绘制节点（⚠️ 必须创建）
    // =========================
    if (_showDebugBounds)
    {
        _debugDraw = DrawNode::create();
        addChild(_debugDraw, 100);
    }

    // =========================
    // 血条初始化（一次）
    // =========================
    initHpBar();
    _hpBarInited = true;
    _currentSpeed = _moveSpeed;
    scheduleUpdate();
    return true;
}

void TroopBase::update(float dt)
{

    if (_ai)
    {
        _ai->update(this, dt);
    }

    if (_move)
    {
        _move->update(this, dt);
    }

    if (_attack)
    {
        _attack->update(this, dt);
    }

    if (_anim)
    {
        _anim->update(this, dt);
    }
    else
    {
        // 尝试自动查找并绑定动画组件 (如果子类没有绑定)
        if (auto animComp = dynamic_cast<AnimationComponent*>(getAnimationComponent()))
        {
             _anim = animComp;
             _anim->update(this, dt);
        }
    }


    if (_showDebugBounds && _debugDraw)
    {
        _debugDraw->clear();

        // 碰撞半径
        _debugDraw->drawCircle(
            Vec2::ZERO,
            _bodyRadius,
            0,
            32,
            false,
            Color4F::RED
        );

        // 攻击范围
        if (_attack && _attack->hasTarget())
        {
            _debugDraw->drawCircle(
                Vec2::ZERO,
                _attack->getAttackRange(),
                0,
                32,
                false,
                Color4F::BLUE
            );
        }
        _debugDraw->drawCircle(
            Vec2::ZERO,
            _alertRange,
            0,
            32,
            false,
            Color4F::ORANGE
        );
    }
    updateHpBarInternal();//更新血条
}

// =========================
// 组件绑定（⚠️ 之前缺失的核心）
// =========================

void TroopBase::setAIComponent(TroopAIComponent* ai)
{
    _ai = ai;
}

void TroopBase::setMoveComponent(MoveComponent* move)
{
    _move = move;
}

void TroopBase::setAttackComponent(AttackComponent* attack)
{
    _attack = attack;
}

void TroopBase::setAnimationComponent(AnimationComponent* anim)
{
    _anim = anim;
    if (_anim && _sprite)
    {
        _anim->setTargetSprite(_sprite);
    }
}

// =========================
// 战斗相关
// =========================

void TroopBase::takeDamage(int damage)
{
    if (_isDying)
        return;

    _hp -= damage;
    if (_hp < 0)
        _hp = 0;

    updateHpBarInternal();

    if (_hp <= 0)
        die();

    // 简单受伤效果
    if (_sprite && damage > 0)
    {
        _sprite->stopActionByTag(1001);  // 防止叠加

        auto tintRed = TintTo::create(0.1f, Color3B::RED);
        auto tintBack = TintTo::create(0.1f, Color3B::WHITE);

        auto seq = Sequence::create(tintRed, tintBack, nullptr);
        seq->setTag(1001);

        _sprite->runAction(seq);
    }

    showDamageNumber(damage);

    CCLOG("受到 %d 伤害，剩余血量：%d/%d", damage, _hp, _maxHp);
}

void TroopBase::die()
{
    if (_isDying)
        return;

    _isDying = true;
    unscheduleUpdate();

    if (_sprite)
    {
        _sprite->runAction(Sequence::create(
            FadeOut::create(0.3f),
            CallFunc::create([this]() {
                removeFromParentAndCleanup(true);
                }),
            nullptr
        ));
    }
    else
    {
        removeFromParentAndCleanup(true);
    }
}

// =========================
// 血条
// =========================

void TroopBase::updateHpBarInternal()
{
    if (!_hpBarInited || !_hpBarFg || _maxHp <= 0)
        return;

    float hpPercent = static_cast<float>(_hp) / _maxHp;
    hpPercent = clampf(hpPercent, 0.f, 1.f);

    _hpBarFg->setScaleX(hpPercent);
}

void TroopBase::initHpBar()
{
    _hpBarNode = Node::create();
    addChild(_hpBarNode, 100);

    float barWidth = 50.0f;
    float barHeight = 6.0f;

    _hpBarBg = LayerColor::create(Color4B(50, 50, 50, 200), barWidth, barHeight);
    // 背景
    _hpBarBg->setAnchorPoint(Vec2(0.f, 0.5f));
    _hpBarBg->setPosition(Vec2(-barWidth / 2, 0));
    _hpBarNode->addChild(_hpBarBg);

    _hpBarFg = LayerColor::create(Color4B::RED, barWidth, barHeight);
    _hpBarFg->setAnchorPoint(Vec2(0.f, 0.5f));
    _hpBarFg->setPosition(Vec2(-barWidth / 2, 0));
    _hpBarNode->addChild(_hpBarFg);

    _hpBarNode->setPosition(Vec2(0, _bodyRadius + 15.0f));
}
void TroopBase::applySlow(float ratio, float duration)//减速
{
    ratio = clampf(ratio, 0.1f, 1.0f);

    // 直接修改“当前速度”
    _currentSpeed = _moveSpeed * ratio;

    // 定时恢复
    runAction(Sequence::create(
        DelayTime::create(duration),
        CallFunc::create([this]() {
            _currentSpeed = _moveSpeed;
            }),
        nullptr
    ));
}
void TroopBase::showDamageNumber(int damage)
{
    auto label = Label::createWithTTF(
        StringUtils::format("-%d", damage),
        "fonts/arial.ttf",
        18
    );
    if (!label) return;

    label->setColor(Color3B::RED);
    label->setPosition(Vec2(0, 100));
    addChild(label, 100);

    // 简单的上浮消失动画
    auto move = MoveBy::create(0.6f, Vec2(0, 40));
    auto fade = FadeOut::create(0.6f);
    auto remove = RemoveSelf::create();
    label->runAction(Sequence::create(
        Spawn::create(move, fade, nullptr),
        remove,
        nullptr
    ));
}