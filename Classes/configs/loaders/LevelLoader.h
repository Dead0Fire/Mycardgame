#pragma once

#include "configs/models/LevelConfig.h"
#include <string>

namespace MyCardGame {

/**
 * @class LevelLoader
 * @brief A utility class for loading level configurations from a file.
 *
 * This class provides a static method to parse a JSON file and create a
 * LevelConfig object. It is responsible for reading the file and mapping
 * the JSON data to the data structures defined in LevelConfig.
 */
class LevelLoader {
public:
    /**
     * @brief Loads a level configuration from a JSON file.
     * @param filename The name of the JSON file to load.
     * @return A unique_ptr to the loaded LevelConfig object.
     */
    static std::unique_ptr<LevelConfig> loadLevelConfig(const std::string& filename);
};}
