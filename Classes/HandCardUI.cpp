#include "HandCardUI.h"

USING_NS_CC;

bool HandCardUI::init()
{
    if (!Node::init())
        return false;

    createBackground();

    setContentSize(Size(800, 160));

    return true;
}

void HandCardUI::createBackground()
{
    _background = Sprite::create();
    auto bg = LayerColor::create(Color4B(0, 0, 0, 150), 800, 160);
    _background->addChild(bg);

    _background->setAnchorPoint(Vec2(0.5f, 0.5f));
    addChild(_background);
}

void HandCardUI::addCard(Card* card)
{
    if (!card)
        return;

    _cards.pushBack(card);
    addChild(card);

    updateLayout();
}

void HandCardUI::removeCard(Card* card)
{
    if (!card)
        return;

    _cards.eraseObject(card);
    card->removeFromParent();

    updateLayout();
}

void HandCardUI::updateLayout()
{
    int cardCount = static_cast<int>(_cards.size());
    if (cardCount == 0)
        return;

    float totalWidth = 800;
    float cardWidth = 100;
    float spacing = (totalWidth - cardCount * cardWidth) / (cardCount + 1);

    float startX = -totalWidth / 2 + spacing + cardWidth / 2;

    for (int i = 0; i < cardCount; i++)
    {
        Card* card = _cards.at(i);
        float x = startX + i * (cardWidth + spacing);
        card->setPosition(x, 0);
    }
}