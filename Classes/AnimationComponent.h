#ifndef __ANIMATION_COMPONENT_H__
#define __ANIMATION_COMPONENT_H__

#include "cocos2d.h"
#include "IAttackable.h" // 包含 State 枚举

class TroopBase;

class AnimationComponent
{
public:
    AnimationComponent();
    ~AnimationComponent();

    // 每帧更新，检测状态变化
    void update(IAttackable* owner, float dt);

    // 注册基于帧的动画 
    void addAnimation(State state, cocos2d::Animation* animation, bool loop = true);

    // 注册通用动作
    // 适用于没有序列帧资源，想用代码模拟动画的情况
    void addAction(State state, cocos2d::ActionInterval* action);

    // 手动强制播放某个状态的动画
    void playAnimation(State state);

    // 停止当前动画
    void stopAnimation();

    // 设置目标 Sprite (通常是 TroopBase 的 _sprite)
    void setTargetSprite(cocos2d::Sprite* sprite);

    // 设置用于恢复的默认贴图路径（用于攻击等一次性动画结束后恢复原样）
    void setDefaultTexture(const std::string& filename);

private:
    cocos2d::Sprite* _targetSprite;
    std::string _defaultTextureFile; // 默认贴图路径
    std::map<State, cocos2d::Action*> _animations; // 存储状态对应的 Action
    
    State _lastState; // 上一帧的状态
    bool _isAnimating;
    bool _firstRun;
};

#endif // __ANIMATION_COMPONENT_H__
