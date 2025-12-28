#include "DataManager.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
DataManager* DataManager::_instance = nullptr;
using namespace cocos2d;
using namespace rapidjson;
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
        /*log("DataManager：卡牌数据已加载，无需重复加载！");*/
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
        cardMap["attackRange"] = card.HasMember("attackRange") ? card["attackRange"].GetFloat() : 0.0f;
        cardMap["viewRange"] = card.HasMember("viewRange") ? card["viewRange"].GetFloat() : 0.0f;
        cardMap["health"] = card.HasMember("health") ? card["health"].GetFloat() : 0.0f;
        cardMap["attackPower"] = card.HasMember("attackPower") ? card["attackPower"].GetFloat() : 0.0f;
        cardMap["attackSpeed"] = card.HasMember("attackSpeed") ? card["attackSpeed"].GetFloat() : 0.0f;
        cardMap["moveSpeed"] = card.HasMember("moveSpeed") ? card["moveSpeed"].GetFloat() : 0.0f;
        cardMap["level"] = card.HasMember("level") ? card["level"].GetInt() : 1;
        cardMap["numberOfCard"] = card.HasMember("numberOfCard") ? card["numberOfCard"].GetInt() : 1;
        cardMap["rarity"] = card.HasMember("rarity") ? card["rarity"].GetString() : "";
        cardMap["volume"] = card.HasMember("volume") ? card["volume"].GetFloat() : 0.0f;
        _allCardData.push_back(cocos2d::Value(cardMap));
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

bool DataManager::upgrade(const std::string& name)
{
    for (auto& cardValue : _allCardData)
    {
        ValueMap cardMap = cardValue.asValueMap();
        if (cardMap["name"].asString() == name) 
        {
            if (cardMap["numberOfCard"].asInt() >= _requiredCardOfUpgrade[cardMap["level"].asInt()])
            {
                float health = cardMap["health"].asFloat();
                health *= 1.1f;
                float attack = cardMap["attackPower"].asFloat();
                attack *= 1.1f;
                cardMap["health"] = health;      // 写回
                cardMap["attackPower"] = attack;

                int card= cardMap["numberOfCard"].asInt();
                card -= _requiredCardOfUpgrade[cardMap["level"].asInt()];
                int level = cardMap["level"].asInt();
                level += 1;
                cardValue = cocos2d::Value(cardMap);      // 再写回 ValueVector
                //saveSingleCardToFile(cardMap);
                return true;
            }

        }
        return false;
    }
    return false;
}
//更新数据文件，但是由于运行时不能更改resources文件，只能放在别的地方
//void saveSingleCardToFile(const ValueMap& cardData)
//{
//    // 1. 创建 JSON 文档
//    Document doc;
//    doc.SetObject();
//    Document::AllocatorType& allocator = doc.GetAllocator();
//
//    // 2. 按你的格式逐字段写
//    doc.AddMember("id", cardData.at("id").asInt(), allocator);
//    doc.AddMember("name",
//        rapidjson::Value(cardData.at("name").asString().c_str(), allocator),
//        allocator);
//
//    doc.AddMember("manaCost", cardData.at("manaCost").asFloat(), allocator);
//
//    doc.AddMember("imgPath",
//        rapidjson::Value(cardData.at("imgPath").asString().c_str(), allocator),
//        allocator);
//
//    doc.AddMember("attackRange", cardData.at("attackRange").asFloat(), allocator);
//    doc.AddMember("viewRange", cardData.at("viewRange").asFloat(), allocator);
//    doc.AddMember("health", cardData.at("health").asFloat(), allocator);
//    doc.AddMember("attackPower", cardData.at("attackPower").asFloat(), allocator);
//    doc.AddMember("attackSpeed", cardData.at("attackSpeed").asFloat(), allocator);
//    doc.AddMember("moveSpeed", cardData.at("moveSpeed").asFloat(), allocator);
//
//    doc.AddMember("level", cardData.at("level").asInt(), allocator);
//    doc.AddMember("numberOfCard", cardData.at("numberOfCard").asInt(), allocator);
//
//    doc.AddMember("rarity",
//        rapidjson::Value(cardData.at("rarity").asString().c_str(), allocator),
//        allocator);
//
//    doc.AddMember("volume", cardData.at("volume").asFloat(), allocator);
//
//    // 3. 转成字符串
//    StringBuffer buffer;
//    Writer<StringBuffer> writer(buffer);
//    doc.Accept(writer);
//
//    // 4. 写入文件（可写目录）
//    std::string path =
//        FileUtils::getInstance()->getWritablePath() ;
//
//    FileUtils::getInstance()->writeStringToFile(buffer.GetString(), path);
//
//    CCLOG("卡牌数据已写入文件：%s", path.c_str());
//}