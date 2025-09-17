#include "controllers/GameController.h"
#include "configs/loaders/LevelLoader.h"
#include "views/CardView.h"

USING_NS_CC;
using namespace MyCardGame;

GameController* GameController::create(Layer* mainCardArea, Layer* pileArea) {
    GameController* pRet = new (std::nothrow) GameController();
    if (pRet && pRet->init(mainCardArea, pileArea)) {
        pRet->autorelease();
        return pRet;
    }
    else {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool GameController::init(Layer* mainCardArea, Layer* pileArea) {
    if (!Node::init()) {
        return false;
    }
    _mainCardArea = mainCardArea;
    _pileArea = pileArea;
    _currentPileCardModel = nullptr;
    _currentPileCardView = nullptr;

    _touchListener = EventListenerTouchOneByOne::create();
    _touchListener->setSwallowTouches(true);
    _touchListener->onTouchBegan = [this](Touch* touch, Event* event) {
        auto target = static_cast<CardView*>(event->getCurrentTarget());
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        Size s = target->getContentSize();
        Rect rect = Rect(0, 0, s.width, s.height);

        if (rect.containsPoint(locationInNode)) {
            onCardClicked(target);
            return true;
        }
        return false;
    };
    return true;
}

void GameController::startGame(int levelId) {
    auto levelConfig = LevelLoader::loadLevelConfig("level.json");

    cleanupPreviousState();

    auto allViews = setupPlayfieldCards(*levelConfig);
    groupOverlappingCards(allViews);

    // Add touch listeners to each card in the piles
    for (auto& pile : _playfieldPiles) {
        for (auto* card : pile) {
            _eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener->clone(), card);
        }
    }

    setupStackCards(*levelConfig);
    setInitialPileCard();
}

void GameController::cleanupPreviousState() {
    // Clean up models
    for (auto card : _playfieldCardModels) delete card;
    _playfieldCardModels.clear();
    for (auto card : _stackCardModels) delete card;
    _stackCardModels.clear();

    // Clean up views
    _mainCardArea->removeAllChildren();
    _pileArea->removeAllChildren();
    _playfieldPiles.clear();
    _stackCardViews.clear();

    _currentPileCardModel = nullptr;
    _currentPileCardView = nullptr;
}

std::vector<CardView*> GameController::setupPlayfieldCards(const LevelConfig& levelConfig) {
    std::vector<CardView*> allViews;
    int cardId = 0;
    for (const auto& cardConfig : levelConfig.playfieldCards) {
        auto model = new CardModel(cardId++, cardConfig.suit, cardConfig.face);
        _playfieldCardModels.push_back(model);
        auto view = CardView::create(model);
        view->setPosition(cardConfig.position);
        _mainCardArea->addChild(view);
        allViews.push_back(view);
    }
    return allViews;
}

void GameController::groupOverlappingCards(const std::vector<CardView*>& allViews) {
    for (auto* card : allViews) {
        cocos2d::Rect rectA = card->getBoundingBox();
        bool added = false;
        for (auto& pile : _playfieldPiles) {
            for (auto* other : pile) {
                cocos2d::Rect rectB = other->getBoundingBox();
                // Check for intersection
                if (rectA.intersectsRect(rectB)) {
                    pile.push_back(card);
                    added = true;
                    break;
                }
            }
            if (added) break;
        }
        if (!added) {
            _playfieldPiles.push_back({card});
        }
    }
}

void GameController::setupStackCards(const LevelConfig& levelConfig) {
    float stackX = 200;
    float stackY = _pileArea->getContentSize().height * 0.5f;
    float offsetX = 70.0f;
    int cardId = _playfieldCardModels.size();

    for (size_t i = 0; i < levelConfig.stackCards.size(); ++i) {
        const auto& cardConfig = levelConfig.stackCards[i];
        auto model = new CardModel(cardId++, cardConfig.suit, cardConfig.face);
        _stackCardModels.push_back(model);

        auto view = CardView::create(model);
        view->setPosition(Vec2(stackX + i * offsetX, stackY));
        view->setLocalZOrder(i);
        _pileArea->addChild(view);
        _stackCardViews.push_back(view);

        _eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener->clone(), view);
    }
}

void GameController::setInitialPileCard() {
    if (!_stackCardModels.empty()) {
        // 从备用牌堆中取出最后一张牌作为手牌
        _currentPileCardModel = _stackCardModels.back();
        _currentPileCardView = _stackCardViews.back();
        
        // 从备用牌堆中移除这张牌
        _stackCardModels.pop_back();
        _stackCardViews.pop_back();
        
        // 设置手牌的位置和显示
        _currentPileCardView->setVisible(true);
        _currentPileCardView->setPosition(Vec2(_pileArea->getContentSize().width / 2 + 80, _pileArea->getContentSize().height / 2));
        _currentPileCardView->setLocalZOrder(1000); // Ensure it's on top
    }
}

void GameController::onCardClicked(CardView* card) {
    if (!card->getModel()) return;

    // Check if the clicked card is a top card of a playfield pile
    for (auto& pile : _playfieldPiles) {
        if (!pile.empty() && card == pile.back()) {
            handlePlayfieldCardClick(card);
            return;
        }
    }

    // Check if the clicked card is from the stack
    auto it_stack = std::find(_stackCardViews.begin(), _stackCardViews.end(), card);
    if (it_stack != _stackCardViews.end()) {
        // 检查是否是备用牌堆的顶部牌
        size_t clickedIndex = std::distance(_stackCardViews.begin(), it_stack);
        if (clickedIndex == _stackCardViews.size() - 1) {  // 是备用牌堆的顶部牌
            handleStackCardClick(card);
        }
    }
}

void GameController::handlePlayfieldCardClick(CardView* card) {
    CardModel* cardModel = const_cast<CardModel*>(card->getModel());
    if (!_currentPileCardModel || !canMatch(cardModel, _currentPileCardModel)) {
        return;
    }

    // Find the pile this card belongs to
    auto pile_it = std::find_if(_playfieldPiles.begin(), _playfieldPiles.end(), 
        [card](const std::vector<CardView*>& p) {
            return !p.empty() && p.back() == card;
    });
    if (pile_it == _playfieldPiles.end()) return;

    // Capture state for undo
    auto oldPileModel = _currentPileCardModel;
    auto oldPileView = _currentPileCardView;
    auto fromPosition = card->getPosition();
    auto fromParent = card->getParent();

    UndoCommand command;
    command.undo = [this, card, oldPileModel, oldPileView, fromPosition, fromParent, pile_it]() {
        // 先设置手牌状态
        if (oldPileView) {
            oldPileView->setVisible(true);
        }
        _currentPileCardModel = oldPileModel;
        _currentPileCardView = oldPileView;

        // 使用动画将牌移回原位
        card->retain();
        card->removeFromParent();
        fromParent->addChild(card);
        card->release();

        moveCard(card, fromPosition, [this, card, pile_it]() {
            pile_it->push_back(card);  // 移动完成后将牌放回原来的牌堆
        });
    };
    _undoManager.addAction(std::move(command));

    // Execute move
    Vec2 targetPos = _currentPileCardView->getPosition();
    moveCard(card, targetPos, [this, card, cardModel, pile_it]() {
        if (_currentPileCardView) {
            _currentPileCardView->setVisible(false);
        }
        _currentPileCardModel = cardModel;
        _currentPileCardView = card;
        
        pile_it->pop_back();

        card->retain();
        card->removeFromParent();
        _pileArea->addChild(card);
        card->release();
        card->setLocalZOrder(1000);
    });
}

void GameController::handleStackCardClick(CardView* card) {
    CardModel* cardModel = const_cast<CardModel*>(card->getModel());

    // 保存当前状态用于撤销
    auto oldPileModel = _currentPileCardModel;
    auto oldPileView = _currentPileCardView;
    auto fromPosition = card->getPosition();
    auto fromZOrder = card->getLocalZOrder();

    // 找到点击的牌在备用牌堆中的位置
    auto it_model = std::find(_stackCardModels.begin(), _stackCardModels.end(), cardModel);
    auto it_view = std::find(_stackCardViews.begin(), _stackCardViews.end(), card);

    UndoCommand command;
    command.undo = [this, card, oldPileModel, oldPileView, fromPosition, fromZOrder, cardModel]() {
        // 先设置手牌状态
        if (oldPileView) {
            oldPileView->setVisible(true);
        }
        _currentPileCardModel = oldPileModel;
        _currentPileCardView = oldPileView;

        // 将牌放回备用牌堆
        _stackCardModels.push_back(cardModel);
        _stackCardViews.push_back(card);

        // 使用动画将牌移回原位
        moveCard(card, fromPosition, [this, card, fromZOrder]() {
            card->setLocalZOrder(fromZOrder);  // 移动完成后恢复原来的层级
        });
    };
    _undoManager.addAction(std::move(command));

    // 从备用牌堆中移除这张牌
    if (it_model != _stackCardModels.end()) {
        _stackCardModels.erase(it_model);
    }
    if (it_view != _stackCardViews.end()) {
        _stackCardViews.erase(it_view);
    }

    // Execute move
    Vec2 targetPos = Vec2(_pileArea->getContentSize().width / 2 + 80, _pileArea->getContentSize().height / 2);
    moveCard(card, targetPos, [this, card, cardModel]() {
        if (_currentPileCardView) {
            _currentPileCardView->setVisible(false);
        }
        _currentPileCardModel = cardModel;
        _currentPileCardView = card;
        card->setLocalZOrder(1000);
    });
}


void GameController::undoLastMove() {
    _undoManager.undo();
}

bool GameController::canMatch(CardModel* card1, CardModel* card2) {
    if (!card1 || !card2) return false;
    int diff = abs(card1->getFaceValue() - card2->getFaceValue());
    return diff == 1 || diff == 12; // Ace-King wrap-around
}

void GameController::moveCard(CardView* card, const Vec2& position, std::function<void()> onComplete) {
    auto moveTo = MoveTo::create(0.2f, position);
    auto callFunc = CallFunc::create(onComplete);
    card->runAction(Sequence::create(moveTo, callFunc, nullptr));
}
