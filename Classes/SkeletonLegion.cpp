#include "SkeletonLegion.h"

USING_NS_CC;

SkeletonLegion* SkeletonLegion::create(int count, float spacing, ECamp camp)
{
    auto ret = new (std::nothrow) SkeletonLegion();
    if (ret && ret->init(count, spacing, camp))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool SkeletonLegion::init(int count, float spacing, ECamp camp)
{
    if (!Node::init())
        return false;

    _count = count;
    _spacing = spacing;
    _camp = camp;   // 

    return true;
}

void SkeletonLegion::spawnAt(Node* parent, const Vec2& worldPos)
{
    if (!parent)
        return;

    const int perRow = 4;
    const float rowSpacing = 12.f;

    for (int i = 0; i < _count; ++i)
    {
        auto skeleton = SkeletonTroop::create();
        if (!skeleton)
            continue;

        // 
        skeleton->setCamp(_camp);

        int row = i / perRow;
        int col = i % perRow;

        float offsetX = (col - (perRow - 1) * 0.5f) * _spacing;
        float offsetY = row * rowSpacing;

        skeleton->setPosition(worldPos + Vec2(offsetX, offsetY));
        skeleton->setWorld(_world);
        parent->addChild(skeleton);
    }

    // 一次性生成器，用完即销毁
    removeFromParent();
}
void SkeletonLegion::setWorld(IWalkableWorld* world)
{
    _world = world;
}