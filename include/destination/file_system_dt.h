#ifndef FILE_SYSTEM_DT_H
#define FILE_SYSTEM_DT_H

#include "./destination_type.h"

class FileSystemDT : public DestinationType
{
    std::string address;
public:
    FileSystemDT(std::string folder_path) : address(folder_path){}
    bool UploadFile(fs::path file_path);
    FileSystemDT(/* args */){};
    ~FileSystemDT(){};
};

#endif // FILE_SYSTEM_DT_H