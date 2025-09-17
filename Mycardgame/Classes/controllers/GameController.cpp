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

    // Clean up previous game state
    for (auto card : _playfieldCardModels) delete card;
    _playfieldCardModels.clear();
    for (auto card : _stackCardModels) delete card;
    _stackCardModels.clear();
    _mainCardArea->removeAllChildren();
    _pileArea->removeAllChildren();
    int cardId = 0;

    // 自动分堆：每个堆是重叠的牌
    _playfieldPiles.clear();
    const float overlapThreshold = 0.0f; // 重叠面积大于0即算同堆
    std::vector<CardView*> allViews;
    for (const auto& cardConfig : levelConfig.playfieldCards) {
        auto model = new CardModel(cardId++, cardConfig.suit, cardConfig.face);
        _playfieldCardModels.push_back(model);
        auto view = CardView::create(model);
        view->setPosition(cardConfig.position);
        _mainCardArea->addChild(view);
        allViews.push_back(view);
    }
    // 分堆算法：遍历所有牌，按重叠分组
    for (auto* card : allViews) {
        cocos2d::Rect rectA = card->getBoundingBox();
        bool added = false;
        for (auto& pile : _playfieldPiles) {
            for (auto* other : pile) {
                cocos2d::Rect rectB = other->getBoundingBox();
                // 计算交集面积
                float x1 = std::max(rectA.getMinX(), rectB.getMinX());
                float y1 = std::max(rectA.getMinY(), rectB.getMinY());
                float x2 = std::min(rectA.getMaxX(), rectB.getMaxX());
                float y2 = std::min(rectA.getMaxY(), rectB.getMaxY());
                float interArea = (x2 > x1 && y2 > y1) ? (x2 - x1) * (y2 - y1) : 0;
                float minArea = std::min(rectA.size.width * rectA.size.height, rectB.size.width * rectB.size.height);
                if (minArea > 0 && interArea > 0) {
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
    // 给每个堆的牌添加点击事件
    for (auto& pile : _playfieldPiles) {
        for (auto* card : pile) {
            _eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener->clone(), card);
        }
    }

    // Create stack cards (备用牌堆)
    float stackX = 200;  // 起始位置进一步向右移
    float stackY = _pileArea->getContentSize().height * 0.5f;
    

    // 堆叠显示所有备用牌，坐标依次相差一点
    float offsetX = 70.0f; // 每张牌向右偏移80像素，明显错开
    float offsetY = 0.0f;  // y坐标不变
    float offsetScale = 0.0f; // 不缩放，保持一致
    
    for (size_t i = 0; i < levelConfig.stackCards.size(); ++i) {
        const auto& cardConfig = levelConfig.stackCards[i];
        auto model = new CardModel(cardId++, cardConfig.suit, cardConfig.face);
        _stackCardModels.push_back(model);
        
        auto view = CardView::create(model);
        // 坐标依次相差一点，制造堆叠效果
        view->setPosition(Vec2(stackX + i * offsetX, stackY + i * offsetY));
        view->setLocalZOrder(i); // 确保后面的牌显示在上层
        view->setOpacity(255); // 完全不透明
        view->setVisible(true); // 所有牌都显示
        _pileArea->addChild(view);
        _stackCardViews.push_back(view);
        
        // 添加点击事件监听
        _eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener->clone(), view);
    }

    // Set initial pile card
    if (!_stackCardModels.empty()) {
        _currentPileCardModel = _stackCardModels.back();
        _currentPileCardView = _stackCardViews.back();
        _currentPileCardView->setVisible(true);
        _currentPileCardView->setPosition(Vec2(_pileArea->getContentSize().width / 2 + 80, _pileArea->getContentSize().height / 2));
    }
}

void GameController::onCardClicked(CardView* card) {
    CardModel* cardModel = card->getModel();
    if (!cardModel) return;

    // 只允许每个堆的顶部牌与手牌区顶部牌进行匹配
    for (auto& pile : _playfieldPiles) {
        if (!pile.empty() && card == pile.back()) {
            if (_currentPileCardModel && canMatch(cardModel, _currentPileCardModel)) {
                // Capture state for undo
                auto oldPileModel = _currentPileCardModel;
                auto oldPileView = _currentPileCardView;
                auto fromPosition = card->getPosition();
                auto fromParent = card->getParent();

                UndoCommand command;
                command.undo = [this, card, oldPileModel, oldPileView, fromPosition, fromParent, &pile]() {
                    // Restore the previous pile card
                    if (oldPileView) {
                        oldPileView->setVisible(true);
                    }
                    _currentPileCardModel = oldPileModel;
                    _currentPileCardView = oldPileView;

                    // Move the card back to其原堆
                    card->retain();
                    card->removeFromParent();
                    fromParent->addChild(card);
                    card->release();
                    auto moveTo = MoveTo::create(0.2f, fromPosition);
                    card->runAction(moveTo);
                    pile.push_back(card);
                };
                _undoManager.addAction(std::move(command));

                Vec2 targetPos = _currentPileCardView->getPosition();
                moveCard(card, targetPos, [this, card, cardModel, &pile]() {
                    if (_currentPileCardView) {
                        _currentPileCardView->setVisible(false); 
                    }
                    _currentPileCardModel = cardModel;
                    _currentPileCardView = card;
                    // 只移除该堆顶部牌
                    if (!pile.empty()) {
                        pile.pop_back();
                    }
                    card->retain();
                    card->removeFromParent();
                    _pileArea->addChild(card);
                    card->release();
                });
            }
            return;
        }
    }

    // Check if it's a card from the stack pile being clicked
    auto it_stack = std::find(_stackCardViews.begin(), _stackCardViews.end(), card);
    if (it_stack != _stackCardViews.end() && card != _currentPileCardView) {
        // Capture state for undo
        auto oldPileModel = _currentPileCardModel;
        auto oldPileView = _currentPileCardView;
        auto fromPosition = card->getPosition();
        auto fromScale = card->getScale();
        auto fromZOrder = card->getLocalZOrder();

        UndoCommand command;
        command.undo = [this, card, oldPileModel, oldPileView, fromPosition, fromScale, fromZOrder]() {
            // Restore the previous pile card
            if (oldPileView) {
                oldPileView->setVisible(true);
            }
            _currentPileCardModel = oldPileModel;
            _currentPileCardView = oldPileView;

            // Move the card back to its original state in the stack
            auto moveTo = MoveTo::create(0.2f, fromPosition);
            auto scaleTo = ScaleTo::create(0.2f, fromScale);
            card->runAction(Spawn::create(moveTo, scaleTo, nullptr));
            card->setLocalZOrder(fromZOrder);
        };
        _undoManager.addAction(std::move(command));

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
