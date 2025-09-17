#include "utils/JsonUtils.h"
#include "cocos2d.h"

USING_NS_CC;
using namespace MyCardGame::Utils;

bool MyCardGame::Utils::parseJsonString(const std::string& jsonString, rapidjson::Document& doc) {
    doc.Parse(jsonString.c_str());
    return !doc.HasParseError();
}

bool MyCardGame::Utils::loadJsonFromFile(const std::string& filename, rapidjson::Document& doc) {
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(filename);
    std::string content = FileUtils::getInstance()->getStringFromFile(fullPath);
    if (content.empty()) {
        return false;
    }
    return parseJsonString(content, doc);
}
