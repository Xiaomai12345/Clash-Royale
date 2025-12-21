#include "DataManager.h"

DataManager* DataManager::_instance = nullptr;

//获取实例
DataManager* DataManager::getInstance() {
    if (_instance == nullptr) {
        _instance = new DataManager();
        _instance->init();
    }
    return _instance;
}
//删除实例
void DataManager::destroyInstance() {
    if (_instance != nullptr) {
        delete _instance;
        _instance = nullptr;
    }
}

// 加载Json
bool DataManager::loadCardConfig(const std::string& jsonPath) {
    // 避免重复解析
    if (_isLoaded) {
        log("DataManager：卡牌数据已加载，无需重复加载！");
        return true;
    }

    std::string filePath = FileUtils::getInstance()->fullPathForFilename(jsonPath);
    std::string jsonContent = FileUtils::getInstance()->getStringFromFile(filePath);

    // 检查文件是否为空
    if (jsonContent.empty()) {
        log("DataManager：Json文件为空！路径：%s", filePath.c_str());
        return false;
    }

    rapidjson::Document jsonDoc;
    jsonDoc.Parse(jsonContent.c_str());

    // 检查解析错误
    if (jsonDoc.HasParseError()) {
        log("DataManager：Json解析失败！错误码：%d，路径：%s", jsonDoc.GetParseError(), filePath.c_str());
        return false;
    }

    // 检查是否为数组格式
    if (!jsonDoc.IsArray()) {
        log("DataManager：Json不是数组格式！路径：%s", filePath.c_str());
        return false;
    }

    // 解析所有卡牌数据
    _allCardData.clear();
    for (int i = 0; i < jsonDoc.Size(); i++) {
        rapidjson::Value& card = jsonDoc[i];
        ValueMap cardMap;

        // 容错处理：如果Json里没有某个字段，设置默认值（避免崩溃）
        cardMap["id"] = card.HasMember("id") ? card["id"].GetInt() : -1;
        cardMap["name"] = card.HasMember("name") ? card["name"].GetString() : "未知卡牌";
        cardMap["manaCost"] = card.HasMember("manaCost") ? card["manaCost"].GetFloat() : 0.0f;
        cardMap["imgPath"] = card.HasMember("imgPath") ? card["imgPath"].GetString() : "";

        _allCardData.push_back(Value(cardMap));
    }

    _isLoaded = true;
    log("DataManager：成功加载%d张卡牌数据！", _allCardData.size());
    return true;
}

// 根据ID获取卡牌数据
ValueMap DataManager::getCardDataById(int cardId) {
    for (auto& cardValue : _allCardData) {
        ValueMap cardMap = cardValue.asValueMap();
        if (cardMap["id"].asInt() == cardId) {
            return cardMap;
        }
    }
    log("DataManager：未找到ID为%d的卡牌！", cardId);
    return ValueMap();
}

// 新增：获取所有卡牌数据
ValueVector DataManager::getAllCardData() {
    return _allCardData;
}

// 新增：根据名称获取卡牌数据（备用）
ValueMap DataManager::getCardDataByName(const std::string& name) {
    for (auto& cardValue : _allCardData) {
        ValueMap cardMap = cardValue.asValueMap();
        if (cardMap["name"].asString() == name) {
            return cardMap;
        }
    }
    log("DataManager：未找到名称为%s的卡牌！", name.c_str());
    return ValueMap();
}

// 新增：获取卡牌总数
int DataManager::getCardCount() {
    return _allCardData.size();
}