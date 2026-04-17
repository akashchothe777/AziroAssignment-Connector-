#ifndef FILE_SYSTEM_ST_H
#define FILE_SYSTEM_ST_H

#include ".\source_type.h"

class FileSystemST : public SourceType
{
public:
    FileSystemST(std::string folder_path) : SourceType(address){}
    bool DownloadFile(FileMetadata file_name, std::string download_folder) override;
    std::vector<FileMetadata> GetFilesDetails() override;
    ~FileSystemST(){};
};

#endif // FILE_SYSTEM_ST_H