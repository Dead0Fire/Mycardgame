#pragma once

#include "cocos2d.h"
#include "models/CardModel.h"
#include "views/CardView.h"
#include "managers/UndoManager.h"
#include <vector>

namespace MyCardGame {

    class GameController : public cocos2d::Node {
    public:
        static GameController* create(cocos2d::Layer* mainCardArea, cocos2d::Layer* pileArea);
        virtual bool init(cocos2d::Layer* mainCardArea, cocos2d::Layer* pileArea);
        void startGame(int levelId);
        void undoLastMove();

    private:
        void onCardClicked(CardView* card);
        bool canMatch(CardModel* card1, CardModel* card2);
        void moveCard(CardView* card, const cocos2d::Vec2& position, std::function<void()> onComplete);

        cocos2d::Layer* _mainCardArea;
        cocos2d::Layer* _pileArea;

        std::vector<CardModel*> _playfieldCardModels;
        std::vector<CardModel*> _stackCardModels;
        CardModel* _currentPileCardModel;

        std::vector<CardView*> _playfieldCardViews;
        std::vector<CardView*> _stackCardViews;
        CardView* _currentPileCardView;

        UndoManager _undoManager;

        cocos2d::EventListenerTouchOneByOne* _touchListener;
    };

}
