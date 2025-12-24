#ifndef TOWER_BASE_H
#define TOWER_BASE_H

#include "cocos2d.h"

class UnitBase;

class TowerBase : public cocos2d::Node
{
public:
    CREATE_FUNC(TowerBase);

    virtual bool init() override;

    // 初始化塔
    void initWithData(int playerId, int towerType, int towerId);

    // 属性访问
    int getPlayerId() const { return _playerId; }
    int getTowerType() const { return _towerType; }
    int getTowerId() const { return _towerId; }
    bool isAlive() const { return _isAlive; }
    bool isKingTower() const { return _towerType == 1; }

    // 战斗方法
    void takeDamage(float damage);

    // 更新
    virtual void update(float delta) override;

private:
    void createSprite();
    void createHealthBar();
    void updateHealthBar();

private:
    int _playerId;
    int _towerType;  // 1:国王塔, 2:公主塔
    int _towerId;
    bool _isAlive;

    // 属性
    float _currentHealth;
    float _maxHealth;

    // 视觉元素
    cocos2d::Sprite* _towerSprite;
    cocos2d::ProgressTimer* _healthBar;
};

#endif // TOWER_BASE_H