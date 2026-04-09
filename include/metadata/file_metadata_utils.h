#ifndef FILE_METADATA_UTILS_H
#define FILE_METADATA_UTILS_H

#include "file_metadata.h"
#include "../file_details.h"
#include <vector>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>
#include <map>

namespace fs = std::filesystem;

class FileMetadataUtils
{
public:
    static std::map<std::string, FileMetadata> filepath_to_metadata;
    static bool IsModified(const FileDetails& file);
    static bool IsNew(const FileDetails& file);
    static std::string GetLastModifiedDateTime(const fs::path& p);
    static std::vector<FileDetails> GetListOfFilesToDownload(std::vector<FileDetails> available_files);
    static void UpdateFileMetadata(std::string filename, std::string last_backup_time, std::string dest_path);
};

#endif // FILE_METADATA_UTILS_H