#ifndef __GIANT_H__
#define __GIANT_H__

#include "TroopBase.h"


class Giant : public TroopBase
{
public:
    Giant();  // 声明构造函数
    ~Giant();
    virtual bool init() override;
    virtual void update(float dt) override; // 重写 update 以处理旋转

    CREATE_FUNC(Giant);  // 宏，用于简化创建对象

};

#endif // __GIANT_H__
