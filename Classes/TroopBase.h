#ifndef __TROOP_BASE_H__
#define __TROOP_BASE_H__

#include "cocos2d.h"
#include "base/CCRef.h"
#include "IAttackable.h"
USING_NS_CC;

// 前向声明（减少头文件耦合）
class TroopAIComponent;
class MoveComponent;
class AttackComponent;

// =========================
// 阵营枚举
// =========================

// =========================
// TroopBase
// =========================
//
class TroopBase :  public IAttackable
{
public:
    TroopBase();
    virtual ~TroopBase();

    virtual bool init() override;
    CREATE_FUNC(TroopBase);

    // 每帧更新（核心调度）
    virtual void update(float dt) override;

    // 受伤并更新血量
    void takeDamage(int damage);
    void updateHpBar();

    // 检查是否死亡
    bool isDead() const { return _hp <= 0; }
    bool isAlive() const { return _hp > 0; }

    // 死亡处理
    void die();

    DrawNode* getDebugDraw()
    {
        return _debugDraw;
    }


public:
    // =========================
    // 对外能力接口（供组件使用）
    // =========================

    // 移动速度（像素 / 秒）
    float getMoveSpeed() const { return _currentSpeed; } //获取速度
    void  setCurrentSpeed(float speed) {  _currentSpeed = std::max(0.0f, speed);};//设置当前速度
    void  resetMoveSpeed() { _currentSpeed = _moveSpeed;};//重置速度
    virtual void applySlow(float ratio, float duration) override;//当前士兵受到减速
    // 当前生命值
    int getHp() const { return _hp; }

    // 碰撞半径（用于距离 / 攻击判断）
    float getBodyRadius() const { return _bodyRadius; }
    void setAlertRange(float range) { _alertRange = range;}//设置警戒范围
    float getAlertRange() const {return _alertRange;}      //获取警戒范围



    // 阵营
    void setCamp(ECamp camp) { _camp = camp; }
    ECamp getCamp() const { return _camp; }
    void setAttackType(AttackType attacktype) { _attacktype = attacktype; }//设置与获取攻击类型
    AttackType getAttackType()const { return _attacktype; }
    void setState(State state) { _state = state;}//设置状态
    State getState() const { return _state; }//返回状态

public:
    // =========================
    // 组件绑定接口
    // =========================

    void setAIComponent(TroopAIComponent* ai);
    void setMoveComponent(MoveComponent* move);
    void setAttackComponent(AttackComponent* attack);

    TroopAIComponent* getAIComponent() const { return _ai; }
    MoveComponent* getMoveComponent() const { return _move; }
    AttackComponent* getAttackComponent() const { return _attack; }

protected:
    // =========================
    // 基础属性
    // =========================
    int   _hp = 0;          // 当前生命
    int   _maxHp = 0;       // 最大生命
    float _moveSpeed = 0.f; // 移动速度
    float _alertRange=50.0f;      // 警戒范围
    float _currentSpeed = 0.f; //当前的移动速度
    float _bodyRadius = 10.0f;

    State _state = State::IDLE;//一开始处于闲置状态
    ECamp _camp = ECamp::LEFT;//阵营
    AttackType _attacktype = AttackType::Both;//默认攻击类型

protected:
    // =========================
    // 组件指针（不拥有生命周期）
    // =========================
    TroopAIComponent* _ai = nullptr;
    MoveComponent* _move = nullptr;
    AttackComponent* _attack = nullptr;

protected:
    // =========================
    // Debug Only（测试阶段使用）
    // =========================

    // 士兵显示用图片
    Sprite* _sprite = nullptr;

    // 调试绘制节点（图片范围 / 碰撞范围）
    DrawNode* _debugDraw = nullptr;

    // 是否显示调试框
    bool _showDebugBounds = true;

    Node* _hpBarNode = nullptr;        // 血条根节点
    LayerColor* _hpBarBg = nullptr;    // 血条背景
    LayerColor* _hpBarFg = nullptr;    // 血条前景

protected:
    // =========================
    // 血条初始化
    // =========================
    void initHpBar();  // 声明血条初始化函数

    // 血条更新函数（内部调用）
    void updateHpBarInternal();
    bool _hpBarInited;  // 血条是否已初始化
    bool _isDying;      // 是否已经死亡
public:
    virtual Vec2 getWorldPosition() const override
    {
        return this->convertToWorldSpaceAR(Vec2::ZERO);
    }
};

#endif // __TROOP_BASE_H__
