#ifndef MANA_BAR_H
#define MANA_BAR_H

#include "cocos2d.h"
#include"ManaSystem.h"


class ManaBar : public cocos2d::Node
{
public:
    CREATE_FUNC(ManaBar);

    virtual bool init() override;
    virtual void update(float dt) override;

    // 设置圣水（0~max）
    void setManaValue(float current, float max, bool animate = true);

private:
    void createUI();
    void drawFillByPercent(float percent);
    void addDivisionLines(float width, float height);
    void updateFill();

private:
    cocos2d::Node* _background = nullptr;
    cocos2d::DrawNode* _fillNode = nullptr;
    cocos2d::DrawNode* _lineNode = nullptr;
    cocos2d::Label* _label = nullptr;
    cocos2d::Label* _label1 = nullptr;
    float _currentPercent = 0.0f;
    int   _segments = 10;
    float _width = 600.0f;
    float _height = 50.0f;
};

#endif
