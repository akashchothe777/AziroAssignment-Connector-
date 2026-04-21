#ifndef FILE_METADATA_UTILS_H
#define FILE_METADATA_UTILS_H

#include "file_metadata.h"
#include "metadata/file_metadata.h"
#include <vector>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>
#include <map>
#include <unordered_map>

namespace fs = std::filesystem;

class FileMetadataUtils
{
public:
    static std::unordered_map<std::string, FileMetadata> file_id_to_metadata;
    static bool IsModified(const FileMetadata& file);
    static bool IsNew(const FileMetadata& file);
    static std::string GetLastModifiedDateTime(const fs::path& p);
    static std::vector<FileMetadata> GetListOfFilesToDownload(std::vector<FileMetadata> available_files);
    static void UpdateFileMetadataForUpload(std::string file_id, fs::path dest_file_path);
    static void UpdateLocalDownloadInfo(std::string file_id, std::string local_filepath);
    static FileMetadata GetFileMetadataOfLocalFile(std::string local_filename);
    static std::string GenerateFileUniqueId(const std::string &filePath);
    static std::string GetCurrentTimeString();
    static bool IsCompletelyDownloaded(std::string file_id);
};

#endif // FILE_METADATA_UTILS_H