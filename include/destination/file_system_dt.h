#ifndef FILE_SYSTEM_DT_H
#define FILE_SYSTEM_DT_H

#include "./destination_type.h"

class FileSystemDT : public DestinationType
{
public:
    FileSystemDT(std::string folder_path) : DestinationType(folder_path){}
    bool UploadFile(fs::path file_path);
    ~FileSystemDT(){};
};

#endif // FILE_SYSTEM_DT_H