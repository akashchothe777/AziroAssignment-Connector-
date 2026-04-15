#ifndef SOURCE_TYPE_H
#define SOURCE_TYPE_H

#include <string>
#include <vector>
#include "metadata/file_metadata.h"

class SourceType
{
    public:
    virtual bool DownloadFile(FileMetadata file_name, std::string download_folder) = 0;
    virtual std::vector<FileMetadata> GetFilesDetails() = 0;
};

#endif // SOURCE_TYPE_H