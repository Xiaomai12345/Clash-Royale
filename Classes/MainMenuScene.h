#ifndef __MAINMENUSCENE_H__
#define __MAINMENUSCENE_H__

#include "cocos2d.h"

class MainMenuScene : public cocos2d::Scene
{
public:
    static MainMenuScene* create();
    bool init();

private:
    void onStartGame(cocos2d::Ref* sender);
};

#endif // __MAINMENUSCENE_H__
