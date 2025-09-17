#include "GameScene.h"

USING_NS_CC;

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

    return true;
}