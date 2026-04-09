#include "metadata/file_metadata_utils.h"
#include <iostream>
#include <algorithm>

std::map<std::string, FileMetadata> FileMetadataUtils::filepath_to_metadata{};

bool FileMetadataUtils::IsModified(const FileDetails& file)
{
    bool is_modified{false};
    // for(const FileMetadata& file_itr : files_metadata)
    // {
    //     if(file_itr.id == file.id)
    //     {
    //         if(file_itr.last_modified_time != file.last_modified_time)
    //         {
    //             is_modified = true;
    //         }
    //         break;
    //     }
    // }

    return is_modified;
}

bool FileMetadataUtils::IsNew(const FileDetails& file)
{
    bool is_new{true};
    // for(const FileMetadata& file_itr : files_metadata)
    // {
    //     if(file_itr.id == file.id)
    //     {
    //         is_new = false;
    //         break;
    //     }
    // }

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
            // auto element_to_update = std::find_if(files_metadata.begin(), files_metadata.end(), [](auto obj){
            //     return (obj.id == itr.id);
            // });

            // element_to_update->last_modified_time = itr.last_modified_time;
            files_to_download.push_back(itr);

        }
    }
    
    return files_to_download;
}