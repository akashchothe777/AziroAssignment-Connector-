#ifndef JSON_HANDLER_H
#define JSON_HANDLER_H

#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include "nlohmann/json.hpp"
#include "file_metadata.h"

using json = nlohmann::json;

class JsonHandler
{
    std::string json_file_name;
    
public:
    JsonHandler(std::string filename) : json_file_name(filename){};

    /** Serialize to JSON string. */
    std::string ToJsonString(std::unordered_map<std::string, FileMetadata>& items_, bool pretty = true) const;

    /** Save to file. Throws std::runtime_error on failure. */
    void SaveToFile(std::unordered_map<std::string, FileMetadata>& items_, bool pretty = true) const;

    /** Load from file. Throws std::runtime_error on failure or parse error. */
    void LoadFromFile(std::unordered_map<std::string, FileMetadata>& items_);
};

#endif
