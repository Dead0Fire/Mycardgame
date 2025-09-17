#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "cocos2d.h"
#include "models/CardModel.h"
#include "views/CardView.h"
#include "managers/UndoManager.h"
#include "configs/models/LevelConfig.h"
#include "services/SaveGameService.h"
#include <vector>
#include <functional>

namespace MyCardGame {

/**
 * @class GameController
 * @brief Manages the main game logic, user interactions, and the overall game state.
 *
 * The GameController is responsible for:
 * - Initializing and starting a new game based on a level configuration.
 * - Handling user input, such as clicking on cards.
 * - Validating and executing game moves (e.g., matching cards).
 * - Coordinating updates between the game models (CardModel) and views (CardView).
 * - Managing the game's undo functionality.
 */
class GameController : public cocos2d::Node {
public:
    /**
     * @brief Creates a new instance of the GameController.
     * @param mainCardArea The layer where the playfield cards are displayed.
     * @param pileArea The layer where the pile card is displayed.
     * @return A pointer to the created GameController instance.
     */
    static GameController* create(cocos2d::Layer* mainCardArea, cocos2d::Layer* pileArea);

    /**
     * @brief Initializes the GameController with the necessary layers.
     * @param mainCardArea The layer for playfield cards.
     * @param pileArea The layer for the pile card.
     * @return True if initialization is successful, false otherwise.
     */
    virtual bool init(cocos2d::Layer* mainCardArea, cocos2d::Layer* pileArea);

    /**
     * @brief Starts a new game for a specific level.
     * @param levelId The ID of the level to load.
     */
    void startGame(int levelId);

    /**
     * @brief Undoes the last move made by the player.
     */
    void undoLastMove();

    /**
     * @brief 保存当前游戏状态
     * @return 是否保存成功
     */
    bool saveGameState();

    /**
     * @brief 加载游戏存档
     * @return 是否加载成功
     */
    bool loadGameState();

    /**
     * @brief 检查是否有可用的存档
     * @return 是否存在存档
     */
    bool hasSaveGame() const;

private:
    /**
     * @brief Handles the logic when a card is clicked by the user.
     * @param card The CardView that was clicked.
     */
    void onCardClicked(CardView* card);

    /**
     * @brief Checks if two cards can be matched.
     * @param card1 The first card model.
     * @param card2 The second card model.
     * @return True if the cards can be matched, false otherwise.
     */
    bool canMatch(CardModel* card1, CardModel* card2);

    /**
     * @brief Moves a card view to a specified position with an animation.
     * @param card The card view to move.
     * @param position The target position.
     * @param onComplete A callback function to execute after the move is complete.
     */
    void moveCard(CardView* card, const cocos2d::Vec2& position, std::function<void()> onComplete);

    /**
     * @brief Cleans up the state of the previous game.
     */
    void cleanupPreviousState();

    /**
     * @brief Sets up the playfield cards based on the level configuration.
     * @param levelConfig The configuration for the current level.
     * @return A vector of all created card views.
     */
    std::vector<CardView*> setupPlayfieldCards(const LevelConfig& levelConfig);

    /**
     * @brief Groups card views into piles based on their overlapping areas.
     * @param allViews A vector of all card views on the playfield.
     */
    void groupOverlappingCards(const std::vector<CardView*>& allViews);

    /**
     * @brief Sets up the stack cards based on the level configuration.
     * @param levelConfig The configuration for the current level.
     */
    void setupStackCards(const LevelConfig& levelConfig);

    /**
     * @brief Sets the initial card for the pile from the stack.
     */
    void setInitialPileCard();

    /**
     * @brief Handles a click on a card from the playfield.
     * @param card The card view that was clicked.
     */
    void handlePlayfieldCardClick(CardView* card);

    /**
     * @brief Handles a click on a card from the stack.
     * @param card The card view that was clicked.
     */
    void handleStackCardClick(CardView* card);

    // Member Variables

    cocos2d::Layer* _mainCardArea;        ///< Layer containing the interactive cards on the playfield.
    cocos2d::Layer* _pileArea;            ///< Layer for the card pile where matched cards are moved.

    std::vector<CardModel*> _playfieldCardModels; ///< Models for the cards currently on the playfield.
    std::vector<CardModel*> _stackCardModels;     ///< Models for the cards in the stack (not currently used).
    CardModel* _currentPileCardModel;             ///< Model for the card at the top of the pile.

    std::vector<std::vector<CardView*>> _playfieldPiles; ///< Groups of overlapping card views on the playfield.
    std::vector<CardView*> _stackCardViews;       ///< Views for the cards in the stack (not currently used).
    CardView* _currentPileCardView;               ///< View for the card at the top of the pile.

    UndoManager _undoManager;                     ///< Manages the undo history and operations.

    cocos2d::EventListenerTouchOneByOne* _touchListener; ///< Listener for touch events on cards.

    // 新增存档相关的私有方法
    int getCurrentScore() const;
    int getUndoSteps() const;
    void setGameState(int levelId, const std::vector<CardModel>& cards, int score, int undoSteps);

    // 新增成员变量
    int _currentLevelId{0};  ///< 当前关卡ID
    int _currentScore{0};    ///< 当前得分
};}

#endif // GAME_CONTROLLER_H
