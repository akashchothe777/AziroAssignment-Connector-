#ifndef SOURCE_TYPE_H
#define SOURCE_TYPE_H

#include <string>
#include <vector>
#include "..\file_details.h"

class SourceType
{
    public:
    virtual bool DownloadFile(std::string file_name, std::string download_folder) = 0;
    virtual std::vector<FileDetails> GetFilesDetails() = 0;
};

#endif // SOURCE_TYPE_H