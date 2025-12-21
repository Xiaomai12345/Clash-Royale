#ifndef __SKELETON_LEGION_H__
#define __SKELETON_LEGION_H__

#include "cocos2d.h"

class SkeletonLegion : public cocos2d::Node
{
public:
    // 工厂方法
    static SkeletonLegion* create(int count, float spacing);

    // 初始化（注意：这是“自定义 init”，不是 override）
    bool init(int count, float spacing);

    // 在指定父节点 + 世界坐标生成骷髅
    void spawnAt(cocos2d::Node* parent, const cocos2d::Vec2& worldPos);

private:
    int _count = 0;        // 骷髅数量
    float _spacing = 0.f; // 横向间距
};

#endif // __SKELETON_LEGION_H__
