#include "metadata/file_metadata_utils.h"
#include <iostream>
#include <algorithm>

std::map<std::string, FileMetadata> FileMetadataUtils::filepath_to_metadata{};

bool FileMetadataUtils::IsModified(const FileDetails& file)
{
    bool is_modified{false};
    for(const auto& file_to_md_itr : filepath_to_metadata)
    {
        if(file_to_md_itr.first == file.file_path)
        {
            if(file_to_md_itr.second.last_modified_time != file.last_modified_time)
            {
                is_modified = true;
            }
            break;
        }
    }

    return is_modified;
}

bool FileMetadataUtils::IsNew(const FileDetails& file)
{
    bool is_new{true};
    for(const auto& file_to_md_itr : filepath_to_metadata)
    {
        if(file_to_md_itr.first == file.file_path)
        {
            is_new = false;
            break;
        }
    }

    return is_new;
}


std::string FileMetadataUtils::GetLastModifiedDateTime(const fs::path& p) {
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

std::vector<FileDetails> FileMetadataUtils::GetListOfFilesToDownload(std::vector<FileDetails> available_files)
{
    std::cout << "In FileMetadataUtils::GetListOfFilesToDownload()" << std::endl;
    std::vector<FileDetails> files_to_download{};
    for(const auto itr : available_files)
    {
        if(IsNew(itr))
        {
            //files_metadata.emplace_back(itr.file_path, itr.name, itr.last_modified_time, itr.size, "", "");
            filepath_to_metadata[itr.file_path] = FileMetadata(itr.file_path, itr.name, itr.last_modified_time, itr.size, "", "");
            files_to_download.push_back(itr);
        }
        else if(IsModified(itr))
        {
            files_to_download.push_back(itr);
        }
    }
    
    return files_to_download;
}

std::string getCurrentTimeString() {
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

void FileMetadataUtils::UpdateFileMetadata(fs::path file_path, fs::path dest_file_path)
{
    auto itr = std::find_if(filepath_to_metadata.begin(), filepath_to_metadata.end(), [=](const auto& item){
        return (item.second.name == file_path.filename().string());
    });
    if(itr != filepath_to_metadata.end())
    {
        itr->second.last_backup_time = getCurrentTimeString();
        itr->second.destination_path = dest_file_path.string();
    }
}
