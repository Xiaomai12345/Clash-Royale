#include "Card.h"

USING_NS_CC;

bool Card::init()
{
    if (!Node::init())
        return false;

    _cardId = 0;
    _name = "Unknown";
    _manaCost = 0.0f;
    _isSelected = false;

    // 创建卡牌背景
    _cardSprite = Sprite::create();
    auto bg = LayerColor::create(Color4B(100, 100, 100, 255), 100, 140);
    _cardSprite->addChild(bg);

    _cardSprite->setAnchorPoint(Vec2::ZERO);
    addChild(_cardSprite);

    // 创建圣水消耗标签
    _manaLabel = Label::createWithSystemFont("0", "Arial", 20);
    _manaLabel->setPosition(25, 120);
    _manaLabel->setTextColor(Color4B(0, 255, 255, 255));
    _cardSprite->addChild(_manaLabel);

    // 创建名称标签
    _nameLabel = Label::createWithSystemFont("Card", "Arial", 16);
    _nameLabel->setPosition(50, 30);
    _nameLabel->setTextColor(Color4B::WHITE);
    _cardSprite->addChild(_nameLabel);

    setContentSize(Size(100, 140));

    return true;
}

void Card::setCardInfo(int cardId, const std::string& name, float manaCost)
{
    _cardId = cardId;
    _name = name;
    _manaCost = manaCost;

    // 更新UI
    _manaLabel->setString(StringUtils::format("%.0f", manaCost));
    _nameLabel->setString(name);

    // 根据圣水消耗设置颜色
    if (manaCost <= 2)
    {
        _manaLabel->setTextColor(Color4B(100, 255, 100, 255));
    }
    else if (manaCost <= 4)
    {
        _manaLabel->setTextColor(Color4B(255, 255, 100, 255));
    }
    else
    {
        _manaLabel->setTextColor(Color4B(255, 100, 100, 255));
    }
}

bool Card::use(const cocos2d::Vec2& position, int playerId)
{
    CCLOG("Using card %d (%s) at (%.1f, %.1f) for player %d",
        _cardId, _name.c_str(), position.x, position.y, playerId);

    // 这里应该触发卡牌效果
    // 返回true表示使用成功
    return true;
}

void Card::setSelected(bool selected)
{
    _isSelected = selected;

    if (selected)
    {
        _cardSprite->setScale(1.1f);
        _cardSprite->setColor(Color3B(255, 255, 0));
    }
    else
    {
        _cardSprite->setScale(1.0f);
        _cardSprite->setColor(Color3B::WHITE);
    }
}