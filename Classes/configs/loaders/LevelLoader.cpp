#include "configs/loaders/LevelLoader.h"
#include "cocos2d.h"
#include "json/rapidjson.h"
#include "json/document.h"

USING_NS_CC;
using namespace MyCardGame;

#include "configs/loaders/LevelLoader.h"
#include "utils/JsonUtils.h"

using namespace MyCardGame;

namespace {
    void parseCards(const rapidjson::Value& cardArray, std::vector<CardConfig>& cardConfigs) {
        for (rapidjson::SizeType i = 0; i < cardArray.Size(); i++) {
            const rapidjson::Value& cardData = cardArray[i];
            CardConfig card;
            card.face = static_cast<CardFaceType>(cardData["CardFace"].GetInt());
            card.suit = static_cast<CardSuitType>(cardData["CardSuit"].GetInt());
            card.position = cocos2d::Vec2(cardData["Position"]["x"].GetFloat(), cardData["Position"]["y"].GetFloat());
            cardConfigs.push_back(card);
        }
    }
}

std::unique_ptr<LevelConfig> LevelLoader::loadLevelConfig(const std::string& filename) {
    auto config = std::make_unique<LevelConfig>();
    rapidjson::Document doc;

    if (!Utils::loadJsonFromFile(filename, doc)) {
        // Log error or handle failure
        return nullptr;
    }

    if (doc.HasMember("Playfield") && doc["Playfield"].IsArray()) {
        parseCards(doc["Playfield"], config->playfieldCards);
    }

    if (doc.HasMember("Stack") && doc["Stack"].IsArray()) {
        parseCards(doc["Stack"], config->stackCards);
    }

    return config;
}
