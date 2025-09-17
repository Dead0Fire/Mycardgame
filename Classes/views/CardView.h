#pragma once

#include "cocos2d.h"
#include "models/CardModel.h"

namespace MyCardGame {

/**
 * @class CardView
 * @brief Represents the visual representation of a single card in the game.
 *
 * The CardView is a Sprite that displays the card's suit and face value.
 * It holds a pointer to a CardModel to get the data it needs to render.
 * This class is part of the View layer and is responsible for display only.
 */
class CardView : public cocos2d::Sprite {
public:
    /**
     * @brief Creates a new CardView instance.
     * @param model A pointer to the CardModel that this view represents.
     * @return A pointer to the created CardView instance.
     */
    static CardView* create(const CardModel* model);

    /**
     * @brief Initializes the CardView with a CardModel.
     * @param model A pointer to the CardModel.
     * @return True if initialization is successful, false otherwise.
     */
    bool init(const CardModel* model);

    /**
     * @brief Gets the unique ID of the card.
     * @return The integer ID of the card.
     */
    int getCardId() const;

    /**
     * @brief Gets the underlying data model for this view.
     * @return A const pointer to the CardModel.
     */
    const CardModel* getModel() const { return _model; }

private:
    /**
     * @brief Sets up the visual elements of the card (suit, number, etc.).
     */
    void setupCardDisplay();

    /**
     * @brief Gets the string representation of the card's face value (e.g., "A", "K", "10").
     * @return A string for the card's number.
     */
    std::string getNumberString() const;

    /**
     * @brief Gets the file path for the large number image in the center of the card.
     * @return A string containing the image path.
     */
    std::string getBigNumberImagePath() const;

    /**
     * @brief Gets the file path for the small number image in the corner of the card.
     * @return A string containing the image path.
     */
    std::string getSmallNumberImagePath() const;

    /**
     * @brief Gets the file path for the suit image (heart, spade, etc.).
     * @return A string containing the image path.
     */
    std::string getSuitImagePath() const;

    const CardModel* _model; ///< The data model for this card. Is const to enforce view-only access.
    cocos2d::Sprite* _topLeftSprite;     ///< Sprite for the small number in the top-left corner.
    cocos2d::Sprite* _centerSprite;      ///< Sprite for the large number in the center.
    cocos2d::Sprite* _topRightSprite;    ///< Sprite for the suit in the top-right corner.
};}
