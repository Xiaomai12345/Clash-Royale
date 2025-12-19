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
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    // 创建背景 - 固定在屏幕底部居中
    _background = Sprite::create();
    auto bg= LayerColor::create(Color4B(100, 200, 255, 255), visibleSize.width/4*3, 100);
    bg->setAnchorPoint(Vec2(0.5, 0.5));
    _background->addChild(bg);
    
    _background->setPosition(visibleSize.width / 4, visibleSize.height/2);
    addChild(_background, 150);

}

void ManaBar::update(float delta)
{
    Node::update(delta);

    updateManaDisplay();
}

void ManaBar::updateManaDisplay()
{
    return;
}