/**
 * @file CardModel.h
 * @brief Defines the data model for a single card.
 */

#pragma once

#include "json/rapidjson.h"
#include "json/document.h"

namespace MyCardGame {

    /**
     * @enum CardSuitType
     * @brief Represents the suit of a card.
     */
    enum class CardSuitType {
        CST_NONE = -1,
        CST_CLUBS,      ///< 梅花
        CST_DIAMONDS,   ///< 方块
        CST_HEARTS,     ///< 红桃
        CST_SPADES,     ///< 黑桃
        CST_NUM_CARD_SUIT_TYPES
    };

    /**
     * @enum CardFaceType
     * @brief Represents the face value of a card.
     */
    enum class CardFaceType {
        CFT_NONE = -1,
        CFT_ACE,
        CFT_TWO,
        CFT_THREE,
        CFT_FOUR,
        CFT_FIVE,
        CFT_SIX,
        CFT_SEVEN,
        CFT_EIGHT,
        CFT_NINE,
        CFT_TEN,
        CFT_JACK,
        CFT_QUEEN,
        CFT_KING,
        CFT_NUM_CARD_FACE_TYPES
    };

    /**
     * @class CardModel
     * @brief Represents the data for a single card.
     *
     * This class stores the essential properties of a card, such as its ID,
     * suit, and face value. It is a pure data class with no business logic.
     * It supports serialization and deserialization to/from JSON.
     */
    class CardModel {
    public:
        /**
         * @brief Constructs a CardModel.
         * @param id The unique identifier for the card.
         * @param suit The suit of the card.
         * @param face The face value of the card.
         */
        CardModel(int id, CardSuitType suit, CardFaceType face) : _id(id), _suit(suit), _face(face) {}

        // Getters
        int getId() const { return _id; }
        CardSuitType getSuit() const { return _suit; }
        CardFaceType getFace() const { return _face; }
        int getFaceValue() const { return static_cast<int>(_face); }

        /**
         * @brief Serializes the card data to a JSON value.
         * @param allocator The allocator for the JSON document.
         * @return The JSON value representing the card.
         */
        rapidjson::Value toJson(rapidjson::Document::AllocatorType& allocator) const;

        /**
         * @brief Deserializes card data from a JSON value.
         * @param value The JSON value to deserialize from.
         */
        void fromJson(const rapidjson::Value& value);

    private:
        int _id;             ///< Unique ID for the card.
        CardSuitType _suit;  ///< The suit of the card.
        CardFaceType _face;  ///< The face value of the card.
    };

}
