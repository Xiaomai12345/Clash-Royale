#include "BattleScene.h"
#include "cocos2d.h"

USING_NS_CC;

BattleScene* BattleScene::create()
{
    BattleScene* ret = new BattleScene();
    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        delete ret;
        return nullptr;
    }
}

bool BattleScene::init()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();  // 获取屏幕尺寸
    Vec2 origin = Director::getInstance()->getVisibleOrigin();     // 获取屏幕原点

    // 加载背景图片
    auto background = Sprite::create("background/battlefield.jpg");
    if (background)
    {
        // 确保背景图像适配屏幕尺寸
        background->setPosition(visibleSize / 2);  // 设置背景图片的位置为屏幕中心
        background->setScale(visibleSize.width / background->getContentSize().width,
            visibleSize.height / background->getContentSize().height);  // 根据屏幕尺寸缩放背景图像
        this->addChild(background);  // 添加背景到当前场景
    }
    else
    {
        CCLOG("背景图片加载失败！");
    }

    // 其他的战斗元素，比如卡牌、按钮等可以在这里添加
    // ...

    return true;
}

void BattleScene::deployCard()
{
    // 在这里处理卡牌部署的逻辑
    CCLOG("卡牌已部署！");
}

void BattleScene::update(float dt)
{
    // 每帧更新战斗逻辑
    // 这里可以处理单位的移动、碰撞检测等
}
