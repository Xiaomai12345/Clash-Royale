#include "AttackComponent.h"

AttackComponent::AttackComponent(float range, float interval)
    : _attackRange(range)
    , _attackInterval(interval)
    , _cooldown(0.f)
{
}

void AttackComponent::setTarget(IAttackable* target)
{
    _target = target;
}

void AttackComponent::clearTarget()
{
    _target = nullptr;
}

void AttackComponent::update(IAttackable* owner, float dt)
{
    if (!owner || !_target)
        return;

    if (owner->isDead() || _target->isDead())
    {
        clearTarget();
        return;
    }

 
    if (_cooldown > 0.f)
        _cooldown -= dt;

 
    if (!isTargetInRange(owner))
        return;


    if (_cooldown <= 0.f)
    {
        doAttack(owner);
        _cooldown = _attackInterval;
    }
}

bool AttackComponent::isTargetInRange(IAttackable* owner) const
{
    Vec2 from = owner->getWorldPosition();
    Vec2 to = _target->getWorldPosition();

    float dist = from.distance(to);

    float effectiveRange =
        _attackRange +
        _target->getBodyRadius();

    return dist - effectiveRange<=0.1f;
}
