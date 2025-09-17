#pragma once

#include "configs/models/LevelConfig.h"
#include <string>

namespace MyCardGame {

    class LevelLoader {
    public:
        static LevelConfig loadLevelConfig(const std::string& filename);
    };

}
