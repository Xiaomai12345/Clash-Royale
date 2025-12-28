#include "Projectile.h"

USING_NS_CC;

Projectile* Projectile::create(
    IAttackable* target,
    int damage,
    float speed,
    const std::function<void()>& onHitCallback)
{
    auto p = new (std::nothrow) Projectile();
    if (p && p->init(target, damage, speed, onHitCallback))
    {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

Projectile::~Projectile()
{
    // 安全释放目标引用
    if (_target)
    {
        _target->release();
        _target = nullptr;
    }
}

bool Projectile::init(
    IAttackable* target,
    int damage,
    float speed,
    const std::function<void()>& onHitCallback)
{
    if (!Node::init())
        return false;

    _target = target;
    
    // 关键修复：持有目标的引用计数，防止目标在飞行途中被销毁导致野指针崩溃
    if (_target)
    {
        _target->retain();
        _lastTargetPos = _target->getWorldPosition();
    }
    else
    {
        _targetLost = true;
        _lastTargetPos = Vec2::ZERO;
    }

    _damage = damage;
    _speed = speed;
    _onHitCallback = onHitCallback;

    _targetLost = false;

    _visual = DrawNode::create();
    _visual->drawSolidCircle(Vec2::ZERO, 5, 0, 16, Color4F::YELLOW);
    addChild(_visual);

    scheduleUpdate();
    return true;
}

void Projectile::update(float dt)
{
    // 如果没有目标且未锁定最后位置，直接销毁（理论上不应发生）
    if (!_target && !_targetLost)
    {
        removeFromParent();
        return;
    }

    if (!_targetLost && _target)
    {
        // 检查目标是否逻辑死亡或已从场景移除
        // 注意：因为我们 retain 了 _target，指针本身是安全的，不会崩溃
        if (_target->isDead() || !_target->getParent())
        {
            _targetLost = true;
            
            // 尝试获取最后一次有效位置（如果是被移除，getWorldPosition可能不准，但比没有好）
            // 如果 parent 没了，通常 worldPosition 会变成 node space 坐标，或者保留上次的值
            // 这里我们不做复杂判断，直接沿用上一帧的 _lastTargetPos 也是可以的
            // 为了保险，如果 target 还有 parent，我们再更新一次；没有就不更新了
            if (_target->getParent())
            {
                _lastTargetPos = _target->getWorldPosition();
            }

            // 既然目标已经逻辑丢失，我们不能释放引用，因为回调函数可能还在引用它
            // _target->release();
            // _target = nullptr;
        }
        else
        {
            // 目标正常，更新追踪位置
            _lastTargetPos = _target->getWorldPosition();
        }
    }
    
    // 将目标世界坐标转换为本地坐标（因为 Projectile 也是加在 Battlefield 上的，通常也是世界坐标系，但为了严谨）
    Vec2 targetPosLocal = _lastTargetPos;
    if (getParent())
    {
        targetPosLocal = getParent()->convertToNodeSpace(_lastTargetPos);
    }

    Vec2 currentPos = getPosition();
    Vec2 dir = targetPosLocal - currentPos;
    float dist = dir.length();
    float step = _speed * dt;

    if (dist <= step) // 到达
    {
        setPosition(targetPosLocal);

        if (_onHitCallback)
            _onHitCallback();

        removeFromParent();
        return;
    }

    dir.normalize();
    setPosition(currentPos + dir * step);
}
