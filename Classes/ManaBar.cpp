#include "ManaBar.h"

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
    // === 参数（唯一尺寸来源） ===
    _width = 600.0f;
    _height = 50.0f;

    // === 背景节点（坐标原点 = 左下角） ===
    _background = Node::create();
    _background->setAnchorPoint(Vec2::ZERO);
    _background->setPosition(Vec2::ZERO);
    addChild(_background);

    // === 背景色 ===
    auto bg = LayerColor::create(
        Color4B(231, 213, 237, 255),
        _width,
        _height
    );
    bg->setAnchorPoint(Vec2::ZERO);
    _background->addChild(bg, 0);

    // === 填充层（进度条） ===
    _fillNode = DrawNode::create();
    _background->addChild(_fillNode, 1);

    // === 分割线 ===
    _lineNode = DrawNode::create();
    _background->addChild(_lineNode, 2);
    addDivisionLines(_width, _height);

    // === 文本 ===
    _label = Label::createWithSystemFont("5/ 10", "Arial", 20);
    _label->setPosition(Vec2(_width / 2, _height / 2));
    _label->setTextColor(Color4B::WHITE);
    _label->enableOutline(Color4B::BLACK, 1);
    _background->addChild(_label, 3);
    
}

void ManaBar::addDivisionLines(float width, float height)
{
    _lineNode->clear();

    int divisions = 10;
    float segment = width / divisions;

    for (int i = 1; i < divisions; ++i)
    {
        float x = i * segment;
        _lineNode->drawLine(
            Vec2(x, 0),
            Vec2(x, height),
            Color4F(0.1f, 0.1f, 0.3f, 0.6f)
        );
    }
}

void ManaBar::update(float dt)
{
    auto manaSystem = ManaSystem::getInstance();
    float current = manaSystem->getCurrentMana();
    float max = manaSystem->getMaxMana();
    if (max <= 0.0f)
        return;

    float percent = current / max;   // 0 ~ 1

    if (_label)
    {
        _label->setString(
            StringUtils::format("%.1f / %.1f", manaSystem->getCurrentMana(), manaSystem->getMaxMana())
        );
    }
    // 防止无意义重复绘制
    if (fabs(percent - _currentPercent) < 0.001f)
        return;

    _currentPercent = percent;
    drawFillByPercent(_currentPercent);
}

void ManaBar::drawFillByPercent(float percent)
{
    _fillNode->clear();

    float fillWidth = _width * percent;
    _fillNode->drawSolidRect(
        Vec2(0, 0),
        Vec2(fillWidth, _height),
        Color4F(180 / 255.0f, 100 / 255.0f, 255 / 255.0f, 1.0f)
    );

}




