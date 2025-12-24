#ifndef __LOADINGSCENE_H__
#define __LOADINGSCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class LoadingScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(LoadingScene);

private:
    cocos2d::Sprite* loadingBg;
    cocos2d::ui::LoadingBar* loadingBar;
    int totalResCount;
    int loadedResCount;
    // 创建加载背景
    void createLoadingBackground();
    // 预加载资源
    void preloadResources();
    // 跳转到主界面
    void jumpToMainMenu(float dt);
    //创建进度条
    void createLoadingBar();
    //用于协调进度和预加载
    static void LoadingScene::onResLoaded(cocos2d::Texture2D* texture, LoadingScene* self);
};

#endif // __LOADINGSCENE_H__