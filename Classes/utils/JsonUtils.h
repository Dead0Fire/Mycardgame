/**
 * @file JsonUtils.h
 * @brief Provides utility functions for working with RapidJSON.
 */

#pragma once

#include "json/rapidjson.h"
#include "json/document.h"
#include <string>
#include "cocos2d.h"

namespace MyCardGame {
namespace Utils {

    /**
     * @brief Parses a JSON string into a RapidJSON document.
     * @param jsonString The string containing the JSON data.
     * @param doc The document to parse into.
     * @return True if parsing is successful, false otherwise.
     */
    bool parseJsonString(const std::string& jsonString, rapidjson::Document& doc);

    /**
     * @brief Reads a file and parses its content as JSON.
     * @param filename The name of the file to read.
     * @param doc The document to parse into.
     * @return True if the file is read and parsed successfully, false otherwise.
     */
    bool loadJsonFromFile(const std::string& filename, rapidjson::Document& doc);

} // namespace Utils
} // namespace MyCardGame
