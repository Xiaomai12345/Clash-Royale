#include "SkeletonLegion.h"
#include "SkeletonTroop.h"

USING_NS_CC;

SkeletonLegion* SkeletonLegion::create(int count, float spacing)
{
    auto ret = new (std::nothrow) SkeletonLegion();
    if (ret && ret->init(count, spacing))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool SkeletonLegion::init(int count, float spacing)
{
    if (!Node::init())
        return false;

    _count = count;
    _spacing = spacing;
    return true;
}

void SkeletonLegion::spawnAt(Node* parent, const Vec2& worldPos)
{
    if (!parent)
    {
        CCLOG("SkeletonLegion::spawnAt parent is null");
        return;
    }

    const int perRow = 4;        // 每行 4 个
    const float rowSpacing = 12.f;

    for (int i = 0; i < _count; ++i)
    {
        auto skeleton = SkeletonTroop::create();
        if (!skeleton)
            continue;

        int row = i / perRow;
        int col = i % perRow;

        float offsetX = (col - (perRow - 1) * 0.5f) * _spacing;
        float offsetY = row * rowSpacing;

        skeleton->setPosition(worldPos + Vec2(offsetX, offsetY));
        parent->addChild(skeleton);
    }

    // 一次性生成器，用完自毁
    removeFromParent();
}
