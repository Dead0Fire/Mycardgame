#include "views/CardView.h"
#include "models/CardModel.h"
#include "cocos2d.h"
#include "2d/CCSprite.h"
#include "math/Vec2.h"
using namespace MyCardGame;

CardView* CardView::create(CardModel* model) {
    CardView* sprite = new (std::nothrow) CardView();
    if (sprite && sprite->init(model)) {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

bool CardView::init(CardModel* model) {
    if (!cocos2d::Sprite::initWithFile("res/card_general.png")) {
        return false;
    }

    _model = model;
    setupCardDisplay();
    return true;
}

void CardView::setupCardDisplay() {
    cocos2d::Size cardSize = this->getContentSize();

    // 左上角小数字
    _topLeftSprite = cocos2d::Sprite::create(getSmallNumberImagePath());
    if (_topLeftSprite) {
        _topLeftSprite->setPosition(cocos2d::Vec2(cardSize.width * 0.15f, cardSize.height * 0.85f));
        _topLeftSprite->setScale(0.8f); // 稍微缩小
        this->addChild(_topLeftSprite);
    }

    // 右上角花色
    _topRightSprite = cocos2d::Sprite::create(getSuitImagePath());
    if (_topRightSprite) {
        _topRightSprite->setPosition(cocos2d::Vec2(cardSize.width * 0.85f, cardSize.height * 0.85f));
        _topRightSprite->setScale(0.6f); // 稍微缩小
        this->addChild(_topRightSprite);
    }

    // 中间大数字
    _centerSprite = cocos2d::Sprite::create(getBigNumberImagePath());
    if (_centerSprite) {
        _centerSprite->setPosition(cocos2d::Vec2(cardSize.width * 0.5f, cardSize.height * 0.5f));
        this->addChild(_centerSprite);
    }
}

std::string CardView::getBigNumberImagePath() const {
    std::string color = (_model->getSuit() == CardSuitType::CST_HEARTS || 
                        _model->getSuit() == CardSuitType::CST_DIAMONDS) ? "red" : "black";
    std::string number = getNumberString();
    return "res/number/big_" + color + "_" + number + ".png";
}

std::string CardView::getSmallNumberImagePath() const {
    std::string color = (_model->getSuit() == CardSuitType::CST_HEARTS || 
                        _model->getSuit() == CardSuitType::CST_DIAMONDS) ? "red" : "black";
    std::string number = getNumberString();
    return "res/number/small_" + color + "_" + number + ".png";
}

std::string CardView::getSuitImagePath() const {
    switch (_model->getSuit()) {
        case CardSuitType::CST_HEARTS: return "res/suits/heart.png";
        case CardSuitType::CST_DIAMONDS: return "res/suits/diamond.png";
        case CardSuitType::CST_CLUBS: return "res/suits/club.png";
        case CardSuitType::CST_SPADES: return "res/suits/spade.png";
        default: return "res/suits/diamond.png";
    }
}

std::string CardView::getNumberString() const {
    switch (_model->getFace()) {
        case CardFaceType::CFT_ACE: return "A";
        case CardFaceType::CFT_JACK: return "J";
        case CardFaceType::CFT_QUEEN: return "Q";
        case CardFaceType::CFT_KING: return "K";
        default:
            return std::to_string(static_cast<int>(_model->getFace()) + 1);
    }
}

int CardView::getCardId() const {
    return _model->getId();
}
