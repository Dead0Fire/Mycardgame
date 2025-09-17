#pragma once

#include "cocos2d.h"

class GameScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(GameScene);

private:
    cocos2d::LayerColor* _mainCardArea; // LayerColor，以支持背景颜色
    cocos2d::LayerColor* _pileArea; 
};