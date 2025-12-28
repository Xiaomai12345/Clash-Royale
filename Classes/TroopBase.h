#ifndef __TROOP_BASE_H__
#define __TROOP_BASE_H__

#include "cocos2d.h"
#include "base/CCRef.h"
#include "IWalkableWorld.h"
#include "IAttackable.h"
#include <algorithm>
USING_NS_CC;


class TroopAIComponent;
class MoveComponent;
class AttackComponent;
class AnimationComponent; // 前向声明
class Battlefield;

class TroopBase :  public IAttackable
{
public:
    TroopBase();
    virtual ~TroopBase();

    virtual bool init() override;
    CREATE_FUNC(TroopBase);

    virtual void update(float dt) override;

    void takeDamage(int damage);
    void showDamageNumber(int damage);
    void updateHpBar();

    bool isDead() const { return _hp <= 0; }
    bool isAlive() const { return _hp > 0; }

    void die();

    DrawNode* getDebugDraw()
    {
        return _debugDraw;
    }


public:



    float getMoveSpeed() const { return _currentSpeed; } 
    void  setCurrentSpeed(float speed) {  _currentSpeed = std::max(0.0f, speed);};
    void  resetMoveSpeed() { _currentSpeed = _moveSpeed;};
    virtual void applySlow(float ratio, float duration) override;

    int getHp() const { return _hp; }

    float getBodyRadius() const { return _bodyRadius; }
    void setAlertRange(float range) { _alertRange = range;}
    float getAlertRange() const {return _alertRange;}      

    void setCamp(ECamp camp) { _camp = camp; }
    ECamp getCamp() const { return _camp; }
    void setAttackType(AttackType attacktype) { _attacktype = attacktype; }
    AttackType getAttackType()const { return _attacktype; }
    void setState(State state) { _state = state;}
    State getState() const { return _state; }
    void setMoveAttack(MoveAttack moveAttack) { _moveAttack = moveAttack; }
    MoveAttack getMoveAttack()const { return _moveAttack; }
    void setMoveAttacked(MoveAttack moveAttacked) { _moveAttacked = moveAttacked; }
    MoveAttack getMoveAttacked()const { return _moveAttacked; }
    void setMoveType(MoveType moveType) { _moveType = moveType; }
    MoveType getMoveType()const { return _moveType; }
    void setWorld(IWalkableWorld* world){ _world = world; }
    IWalkableWorld* getWorld() const{ return _world;}

public:


    void setAIComponent(TroopAIComponent* ai);
    void setMoveComponent(MoveComponent* move);
    void setAttackComponent(AttackComponent* attack);
    void setAnimationComponent(AnimationComponent* anim); 

    TroopAIComponent* getAIComponent() const { return _ai; }
    MoveComponent* getMoveComponent() const { return _move; }
    AttackComponent* getAttackComponent() const { return _attack; }
    AnimationComponent* getAnimationComponent() const { return _anim; } // 获取动画组件
    
    // 获取显示精灵（供动画组件使用）
    Sprite* getSprite() const { return _sprite; }

protected:

    int   _hp = 0;         
    int   _maxHp = 0;       
    float _moveSpeed = 0.f; 
    float _alertRange=0.f;    
    float _currentSpeed = 0.f; 
    float _bodyRadius = 10.0f;

    State _state = State::IDLE;
    ECamp _camp = ECamp::LEFT;
    AttackType _attacktype = AttackType::Both;
    MoveAttack _moveAttack = MoveAttack::Both;
    MoveAttack _moveAttacked = MoveAttack::Both;
    MoveType   _moveType = MoveType::Ground;
    IWalkableWorld* _world = nullptr;

protected:

    TroopAIComponent* _ai = nullptr;
    MoveComponent* _move = nullptr;
    AttackComponent* _attack = nullptr;
    AnimationComponent* _anim = nullptr; // 动画组件指针

protected:



    Sprite* _sprite = nullptr;

    DrawNode* _debugDraw = nullptr;


    bool _showDebugBounds = 0;

    Node* _hpBarNode = nullptr;   
    LayerColor* _hpBarBg = nullptr;   
    LayerColor* _hpBarFg = nullptr;    

protected:

    void initHpBar();  

    void updateHpBarInternal();
    bool _hpBarInited;  
    bool _isDying;      
public:
    virtual Vec2 getWorldPosition() const override
    {
        return this->convertToWorldSpaceAR(Vec2::ZERO);
    }
};

#endif // __TROOP_BASE_H__
