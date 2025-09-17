#include "GameScene.h"
#include "controllers/GameController.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace MyCardGame;

Scene* GameScene::createScene() {
    return GameScene::create();
}

bool GameScene::init() {
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 主牌区：顶部 1500 高度
    _mainCardArea = LayerColor::create(Color4B(0, 128, 0, 255));
    _mainCardArea->setContentSize(Size(1080, 1500));
    _mainCardArea->setPosition(Vec2(origin.x, origin.y + 580));
    this->addChild(_mainCardArea);

    // 堆牌区：底部 580 高度
    _pileArea = LayerColor::create(Color4B(128, 0, 0, 255));
    _pileArea->setContentSize(Size(1080, 580));
    _pileArea->setPosition(Vec2(origin.x, origin.y));
    this->addChild(_pileArea);

    _gameController = GameController::create(_mainCardArea, _pileArea);
    this->addChild(_gameController);
    _gameController->startGame(1); // Start level 1

    // Add an undo button at the center bottom of the pile area
    auto undoButton = ui::Button::create("CloseNormal.png", "CloseSelected.png");
    undoButton->setPosition(Vec2(_pileArea->getContentSize().width * 0.8f, _pileArea->getContentSize().height * 0.5f));
    undoButton->setTitleText("Undo");
    undoButton->setTitleFontSize(24);
    undoButton->addClickEventListener([this](Ref* sender) {
        _gameController->undoLastMove();
    });
    _pileArea->addChild(undoButton);

    return true;
}