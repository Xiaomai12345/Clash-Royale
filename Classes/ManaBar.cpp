#include "ManaBar.h"
#include "ManaSystem.h"

USING_NS_CC;

bool ManaBar::init()
{
    if (!Node::init())
        return false;

    createUI();
    scheduleUpdate();

    return true;
}

void ManaBar::createUI()
{
    // 创建背景
    _background = Sprite::create();
    auto bg = LayerColor::create(Color4B(50, 50, 100, 200), 300, 40);
    _background->addChild(bg);
    _background->setAnchorPoint(Vec2(0.5f, 0.5f));
    addChild(_background);

    // 创建进度条
    auto barSprite = Sprite::create();
    auto barColor = LayerColor::create(Color4B(100, 200, 255, 255), 280, 30);
    barSprite->addChild(barColor);

    _manaBar = ProgressTimer::create(barSprite);
    _manaBar->setType(ProgressTimer::Type::BAR);
    _manaBar->setBarChangeRate(Vec2(1, 0));
    _manaBar->setMidpoint(Vec2(0, 0.5));
    _manaBar->setPercentage(100);
    _manaBar->setPosition(0, 0);
    _background->addChild(_manaBar, 1);

    // 创建数字标签
    _manaLabel = Label::createWithSystemFont("10/10", "Arial", 20);
    _manaLabel->setPosition(0, 0);
    _manaLabel->setTextColor(Color4B::WHITE);
    _background->addChild(_manaLabel, 2);

    setContentSize(Size(300, 40));
}

void ManaBar::update(float delta)
{
    Node::update(delta);

    updateManaDisplay();
}

void ManaBar::updateManaDisplay()
{
    auto manaSystem = ManaSystem::getInstance();
    float currentMana = manaSystem->getCurrentMana();
    float maxMana = manaSystem->getMaxMana();

    // 更新进度条
    float percentage = (currentMana / maxMana) * 100.0f;
    _manaBar->setPercentage(percentage);

    // 更新数字显示
    _manaLabel->setString(StringUtils::format("%.1f/%.1f", currentMana, maxMana));

    // 根据圣水量改变颜色
    if (percentage > 70.0f)
    {
        _manaBar->setColor(Color3B(100, 255, 100));
    }
    else if (percentage > 30.0f)
    {
        _manaBar->setColor(Color3B(255, 255, 100));
    }
    else
    {
        _manaBar->setColor(Color3B(255, 100, 100));
    }
}