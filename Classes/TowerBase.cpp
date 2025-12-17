#include "TowerBase.h"

USING_NS_CC;

bool TowerBase::init()
{
    if (!Node::init())
        return false;

    _playerId = 0;
    _towerType = 0;
    _towerId = 0;
    _isAlive = true;
    _currentHealth = 0;
    _maxHealth = 0;

    return true;
}

void TowerBase::initWithData(int playerId, int towerType, int towerId)
{
    _playerId = playerId;
    _towerType = towerType;
    _towerId = towerId;
    _isAlive = true;

    // 设置血量
    if (towerType == 1) // 国王塔
    {
        _maxHealth = 2400;
    }
    else // 公主塔
    {
        _maxHealth = 1400;
    }
    _currentHealth = _maxHealth;

    // 创建视觉元素
    createSprite();
    createHealthBar();

    // 开启更新
    scheduleUpdate();
}

void TowerBase::createSprite()
{
    _towerSprite = Sprite::create();

    // 创建塔的形状
    Size towerSize;
    Color4B towerColor;

    if (_towerType == 1) // 国王塔
    {
        towerSize = Size(120, 160);
        towerColor = _playerId == 1 ? Color4B(0, 0, 200, 255) : Color4B(200, 0, 0, 255);
    }
    else // 公主塔
    {
        towerSize = Size(80, 120);
        towerColor = _playerId == 1 ? Color4B(100, 100, 255, 255) : Color4B(255, 100, 100, 255);
    }

    auto towerBody = LayerColor::create(towerColor, towerSize.width, towerSize.height);
    _towerSprite->addChild(towerBody);

    // 添加塔顶
    auto towerTop = LayerColor::create(Color4B(150, 150, 150, 255),
        towerSize.width * 0.8f,
        towerSize.height * 0.2f);
    towerTop->setPosition(towerSize.width * 0.1f, towerSize.height * 0.8f);
    towerBody->addChild(towerTop);

    _towerSprite->setAnchorPoint(Vec2(0.5f, 0));
    addChild(_towerSprite);
}

void TowerBase::createHealthBar()
{
    _healthBar = ProgressTimer::create(Sprite::create());
    auto bar = LayerColor::create(Color4B::GREEN, 80, 8);
    _healthBar->addChild(bar);

    _healthBar->setType(ProgressTimer::Type::BAR);
    _healthBar->setBarChangeRate(Vec2(1, 0));
    _healthBar->setMidpoint(Vec2(0, 0.5));
    _healthBar->setPercentage(100);
    _healthBar->setPosition(0, 100);

    addChild(_healthBar, 10);
}

void TowerBase::update(float delta)
{
    Node::update(delta);

    if (!_isAlive)
        return;

    // 更新血条
    updateHealthBar();
}

void TowerBase::takeDamage(float damage)
{
    if (!_isAlive)
        return;

    _currentHealth -= damage;
    if (_currentHealth < 0)
        _currentHealth = 0;

    // 闪烁效果
    auto tintTo = TintTo::create(0.1f, 255, 100, 100);
    auto tintBack = TintTo::create(0.1f, 255, 255, 255);
    auto sequence = Sequence::create(tintTo, tintBack, nullptr);
    _towerSprite->runAction(sequence);

    // 如果死亡
    if (_currentHealth <= 0)
    {
        _isAlive = false;
        _towerSprite->setColor(Color3B::GRAY);
        unscheduleUpdate();
    }
}

void TowerBase::updateHealthBar()
{
    float percentage = (_currentHealth / _maxHealth) * 100.0f;
    _healthBar->setPercentage(percentage);

    // 根据血量改变颜色
    if (percentage > 50.0f)
    {
        _healthBar->setColor(Color3B::GREEN);
    }
    else if (percentage > 25.0f)
    {
        _healthBar->setColor(Color3B::YELLOW);
    }
    else
    {
        _healthBar->setColor(Color3B::RED);
    }
}