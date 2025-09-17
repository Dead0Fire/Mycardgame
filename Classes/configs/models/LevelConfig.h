#pragma once

#include "models/CardModel.h"
#include "cocos2d.h"
#include <vector>

namespace MyCardGame {

    struct CardConfig {
        MyCardGame::CardFaceType face;
        MyCardGame::CardSuitType suit;
        cocos2d::Vec2 position;
    };

    struct LevelConfig {
        std::vector<CardConfig> playfieldCards;
        std::vector<CardConfig> stackCards;
    };
}
