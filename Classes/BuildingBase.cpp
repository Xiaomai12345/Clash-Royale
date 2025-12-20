#include "BuildingBase.h"
#include "BuildingAI.h"
#include "MoveComponent.h"
#include "AttackComponent.h"

USING_NS_CC;

BuildingBase::BuildingBase()  // ✅ 修正：构造函数名拼写
    : _hp(0)
    , _maxHp(0)
    , _bodyRadius(20.0f)
    , _ai(nullptr)
    , _attack(nullptr)
    , _sprite(nullptr)
    , _debugDraw(nullptr)
    , _showDebugBounds(true)
    , _hpBarNode(nullptr)
    , _hpBarBg(nullptr)
    , _hpBarFg(nullptr)
    , _hpBarInited(false)
    , _isDying(false)
    , _camp(ECamp::LEFT)
{
}

BuildingBase::~BuildingBase()  // ✅ 修正：析构函数名拼写
{
}

bool BuildingBase::init()  // ✅ 修正：函数名拼写
{
    if (!Node::init())
        return false;

    // ✅ 修正：如果_maxHp为0，先设置默认值
    if (_maxHp == 0) {
        _maxHp = 1000;  // 默认血量
    }
    _hp = _maxHp;  // 初始化血量

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

    scheduleUpdate();
    return true;
}

void BuildingBase::update(float dt)
{
    // 调用AI组件更新
    if (_ai)
    {
        _ai->update(this, dt);
    }

    // 攻击组件更新
    if (_attack)  // ✅ 修正：去掉多余的C括号
    {
        _attack->update(this, dt);
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
        if (_attack)
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

    }

    updateHpBarInternal();  // 更新血条
}

// =========================
// 组件绑定
// =========================

void BuildingBase::setAIComponent(BuildingAI* ai)
{
    _ai = ai;
}

void BuildingBase::setAttackComponent(AttackComponent* attack)
{
    _attack = attack;
}

// =========================
// 战斗相关
// =========================

void BuildingBase::takeDamage(int damage)
{
    if (_isDying)
        return;

    _hp -= damage;
    if (_hp < 0)
        _hp = 0;

    updateHpBarInternal();

    if (_hp <= 0)
        die();
}

void BuildingBase::die()
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

void BuildingBase::updateHpBarInternal()
{
    if (!_hpBarInited || !_hpBarFg || _maxHp <= 0)
        return;

    float hpPercent = static_cast<float>(_hp) / _maxHp;
    hpPercent = clampf(hpPercent, 0.f, 1.f);

    _hpBarFg->setScaleX(hpPercent);
}

void BuildingBase::initHpBar()  // 初始化血量条
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
void BuildingBase::applySlow(float ratio, float duration)
{
    // 建筑免疫减速
}