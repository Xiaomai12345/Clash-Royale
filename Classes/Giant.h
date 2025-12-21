#ifndef __GIANT_H__
#define __GIANT_H__

#include "TroopBase.h"

// Giant
// -----------
// 巨人单位
// 用于测试 AI / Move / Attack 是否能正常工作
//
class Giant : public TroopBase
{
public:
    Giant();  // 声明构造函数
    ~Giant();
    virtual bool init() override;

    CREATE_FUNC(Giant);  // 宏，用于简化创建对象

};

#endif // __GIANT_H__
