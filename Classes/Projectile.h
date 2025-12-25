#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include "cocos2d.h"
#include "IAttackable.h"
#include <functional>

class Projectile : public cocos2d::Node
{
public:
    static Projectile* create(
        IAttackable* target,
        int damage,
        float speed,
        const std::function<void()>& onHitCallback
    );

    bool init(
        IAttackable* target,
        int damage,
        float speed,
        const std::function<void()>& onHitCallback
    );

    virtual void update(float dt) override;
    
    virtual ~Projectile();

private:
    IAttackable* _target = nullptr;
    int _damage = 0;
    float _speed = 0.0f;

    std::function<void()> _onHitCallback;

    cocos2d::Vec2 _lastTargetPos;
    bool _targetLost = false;

    cocos2d::DrawNode* _visual = nullptr;
};

#endif // __PROJECTILE_H__
