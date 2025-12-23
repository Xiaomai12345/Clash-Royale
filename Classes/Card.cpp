#include "Card.h"
#include <algorithm>

USING_NS_CC;

bool Card::init()
{
    if (!Node::init())
        return false;

    /* =========================
     *  卡牌尺寸（唯一尺寸源）
     * ========================= */
    Size cardSize(120, 160);

    /* =========================
     * 卡牌容器
     * ========================= */
    _cardSprite = Sprite::create();
    _cardSprite->setContentSize(cardSize);      // 关键：给尺寸
    _cardSprite->setAnchorPoint(Vec2::ZERO);
    addChild(_cardSprite,200);

    setContentSize(cardSize);



    //圣水图标
    auto manaIcon = Sprite::create("Images/background/mana.webp");
    if (manaIcon)
    {
        // 图标缩放（根据素材大小调整）
        manaIcon->setScale(0.6f);

        // 槽位正下方居中
        manaIcon->setPosition(Vec2(cardSize.width * 0.5f,
            0));

        _cardSprite->addChild(manaIcon, 100);
    }
    /* =========================
     * 圣水消耗
     * ========================= */
    _manaLabel = Label::createWithSystemFont("0", "Arial", 40);
    _manaLabel->setPosition(Vec2(cardSize.width * 0.5f,
        0));
    _manaLabel->setTextColor(Color4B::WHITE);
    _cardSprite->addChild(_manaLabel, 200);



    return true;
}

void Card::setCardInfo(int cardId,
    const std::string& name,
    int manaCost, const std::string& drawPath)
{
    _cardId = cardId;
    _name = name;
    _manaCost = manaCost;
    _drawPath = drawPath;

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

    // 用 cardSprite 的尺寸（现在一定不是 0）
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
bool Card::hitTest(const Vec2& worldPos) const
{
    Vec2 local = this->convertToNodeSpace(worldPos);
    Rect rect(Vec2::ZERO, getContentSize());
    return rect.containsPoint(local);
}
void Card::refreshView()
{

    // 圣水
    _manaLabel->setString(StringUtils::format("%d", (int)_manaCost));

    // 插画
    setCardArt(_drawPath);

}