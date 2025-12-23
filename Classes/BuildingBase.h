
#ifndef __BUILDING_BASE_H__
#define __BUILDING_BASE_H__

#include "cocos2d.h"
#include "base/CCRef.h"
#include "IAttackable.h"
USING_NS_CC;

// 前向声明（减少头文件耦合）
class BuildingAI;
class MoveComponent;
class AttackComponent;


class BuildingBase : public IAttackable  // ✅ 修正：类名拼写
{
public:
    BuildingBase();
    virtual ~BuildingBase();  // ✅ 修正：析构函数名拼写

    virtual bool init() override;
    CREATE_FUNC(BuildingBase);  // ✅ 修正：宏名拼写

    // 每帧更新（核心调度）
    virtual void update(float dt) override;

    // 受伤并更新血量
    void takeDamage(int damage);
    void updateHpBar();
    void showDamageNumber(int damage);

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

    // 当前生命值
    virtual void applySlow(float ratio, float duration) override ;//免疫
    int getHp() const { return _hp; }

    // 碰撞半径（用于距离 / 攻击判断）
    float getBodyRadius() const { return _bodyRadius; }

    // 阵营
    void setCamp(ECamp camp) { _camp = camp; }
    ECamp getCamp() const { return _camp; }
    void setAttackType(AttackType attacktype) {}//设置与获取攻击类型
    AttackType getAttackType()const { return AttackType::Both; }
    void setMoveAttack(MoveAttack moveAttack) { _moveAttack = moveAttack; }//设置
    MoveAttack getMoveAttack()const { return _moveAttack; }//用以返回
    void setMoveAttacked(MoveAttack moveAttacked) { _moveAttacked = moveAttacked; }//设置
    MoveAttack getMoveAttacked()const { return _moveAttacked; }//用以返回
    MoveType getMoveType()const { return MoveType::Ground; }//用不到，但需要定义
	void setState(State state) { _state = state; }//设置当前状态
	State getState()const override { return _state; }//获取当前状态
public:
    // =========================
    // 组件绑定接口
    // =========================

    void setAIComponent(BuildingAI* ai);  // ✅ 修正：函数声明前的空格
    void setAttackComponent(AttackComponent* attack);  // ✅ 修正：函数声明前的空格

    BuildingAI* getAIComponent() const { return _ai; }
    AttackComponent* getAttackComponent() const { return _attack; }

protected:
    // =========================
    // 基础属性
    // =========================
    int   _hp = 0;          // 当前生命
    int   _maxHp = 0;       // 最大生命
    float _bodyRadius = 10.0f;

    ECamp _camp = ECamp::LEFT;
    MoveAttack _moveAttack = MoveAttack::Both;//默认空地均可攻击
    MoveAttack _moveAttacked = MoveAttack::Both;//默认均可被攻击
	State _state = State::IDLE; // 当前状态
protected:
    // =========================
    // 组件指针（不拥有生命周期）
    // =========================
    BuildingAI* _ai = nullptr;
    AttackComponent* _attack = nullptr;

protected:
    // =========================
    // Debug Only（测试阶段使用）
    // =========================

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

#endif // __BUILDING_BASE_H__