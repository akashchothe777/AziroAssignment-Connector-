#include "metadata/json_handler.h"

// to_json/from_json for nlohmann::json
inline void to_json(json& j, const FileMetadata& m) 
{
    j = json{
        {"file_id", m.unique_id},
        {"source_path", m.source_path},
        {"local_path", m.local_path},
        {"file_name", m.name},
        {"modified_time", m.last_modified_time},
        {"file_size", m.size},
        {"backup_time", m.last_backup_time},
        {"destination_path", m.destination_path}
    };
}

inline void from_json(const json& j, FileMetadata& m) 
{
    j.at("file_id").get_to(m.unique_id);
    if (j.contains("source_path")) j.at("source_path").get_to(m.source_path);
    if (j.contains("local_path")) j.at("local_path").get_to(m.local_path);
    if (j.contains("file_name")) j.at("file_name").get_to(m.name);
    if (j.contains("modified_time")) j.at("modified_time").get_to(m.last_modified_time);
    if (j.contains("file_size")) j.at("file_size").get_to(m.size);
    if (j.contains("backup_time")) j.at("backup_time").get_to(m.last_backup_time);
    if (j.contains("destination_path")) j.at("destination_path").get_to(m.destination_path);
}

std::string JsonHandler::ToJsonString(std::unordered_map<std::string, FileMetadata>& items_, bool pretty) const 
{
    json j = items_;
    return pretty ? j.dump(2) : j.dump();
}

void JsonHandler::SaveToFile(std::unordered_map<std::string, FileMetadata>& items_, bool pretty) const 
{
    std::ofstream ofs(json_file_name, std::ios::binary);
    if (!ofs) throw std::runtime_error("Failed to open file for writing: " + json_file_name);
    ofs << ToJsonString(items_, pretty);
    if (!ofs) throw std::runtime_error("Failed to write JSON to file: " + json_file_name);
    std::cout << "Saved metadata to file: " << json_file_name << std::endl;
}

void JsonHandler::LoadFromFile(std::unordered_map<std::string, FileMetadata>& items_) 
{
    std::ifstream ifs(json_file_name, std::ios::binary);

    if (!ifs) throw std::runtime_error("Failed to open file for reading: " + json_file_name);
    json j;
    try 
    {
        ifs >> j;
    } 
    catch (const json::parse_error& e) 
    {
        throw std::runtime_error(std::string("JSON parse error: ") + e.what());
    }
    catch (const json::exception& e)
    {
        throw std::runtime_error(std::string("JSON error: ") + e.what());
    }

    if(j.is_null() || j.empty())
    {
        std::cout << "Error: metadata file is having no data" << std::endl;
    }
    else
    {
        items_.clear();
        items_ = j.get<std::unordered_map<std::string, FileMetadata>>();
    }
}