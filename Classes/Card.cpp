#include "Card.h"
#include <algorithm>

USING_NS_CC;

bool Card::init()
{
    if (!Node::init())
        return false;

    /* =========================
     * 卡牌尺寸（唯一尺寸源）
     * ========================= */
    Size cardSize(100, 140);

    /* =========================
     * 卡牌容器
     * ========================= */
    _cardSprite = Sprite::create();
    _cardSprite->setContentSize(cardSize);      // ⭐ 关键：给尺寸
    _cardSprite->setAnchorPoint(Vec2::ZERO);
    addChild(_cardSprite);

    setContentSize(cardSize);

    /* =========================
     * 卡牌背景
     * ========================= */
    auto bg = LayerColor::create(Color4B(80, 80, 80, 255),
        cardSize.width,
        cardSize.height);
    bg->setAnchorPoint(Vec2::ZERO);
    bg->setPosition(Vec2::ZERO);
    _cardSprite->addChild(bg, 0);

    /* =========================
     * 圣水消耗
     * ========================= */
    _manaLabel = Label::createWithSystemFont("0", "Arial", 20);
    _manaLabel->setPosition(Vec2(cardSize.width * 0.2f,
        cardSize.height * 0.85f));
    _manaLabel->setTextColor(Color4B::RED);
    _cardSprite->addChild(_manaLabel, 2);

    /* =========================
     * 卡牌名称
     * ========================= */
    _nameLabel = Label::createWithSystemFont("Card", "Arial", 16);
    _nameLabel->setPosition(Vec2(cardSize.width * 0.5f,
        cardSize.height * 0.15f));
    _nameLabel->setTextColor(Color4B::WHITE);
    _cardSprite->addChild(_nameLabel, 2);

    return true;
}

void Card::setCardInfo(int cardId,
    const std::string& name,
    int manaCost)
{
    _cardId = cardId;
    _name = name;
    _manaCost = manaCost;

    _manaLabel->setString(StringUtils::format("%d", manaCost));
    _nameLabel->setString(name);

    if (manaCost <= 2)
        _manaLabel->setTextColor(Color4B(100, 255, 100, 255));
    else if (manaCost <= 4)
        _manaLabel->setTextColor(Color4B(255, 255, 100, 255));
    else
        _manaLabel->setTextColor(Color4B(255, 100, 100, 255));
}

void Card::setPlayFunc(const PlayFunc& func)
{
    _playFunc = func;
}

bool Card::use(const Vec2& worldPos, int playerId)
{
    if (!_playFunc)
    {
        CCLOG("Card [%s] has no play function!", _name.c_str());
        return false;
    }
    return _playFunc(worldPos, playerId);
}

void Card::setSelected(bool selected)
{
    _isSelected = selected;

    if (selected)
    {
        _cardSprite->setScale(1.1f);
        _cardSprite->setColor(Color3B::YELLOW);
    }
    else
    {
        _cardSprite->setScale(1.0f);
        _cardSprite->setColor(Color3B::WHITE);
    }
}

/* =========================
 * 设置卡牌插画（铺满背景）
 * ========================= */
void Card::setCardArt(const std::string& imagePath)
{
    if (_artSprite)
    {
        _artSprite->removeFromParent();
        _artSprite = nullptr;
    }

    _artSprite = Sprite::create(imagePath);
    if (!_artSprite) return;

    // ⭐ 用 cardSprite 的尺寸（现在一定不是 0）
    Size bgSize = _cardSprite->getContentSize();
    Size imgSize = _artSprite->getContentSize();

    float scaleX = bgSize.width / imgSize.width;
    float scaleY = bgSize.height / imgSize.height;
    CCLOG("%f %f",scaleX,scaleY);

    // cover：铺满
    float scale = std::max(scaleX, scaleY);

    _artSprite->setScale(scale);
    _artSprite->setPosition(bgSize.width * 0.5f,
        bgSize.height * 0.5f);

    // 插画在背景之上、UI 之下
    _cardSprite->addChild(_artSprite, 1);
}
