#include "MainMenuScene.h"
#include "BattleScene.h"
#include "GameManager.h"

USING_NS_CC;

MainMenuScene* MainMenuScene::create()
{
    MainMenuScene* ret = new MainMenuScene();
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

bool MainMenuScene::init()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();

    // 创建开始游戏按钮
    auto startButton = MenuItemLabel::create(Label::createWithTTF("开始游戏", "fonts/arial.ttf", 24), CC_CALLBACK_1(MainMenuScene::onStartGame, this));
    startButton->setPosition(visibleSize.width / 2, visibleSize.height / 2);

    auto menu = Menu::create(startButton, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);

    return true;
}

void MainMenuScene::onStartGame(Ref* sender)
{
    // 切换到战斗场景
    Director::getInstance()->replaceScene(BattleScene::create());
}
