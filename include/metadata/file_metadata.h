#ifndef FILE_METADATA_H
#define FILE_METADATA_H

#include <vector>
#include <string>

struct FileMetadata
{
    std::string unique_id;
    std::string source_path;
    std::string local_path;
    std::string name;
    std::string last_modified_time;
    unsigned long long size;
    std::string last_backup_time;
    std::string destination_path;
    bool is_download_complete;

    FileMetadata(){}

    FileMetadata(std::string _unique_id,
        std::string _source_path,
        std::string _local_path,
        std::string _name,
        std::string _last_modified_time,
        unsigned long long _size,
        std::string _last_backup_time,
        std::string _destination_path,
        bool _is_download_complete = false) :
            unique_id(_unique_id),
            source_path(_source_path), 
            local_path(_local_path), 
            name(_name), 
            last_modified_time(_last_modified_time),
            size(_size), 
            last_backup_time(_last_backup_time), 
            destination_path(_destination_path),
            is_download_complete(_is_download_complete){}
};

#endif // FILE_METADATA_H
