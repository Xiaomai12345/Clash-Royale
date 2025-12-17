#ifndef MANA_BAR_H
#define MANA_BAR_H

#include "cocos2d.h"

class ManaBar : public cocos2d::Node
{
public:
    CREATE_FUNC(ManaBar);

    virtual bool init() override;
    virtual void update(float delta) override;

private:
    void createUI();
    void updateManaDisplay();

private:
    cocos2d::ProgressTimer* _manaBar;
    cocos2d::Label* _manaLabel;
    cocos2d::Sprite* _background;
};

#endif // MANA_BAR_H