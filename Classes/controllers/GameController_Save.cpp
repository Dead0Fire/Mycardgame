#include "GameController.h"

namespace MyCardGame {

bool GameController::saveGameState() {
    // 收集所有需要保存的卡牌数据
    std::vector<CardModel> allCards;

    // 保存当前在牌堆中的卡牌
    if (_currentPileCardModel) {
        allCards.push_back(*_currentPileCardModel);
    }

    // 保存场上所有卡牌
    for (const auto* model : _playfieldCardModels) {
        if (model) {
            allCards.push_back(*model);
        }
    }

    // 保存牌堆中的卡牌
    for (const auto* model : _stackCardModels) {
        if (model) {
            allCards.push_back(*model);
        }
    }

    // 调用存档服务保存游戏状态
    return SaveGameService::getInstance()->saveGame(
        _currentLevelId,
        allCards,
        getCurrentScore(),
        getUndoSteps()
    );
}

bool GameController::loadGameState() {
    int levelId = 0;
    std::vector<CardModel> cards;
    int score = 0;
    int undoSteps = 0;

    // 尝试加载存档
    if (!SaveGameService::getInstance()->loadGame(levelId, cards, score, undoSteps)) {
        return false;
    }

    // 清理当前游戏状态
    cleanupPreviousState();

    // 设置新的游戏状态
    setGameState(levelId, cards, score, undoSteps);

    return true;
}

bool GameController::hasSaveGame() const {
    return SaveGameService::getInstance()->hasSaveGame();
}

int GameController::getCurrentScore() const {
    return _currentScore;
}

int GameController::getUndoSteps() const {
    return _undoManager.getUndoCount();
}

void GameController::setGameState(int levelId, const std::vector<CardModel>& cards,
                                int score, int undoSteps) {
    _currentLevelId = levelId;
    _currentScore = score;

    // 根据加载的卡牌数据重新创建游戏状态
    // 这里需要根据具体的游戏规则来分配卡牌到不同的区域
    for (const auto& card : cards) {
        // 创建新的卡牌模型
        auto* model = new CardModel(card.getId(), card.getSuit(), card.getFace());
        
        // 根据卡牌ID或其他规则决定将卡牌放在哪个区域
        // 这里需要根据具体游戏逻辑来实现
        // ...

        // 创建对应的视图
        auto* view = CardView::create(model);
        if (view) {
            // 设置卡牌位置和其他属性
            // ...
        }
    }

    // 如果需要，恢复撤销管理器状态
    // _undoManager...
}

} // namespace MyCardGame