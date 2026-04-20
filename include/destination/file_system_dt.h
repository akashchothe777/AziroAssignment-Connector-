#ifndef FILE_SYSTEM_DT_H
#define FILE_SYSTEM_DT_H

#include "./destination_type.h"

class FileSystemDT : public DestinationType
{
public:
    FileSystemDT(std::string folder_path, unsigned int rc) : DestinationType(folder_path,rc){}
    bool UploadFile(FileMetadata file_path);
    ~FileSystemDT(){};
};

#endif // FILE_SYSTEM_DT_H