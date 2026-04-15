#include "utils/utils.h"

std::string ConnectorUtils::GenerateFileUniqueId(const std::string &filePath) 
{
    size_t hashValue = std::hash<std::string>{}(filePath);

    // convert size_t to string
    return std::to_string(hashValue);
}