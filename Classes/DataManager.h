#ifndef _DATAMANAGER_H_
#define _DATAMANAGER_H_

#include "cocos2d.h"
#include "json/document.h"
#include "json/rapidjson.h"
USING_NS_CC;

class DataManager {
private:
    DataManager() {}
    ~DataManager() {}
    static DataManager* _instance;

    ValueVector _allCardData; // 存储所有卡牌数据
    bool _isLoaded = false; // 标记是否已加载

public:
    // 获取单例
    static DataManager* getInstance();
    // 释放单例
    static void destroyInstance();
    //加载卡牌Json
    bool loadCardConfig(const std::string& jsonPath);
    //根据ID获取卡牌数据
    ValueMap getCardDataById(int cardId);
    //获取所有卡牌数据
    ValueVector getAllCardData();
    //根据名称获取卡牌数据
    ValueMap getCardDataByName(const std::string& name);
    //获取卡牌总数
    int getCardCount();

private:
    bool init() { return true; }
};

#endif // _DATAMANAGER_H_
