#ifndef UNIT_BASE_H
#define UNIT_BASE_H

#include "cocos2d.h"

class TowerBase;

class UnitBase : public cocos2d::Node
{
public:
    CREATE_FUNC(UnitBase);

    virtual bool init() override;

    // 初始化单位
    virtual void initUnit(int playerId, int level, const cocos2d::Vec2& position);

    // 属性访问
    int getPlayerId() const { return _playerId; }
    int getUnitType() const { return _unitType; }
    int getLevel() const { return _level; }
    float getCurrentHealth() const { return _currentHealth; }
    float getMaxHealth() const { return _maxHealth; }
    float getAttackDamage() const { return _attackDamage; }
    float getAttackRange() const { return _attackRange; }
    float getAttackSpeed() const { return _attackSpeed; }
    float getMoveSpeed() const { return _moveSpeed; }
    bool isAlive() const { return _currentHealth > 0; }
    bool isFlying() const { return _isFlying; }
    bool isGround() const { return !_isFlying; }

    // 目标管理
    bool hasTarget() const { return _currentTarget != nullptr || _currentTargetTower != nullptr; }
    UnitBase* getTarget() const { return _currentTarget; }
    TowerBase* getTargetTower() const { return _currentTargetTower; }
    void setTarget(UnitBase* target);
    void setTargetTower(TowerBase* tower);
    void clearTarget();

    // 战斗状态
    bool isAttacking() const { return _isAttacking; }
    bool canAttack(UnitBase* target) const;
    bool canAttackTower() const;

    // 移动
    void moveTo(const cocos2d::Vec2& position, float delta);
    bool hasReachedTarget() const;

    // 战斗方法
    virtual void takeDamage(float damage);
    virtual void attack(UnitBase* target);
    virtual void attackTower(TowerBase* tower);
    virtual void update(float delta) override;

    // 特效
    void playAttackEffect();
    void playDamageEffect();
    void playDeathEffect();

    // 寻路
    void setLane(int lane) { _lane = lane; }
    int getLane() const { return _lane; }

protected:
    int _playerId;          // 所属玩家
    int _unitType;          // 单位类型ID
    int _level;             // 等级
    int _lane;              // 所在车道

    // 基本属性
    float _currentHealth;
    float _maxHealth;
    float _attackDamage;
    float _attackRange;
    float _attackSpeed;
    float _moveSpeed;
    bool _isFlying;         // 是否为飞行单位

    // 攻击目标
    UnitBase* _currentTarget;
    TowerBase* _currentTargetTower;
    float _attackTimer;
    bool _isAttacking;

    // 移动目标
    cocos2d::Vec2 _moveTarget;
    bool _hasMoveTarget;

    // 视觉元素
    cocos2d::Sprite* _unitSprite;
    cocos2d::ProgressTimer* _healthBar;

    // 创建UI
    void createHealthBar();
    void updateHealthBar();

    // 攻击检查
    bool isTargetInRange() const;
    bool isTowerInRange() const;
};

#endif // UNIT_BASE_H