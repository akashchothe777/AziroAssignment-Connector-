#ifndef ONE_DRIVE_ST_H
#define ONE_DRIVE_ST_H

#include ".\source_type.h"
#include <curl/curl.h>

class OneDriveST : public SourceType
{
    std::string access_token;
public:
    OneDriveST(std::string url, std::string acc_token) : access_token(acc_token), SourceType(url){}
    bool DownloadFile(FileMetadata file_name, std::string download_folder);
    bool GetFileDetailsRecursively(std::vector<FileMetadata>& files_metadata, std::string folder_id);
    std::vector<FileMetadata> GetFilesDetails();
};

#endif // ONE_DRIVE_ST_H