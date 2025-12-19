#include "Battlefield.h"

USING_NS_CC;

bool Battlefield::init()
{
    if (!Node::init())
        return false;

    _debugEnabled = false;
    _mapSize = Size(1308, 1732);
    _gridSize = Size(80, 80);

    // 创建地图元素
    createBackground();
    createTowers();

    if (_debugEnabled)
    {
        createDebugLayer();
    }

    setContentSize(_mapSize);

    return true;
}

void Battlefield::setupBattlefield(int level)
{
    CCLOG("Setup battlefield level %d", level);
}

void Battlefield::createBackground()
{
    // 尝试加载图片背景
    _background = Sprite::create("Images/background/battlefield.jpg");

    // 如果图片加载失败，使用纯色背景作为fallback
    if (!_background)
    {
        CCLOG("Warning: Failed to load Images/background/battlefield.jpg, using fallback color");
        _background = Sprite::create();
        auto layerColor = LayerColor::create(Color4B(10, 200, 50, 255),
            _mapSize.width,
            _mapSize.height);
        _background->addChild(layerColor);
    }
    else
    {
        CCLOG("Successfully loaded battlefield background image");

        // 获取原始尺寸并计算缩放
        Size originalSize = _background->getContentSize();
        float scaleX = 0.8 * 0.85;
        float scaleY = 0.8 * 0.8;

        // 设置缩放
        _background->setScaleX(scaleX);
        _background->setScaleY(scaleY);


        CCLOG("Scaled size: %.0f x %.0f",
            originalSize.width * scaleX,
            originalSize.height * scaleY);
    }

    _background->setAnchorPoint(Vec2(0.5f, 1.0f));  // 锚点在顶部中间
    _background->setPosition(_mapSize.width * 0.8 * 0.85 / 2, _mapSize.height * 0.8-50);  // 放在顶部
    addChild(_background, 0);
    auto lavenderRect = LayerColor::create(Color4B(82, 150, 111, 255), _mapSize.width * 0.8 * 0.85, 50);
    lavenderRect->setPosition(0, _mapSize.height * 0.8-50);
    this->addChild(lavenderRect);


}

void Battlefield::createTowers()
{

}

void Battlefield::createDebugLayer()
{
    _debugDrawNode = DrawNode::create();
    addChild(_debugDrawNode, 100);

    // 绘制网格
    for (int x = 0; x < _mapSize.width; x += _gridSize.width)
    {
        _debugDrawNode->drawLine(Vec2(x, 0), Vec2(x, _mapSize.height), Color4F::WHITE);
    }

    for (int y = 0; y < _mapSize.height; y += _gridSize.height)
    {
        _debugDrawNode->drawLine(Vec2(0, y), Vec2(_mapSize.width, y), Color4F::WHITE);
    }

    // 绘制部署区域
    Rect player1Area(0, 0, _mapSize.width / 2, _mapSize.height);
    Rect player2Area(_mapSize.width / 2, 0, _mapSize.width / 2, _mapSize.height);

    _debugDrawNode->drawRect(player1Area.origin,
        Vec2(player1Area.getMaxX(), player1Area.getMaxY()),
        Color4F(0, 0, 1, 0.3f));

    _debugDrawNode->drawRect(player2Area.origin,
        Vec2(player2Area.getMaxX(), player2Area.getMaxY()),
        Color4F(1, 0, 0, 0.3f));
}

bool Battlefield::isValidDeployPosition(const cocos2d::Vec2& worldPos, int playerId) const
{
    // 简单检查：玩家1只能部署在左半场，玩家2在右半场
    if (playerId == 1)
    {
        return worldPos.x < _mapSize.width / 2;
    }
    else if (playerId == 2)
    {
        return worldPos.x > _mapSize.width / 2;
    }

    return false;
}

void Battlefield::enableDebugDraw(bool enable)
{
    _debugEnabled = enable;

    if (_debugDrawNode)
    {
        _debugDrawNode->setVisible(enable);
    }
}