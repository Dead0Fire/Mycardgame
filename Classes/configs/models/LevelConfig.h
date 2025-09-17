/**
 * @file LevelConfig.h
 * @brief Defines the data structures for level configuration.
 */

#pragma once

#include "models/CardModel.h"
#include "cocos2d.h"
#include <vector>
#include <memory>

namespace MyCardGame {

    /**
     * @struct CardConfig
     * @brief Holds the configuration for a single card in a level layout.
     *
     * This structure is used by the LevelLoader to temporarily store card
     * properties before the full CardModel objects are created.
     */
    struct CardConfig {
        MyCardGame::CardFaceType face; ///< The face value of the card.
        MyCardGame::CardSuitType suit; ///< The suit of the card.
        cocos2d::Vec2 position;        ///< The initial position of the card on the screen.
    };

    /**
     * @struct LevelConfig
     * @brief Contains all the configuration data for a single game level.
     *
     * This includes the layout of cards on the playfield and the cards
     * available in the initial stack.
     */
    struct LevelConfig {
        std::vector<CardConfig> playfieldCards; ///< Configuration for cards on the main playfield.
        std::vector<CardConfig> stackCards;     ///< Configuration for cards in the starting stack.
    };
}
