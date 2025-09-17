#pragma once

#include "cocos2d.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "models/CardModel.h"
#include <string>
#include <vector>

namespace MyCardGame {

/**
 * @class SaveGameService
 * @brief 处理游戏存档和读档功能的服务类
 */
class SaveGameService {
public:
    /**
     * @brief 获取单例实例
     */
    static SaveGameService* getInstance();

    /**
     * @brief 保存游戏状态
     * @param levelId 当前关卡ID
     * @param cards 当前卡牌状态
     * @param score 当前分数
     * @param undoSteps 可撤销的步骤数量
     * @return 是否保存成功
     */
    bool saveGame(int levelId, 
                 const std::vector<CardModel>& cards,
                 int score,
                 int undoSteps);

    /**
     * @brief 加载游戏存档
     * @param[out] levelId 加载的关卡ID
     * @param[out] cards 加载的卡牌状态
     * @param[out] score 加载的分数
     * @param[out] undoSteps 加载的可撤销步骤数量
     * @return 是否加载成功
     */
    bool loadGame(int& levelId,
                 std::vector<CardModel>& cards,
                 int& score,
                 int& undoSteps);

    /**
     * @brief 检查是否存在游戏存档
     * @return 是否存在存档
     */
    bool hasSaveGame() const;

private:
    SaveGameService() = default;
    ~SaveGameService() = default;

    /**
     * @brief 获取存档文件路径
     */
    std::string getSaveFilePath() const;

private:
    static SaveGameService* _instance;
};

} // namespace MyCardGame