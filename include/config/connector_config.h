#ifndef CONNECTOR_CONFIG_H
#define CONNECTOR_CONFIG_H

#include <string>

#include "../../third_party/nlohmann/json.hpp"
#include <fstream>

using json = nlohmann::json;

class ConnectorConfig 
{
private:
    json data;

public:
    explicit ConnectorConfig(const std::string& filename) 
    {
        std::ifstream file(filename);
        if (!file.is_open()) 
        {
            throw std::runtime_error("Cannot open config file");
        }
        file >> data;
    }

    template<typename T>
    T get(const std::string& key, const T& defaultVal) const 
    {
        if (data.contains(key)) 
        {
            return data[key].get<T>();
        }
        return defaultVal;
    }
};

#endif // CONNECTOR_CONFIG_H
