#include "models/CardModel.h"

using namespace MyCardGame;

rapidjson::Value CardModel::toJson(rapidjson::Document::AllocatorType& allocator) const {
    rapidjson::Value obj(rapidjson::kObjectType);
    obj.AddMember("id", _id, allocator);
    obj.AddMember("suit", static_cast<int>(_suit), allocator);
    obj.AddMember("face", static_cast<int>(_face), allocator);
    return obj;
}

void CardModel::fromJson(const rapidjson::Value& value) {
    if (value.HasMember("id") && value["id"].IsInt()) {
        _id = value["id"].GetInt();
    }
    if (value.HasMember("suit") && value["suit"].IsInt()) {
        _suit = static_cast<CardSuitType>(value["suit"].GetInt());
    }
    if (value.HasMember("face") && value["face"].IsInt()) {
        _face = static_cast<CardFaceType>(value["face"].GetInt());
    }
}
