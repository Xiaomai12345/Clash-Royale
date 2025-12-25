#include "AnimationComponent.h"
#include "TroopBase.h"

USING_NS_CC;

AnimationComponent::AnimationComponent()
    : _targetSprite(nullptr)
    , _lastState(State::IDLE)
    , _isAnimating(false)
{
}

AnimationComponent::~AnimationComponent()
{
    // 释放所有持有的 Action
    for (auto& pair : _animations)
    {
        CC_SAFE_RELEASE(pair.second);
    }
    _animations.clear();
}

void AnimationComponent::setTargetSprite(Sprite* sprite)
{
    _targetSprite = sprite;
}

void AnimationComponent::setDefaultTexture(const std::string& filename)
{
    _defaultTextureFile = filename;
}

void AnimationComponent::addAnimation(State state, Animation* animation, bool loop)
{
    if (!animation) return;

    // 创建 Action
    ActionInterval* action = Animate::create(animation);
    
    if (loop)
    {
        action = RepeatForever::create(action);
    }

    action->retain(); // 保持引用，防止被自动释放

    // 如果该状态已有动画，先释放旧的
    if (_animations.find(state) != _animations.end())
    {
        CC_SAFE_RELEASE(_animations[state]);
    }

    _animations[state] = action;
}

void AnimationComponent::addAction(State state, ActionInterval* action)
{
    if (!action) return;

    action->retain(); // 保持引用

    // 如果该状态已有动画，先释放旧的
    if (_animations.find(state) != _animations.end())
    {
        CC_SAFE_RELEASE(_animations[state]);
    }

    _animations[state] = action;
}

void AnimationComponent::update(TroopBase* owner, float dt)
{
    if (!_targetSprite || !owner) return;

    State currentState = owner->getState();

    // 特殊处理：ALERT 和 IDLE 通常共享同一个待机动画
    if (currentState == State::ALERT)
    {
        // 如果没有专门为 ALERT 设置动画，就尝试用 IDLE
        if (_animations.find(State::ALERT) == _animations.end())
        {
            currentState = State::IDLE;
        }
    }
    // 特殊处理：FOLLOWING (移动)
    // 如果是 FOLLOWING，对应移动动画

    // 如果状态发生改变，或者当前没有在播放动画（且应该播放）
    if (currentState != _lastState)
    {
        playAnimation(currentState);
        _lastState = currentState;
    }
}

void AnimationComponent::playAnimation(State state)
{
    if (!_targetSprite) return;

    // 查找对应状态的动画
    auto it = _animations.find(state);
    if (it != _animations.end())
    {
        // 停止当前所有动作 (或者只停止动画相关的 tag，这里简单起见停止所有)
        _targetSprite->stopAllActions();

        // 如果有默认贴图，先恢复默认（防止上一状态的贴图残留）
        if (!_defaultTextureFile.empty())
        {
            _targetSprite->setTexture(_defaultTextureFile);
        }

        // 播放新动画
        _targetSprite->runAction(it->second);
        _isAnimating = true;
    }
    else
    {
        // 如果没有该状态的动画，停止当前动画并恢复默认贴图
        _targetSprite->stopAllActions();
        if (!_defaultTextureFile.empty())
        {
            _targetSprite->setTexture(_defaultTextureFile);
        }
    }
}

void AnimationComponent::stopAnimation()
{
    if (_targetSprite)
    {
        _targetSprite->stopAllActions();
    }
    _isAnimating = false;
}
