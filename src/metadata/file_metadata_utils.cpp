#include "metadata/file_metadata_utils.h"
#include <iostream>
#include <algorithm>

std::unordered_map<std::string, FileMetadata> FileMetadataUtils::file_id_to_metadata{};

bool FileMetadataUtils::IsModified(const FileMetadata& file)
{
    std::cout << "Info: In FileMetadataUtils::IsModified()" << std::endl;

    bool is_modified{false};

    auto itr = file_id_to_metadata.find(file.unique_id);

    if(itr != file_id_to_metadata.end())
    {
        if(itr->second.last_modified_time != file.last_modified_time)
        {
            is_modified = true;
        }
    }

    return is_modified;
}

bool FileMetadataUtils::IsNew(const FileMetadata& file)
{
    std::cout << "Info: In FileMetadataUtils::IsNew()" << std::endl;
    bool is_new{false};
    auto itr = file_id_to_metadata.find(file.unique_id);

    if(itr == file_id_to_metadata.end())
    {
        is_new = true;
    }

    return is_new;
}

std::string FileMetadataUtils::GenerateFileUniqueId(const std::string &filePath) 
{
    std::cout << "Info: In FileMetadataUtils::GenerateFileUniqueId()" << std::endl;

    size_t hashValue = std::hash<std::string>{}(filePath);

    // convert size_t to string
    return std::to_string(hashValue);
}

void FileMetadataUtils::UpdateLocalFilePath(std::string file_id, std::string local_filepath)
{
    std::cout << "Info: In FileMetadataUtils::UpdateLocalFilePath()" << std::endl;

    file_id_to_metadata[file_id].local_path = local_filepath;
}

FileMetadata FileMetadataUtils::GetFileMetadataOfLocalFile(std::string local_filename)
{
    std::cout << "Info: In FileMetadataUtils::GetFileMetadataOfLocalFile()" << std::endl;

    // Find last backslash
    size_t pos = local_filename.find_last_of("\\/");

    std::string file_unique_id = local_filename.substr(pos + 1);

    return file_id_to_metadata[file_unique_id];
}

std::string FileMetadataUtils::GetLastModifiedDateTime(const fs::path& p) 
{
    std::cout << "Info: In FileMetadataUtils::GetLastModifiedDateTime()" << std::endl;
    auto ftime = fs::last_write_time(p); 
    auto sctp = std::chrono::system_clock::now()
              + (ftime - fs::file_time_type::clock::now());
    std::time_t tt = std::chrono::system_clock::to_time_t(sctp);

    std::tm tm;
#if defined(_WIN32)
    localtime_s(&tm, &tt);
#else
    localtime_r(&tt, &tm);
#endif

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%dT%H:%M:%S");
    return oss.str();
}

std::vector<FileMetadata> FileMetadataUtils::GetListOfFilesToDownload(std::vector<FileMetadata> available_files)
{
    std::cout << "Info: In FileMetadataUtils::GetListOfFilesToDownload()" << std::endl;
    std::vector<FileMetadata> files_to_download{};
    for(const auto itr : available_files)
    {
        if(IsNew(itr))
        {
            file_id_to_metadata[itr.unique_id] = FileMetadata(itr.unique_id,itr.source_path,"", itr.name, itr.last_modified_time, itr.size, "", "");
            files_to_download.push_back(itr);
        }
        else if(IsModified(itr))
        {
            file_id_to_metadata[itr.unique_id].last_modified_time = itr.last_modified_time;
            file_id_to_metadata[itr.unique_id].size = itr.size;
            files_to_download.push_back(itr);
        }
    }
    
    return files_to_download;
}

std::string getCurrentTimeString() 
{
    std::cout << "Info: In FileMetadataUtils::getCurrentTimeString()" << std::endl;
    // Get current time as system_clock
    auto now = std::chrono::system_clock::now();

    // Convert to time_t for formatting
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    // Convert to tm structure (local time)
    std::tm local_tm;
#ifdef _WIN32
    localtime_s(&local_tm, &now_time);  // Windows safe version
#else
    localtime_r(&now_time, &local_tm);  // POSIX safe version
#endif

    // Format into string
    std::ostringstream oss;
    oss << std::put_time(&local_tm, "%Y-%m-%dT%H:%M:%S");
    return oss.str();
}

void FileMetadataUtils::UpdateFileMetadataForUpload(std::string file_id, fs::path dest_file_path)
{
    std::cout << "Info: In FileMetadataUtils::UpdateFileMetadataForUpload()" << std::endl;

    file_id_to_metadata[file_id].last_backup_time = getCurrentTimeString();
    file_id_to_metadata[file_id].destination_path = dest_file_path.string();
}
