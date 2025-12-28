
#ifndef __BUILDING_BASE_H__
#define __BUILDING_BASE_H__

#include "cocos2d.h"
#include "base/CCRef.h"
#include "IAttackable.h"
#include "AnimationComponent.h"
USING_NS_CC;


class BuildingAI;
class MoveComponent;
class AttackComponent;


class BuildingBase : public IAttackable 
{
public:
    BuildingBase();
    virtual ~BuildingBase();  

    virtual bool init() override;
    CREATE_FUNC(BuildingBase);  

    virtual void update(float dt) override;

    void takeDamage(int damage);
    void updateHpBar();
    void showDamageNumber(int damage);

    bool isDead() const { return _hp <= 0; }
    bool isAlive() const { return _hp > 0; }

    virtual void die();

    DrawNode* getDebugDraw()
    {
        return _debugDraw;
    }


public:

    virtual void applySlow(float ratio, float duration) override ;
    int getHp() const { return _hp; }

    float getBodyRadius() const { return _bodyRadius; }

    virtual void setCamp(ECamp camp) { _camp = camp; }
    ECamp getCamp() const { return _camp; }
    void setAttackType(AttackType attacktype) {}//设置与获取攻击类型
    AttackType getAttackType()const { return AttackType::Both; }
    void setMoveAttack(MoveAttack moveAttack) { _moveAttack = moveAttack; }
    MoveAttack getMoveAttack()const { return _moveAttack; }//用以返回
    void setMoveAttacked(MoveAttack moveAttacked) { _moveAttacked = moveAttacked; }
    MoveAttack getMoveAttacked()const { return _moveAttacked; }//用以返回
    MoveType getMoveType()const { return MoveType::Ground; }//用不到，但需要定义
	void setState(State state) { _state = state; }//设置当前状态
	State getState()const override { return _state; }//获取当前状态
public:


    void setAIComponent(BuildingAI* ai); 
    void setAttackComponent(AttackComponent* attack);  
    void setAnimationComponent(AnimationComponent* anim);
    BuildingAI* getAIComponent() const { return _ai; }
    AttackComponent* getAttackComponent() const { return _attack; }

protected:

    int   _hp = 0;          // 当前生命
    int   _maxHp = 0;       // 最大生命
    float _bodyRadius = 10.0f;

    ECamp _camp = ECamp::LEFT;
    MoveAttack _moveAttack = MoveAttack::Both;//默认空地均可攻击
    MoveAttack _moveAttacked = MoveAttack::Both;//默认均可被攻击
	State _state = State::IDLE; // 当前状态
protected:

    BuildingAI* _ai = nullptr;
    AttackComponent* _attack = nullptr;
	AnimationComponent* _anim = nullptr;

protected:


    // 建筑显示用图片
    Sprite* _sprite = nullptr;

    // 调试绘制节点（图片范围 / 碰撞范围）
    DrawNode* _debugDraw = nullptr;

    // 是否显示调试框
    bool _showDebugBounds = true;

    Node* _hpBarNode = nullptr;        // 血条根节点
    LayerColor* _hpBarBg = nullptr;    // 血条背景
    LayerColor* _hpBarFg = nullptr;    // 血条前景

protected:

    void initHpBar();  // 声明血条初始化函数

    // 血条更新函数（内部调用）
    void updateHpBarInternal();
    bool _hpBarInited;  // 血条是否已初始化
    bool _isDying;      // 是否已经死亡
    bool _shouldRotate = false; // 是否允许旋转（如加农炮需要旋转，塔不需要）

public:
    virtual Vec2 getWorldPosition() const override
    {
        return this->convertToWorldSpaceAR(Vec2::ZERO);
    }
};

#endif // __BUILDING_BASE_H__