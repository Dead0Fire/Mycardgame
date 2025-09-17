#include "services/SaveGameService.h"
#include "cocos2d.h"

namespace MyCardGame {

SaveGameService* SaveGameService::_instance = nullptr;

SaveGameService* SaveGameService::getInstance() {
    if (!_instance) {
        _instance = new SaveGameService();
    }
    return _instance;
}

std::string SaveGameService::getSaveFilePath() const {
    // 获取写入权限的目录路径
    std::string writablePath = cocos2d::FileUtils::getInstance()->getWritablePath();
    return writablePath + "savegame.json";
}

bool SaveGameService::saveGame(int levelId,
                             const std::vector<CardModel>& cards,
                             int score,
                             int undoSteps) {
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

    // 保存基本游戏状态
    document.AddMember("levelId", levelId, allocator);
    document.AddMember("score", score, allocator);
    document.AddMember("undoSteps", undoSteps, allocator);

    // 保存卡牌状态
    rapidjson::Value cardsArray(rapidjson::kArrayType);
    for (const auto& card : cards) {
        cardsArray.PushBack(card.toJson(allocator), allocator);
    }
    document.AddMember("cards", cardsArray, allocator);

    // 将 JSON 转换为字符串
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);

    // 写入文件
    return cocos2d::FileUtils::getInstance()->writeStringToFile(
        buffer.GetString(),
        getSaveFilePath()
    );
}

bool SaveGameService::loadGame(int& levelId,
                             std::vector<CardModel>& cards,
                             int& score,
                             int& undoSteps) {
    // 读取存档文件
    std::string jsonStr = cocos2d::FileUtils::getInstance()->getStringFromFile(getSaveFilePath());
    if (jsonStr.empty()) {
        return false;
    }

    // 解析 JSON
    rapidjson::Document document;
    if (document.Parse(jsonStr.c_str()).HasParseError()) {
        return false;
    }

    // 加载基本游戏状态
    if (document.HasMember("levelId") && document["levelId"].IsInt()) {
        levelId = document["levelId"].GetInt();
    }
    if (document.HasMember("score") && document["score"].IsInt()) {
        score = document["score"].GetInt();
    }
    if (document.HasMember("undoSteps") && document["undoSteps"].IsInt()) {
        undoSteps = document["undoSteps"].GetInt();
    }

    // 加载卡牌状态
    cards.clear();
    if (document.HasMember("cards") && document["cards"].IsArray()) {
        const rapidjson::Value& cardsArray = document["cards"];
        for (rapidjson::SizeType i = 0; i < cardsArray.Size(); i++) {
            CardModel card(0, CardSuitType::CST_NONE, CardFaceType::CFT_NONE);
            card.fromJson(cardsArray[i]);
            cards.push_back(card);
        }
    }

    return true;
}

bool SaveGameService::hasSaveGame() const {
    return cocos2d::FileUtils::getInstance()->isFileExist(getSaveFilePath());
}

} // namespace MyCardGame