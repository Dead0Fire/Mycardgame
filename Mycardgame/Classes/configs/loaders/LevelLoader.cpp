#include "configs/loaders/LevelLoader.h"
#include "cocos2d.h"
#include "json/rapidjson.h"
#include "json/document.h"

USING_NS_CC;
using namespace MyCardGame;

LevelConfig LevelLoader::loadLevelConfig(const std::string& filename) {
    LevelConfig config;
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(filename);
    std::string content = FileUtils::getInstance()->getStringFromFile(fullPath);

    rapidjson::Document doc;
    doc.Parse(content.c_str());

    if (doc.HasMember("Playfield")) {
        const rapidjson::Value& playfield = doc["Playfield"];
        for (rapidjson::SizeType i = 0; i < playfield.Size(); i++) {
            const rapidjson::Value& cardData = playfield[i];
            CardConfig card;
            card.face = static_cast<CardFaceType>(cardData["CardFace"].GetInt());
            card.suit = static_cast<CardSuitType>(cardData["CardSuit"].GetInt());
            card.position = Vec2(cardData["Position"]["x"].GetFloat(), cardData["Position"]["y"].GetFloat());
            config.playfieldCards.push_back(card);
        }
    }

    if (doc.HasMember("Stack")) {
        const rapidjson::Value& stack = doc["Stack"];
        for (rapidjson::SizeType i = 0; i < stack.Size(); i++) {
            const rapidjson::Value& cardData = stack[i];
            CardConfig card;
            card.face = static_cast<CardFaceType>(cardData["CardFace"].GetInt());
            card.suit = static_cast<CardSuitType>(cardData["CardSuit"].GetInt());
            card.position = Vec2(cardData["Position"]["x"].GetFloat(), cardData["Position"]["y"].GetFloat());
            config.stackCards.push_back(card);
        }
    }

    return config;
}
