#ifndef __MAINMENUSCENE_H__
#define __MAINMENUSCENE_H__

#include "cocos2d.h"

class MainMenuScene : public cocos2d::Scene
{
public:
    static MainMenuScene* create();
    static cocos2d::Scene* createScene();
    void createBackground();
    void createButton();
    bool init();

private:
    void onStartGame(cocos2d::Ref* sender);
    cocos2d::Sprite* background;
};

#endif // __MAINMENUSCENE_H__
