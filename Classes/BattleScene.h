#ifndef __BATTLESCENE_H__
#define __BATTLESCENE_H__

#include "cocos2d.h"

class BattleScene : public cocos2d::Scene
{
public:
    static BattleScene* create();
    bool init();

private:
    void deployCard();
    void update(float dt);
};

#endif // __BATTLESCENE_H__
