#ifndef __SKELETON_LEGION_H__
#define __SKELETON_LEGION_H__

#include "cocos2d.h"
#include"SkeletonTroop.h"
class SkeletonLegion : public cocos2d::Node
{
public:
    // 
    static SkeletonLegion* create(int count, float spacing, ECamp camp);

    // 初始化
    bool init(int count, float spacing, ECamp camp);

    void setWorld(IWalkableWorld* world);
    // 生成骷髅
    void spawnAt(cocos2d::Node* parent, const cocos2d::Vec2& worldPos);

    // （可选）如果你后面想动态改阵营
    void setCamp(ECamp camp) { _camp = camp; }

private:
    IWalkableWorld* _world = nullptr;
    int _count = 0;
    float _spacing = 0.f;
    ECamp _camp = ECamp::LEFT;   // 
};

#endif
