#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "cocos2d.h"

class AppDelegate : private cocos2d::Application
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    virtual bool applicationDidFinishLaunching() override;
    virtual void applicationDidEnterBackground() override;
    virtual void applicationWillEnterForeground() override;

private:
    cocos2d::Director* _director;
    cocos2d::GLView* _glview;
};

#endif // _APP_DELEGATE_H_