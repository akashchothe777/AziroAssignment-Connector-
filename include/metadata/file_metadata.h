#ifndef FILE_METADATA_H
#define FILE_METADATA_H

#include <vector>
#include "../file_details.h"

struct FileMetadata
{
    std::string path;
    std::string name;
    std::string last_modified_time;
    unsigned long long size;
    std::string last_backup_time;
    std::string destination_path;

    FileMetadata(){}

    FileMetadata(std::string _path,
    std::string _name,
    std::string _last_modified_time,
    unsigned long long _size,
    std::string _last_backup_time,
    std::string _destination_path) :
        path(_path), name(_name), last_modified_time(_last_modified_time), 
        size(_size), last_backup_time(_last_backup_time), destination_path(_destination_path){}
};

#endif // FILE_METADATA_H
