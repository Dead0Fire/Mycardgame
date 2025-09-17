#pragma once

namespace MyCardGame {

    enum class CardSuitType
    {
        CST_NONE = -1,
        CST_CLUBS,      // 梅花
        CST_DIAMONDS,   // 方块
        CST_HEARTS,     // 红桃
        CST_SPADES,     // 黑桃
        CST_NUM_CARD_SUIT_TYPES
    };

    enum class CardFaceType
    {
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

    class CardModel {
    public:
        CardModel(int id, CardSuitType suit, CardFaceType face) : _id(id), _suit(suit), _face(face) {}

        int getId() const { return _id; }
        CardSuitType getSuit() const { return _suit; }
        CardFaceType getFace() const { return _face; }
        int getFaceValue() const { return static_cast<int>(_face); }

    private:
        int _id;
        CardSuitType _suit;
        CardFaceType _face;
    };

}
