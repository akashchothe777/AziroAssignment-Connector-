#ifndef SOURCE_TYPE_H
#define SOURCE_TYPE_H

#include <string>
#include <vector>
#include "metadata/file_metadata.h"

class SourceType
{
    protected:
    std::string address;
    public:
    SourceType(std::string ads) : address(ads){}
    virtual bool DownloadFile(FileMetadata file_name, std::string download_folder) = 0;
    virtual std::vector<FileMetadata> GetFilesDetails() = 0;
    virtual ~SourceType(){};
};

#endif // SOURCE_TYPE_H