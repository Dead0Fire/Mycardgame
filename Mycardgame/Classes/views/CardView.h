#pragma once

#include "cocos2d.h"
#include "models/CardModel.h"

namespace MyCardGame {

    class CardView : public cocos2d::Sprite {
    public:
        static CardView* create(CardModel* model);
        bool init(CardModel* model);
        int getCardId() const;
        CardModel* getModel() const { return _model; }

    private:
        void setupCardDisplay();
        std::string getNumberString() const;
        std::string getSuitSymbol() const;
        std::string getBigNumberImagePath() const;
        std::string getSmallNumberImagePath() const;
        std::string getSuitImagePath() const;

        CardModel* _model;
        cocos2d::Sprite* _topLeftSprite;     // 左上角小数字
        cocos2d::Sprite* _centerSprite;      // 中间大数字
        cocos2d::Sprite* _topRightSprite;    // 右上角花色
    };

}
