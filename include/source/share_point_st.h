#ifndef SHARE_POINT_ST_H
#define SHARE_POINT_ST_H

#include ".\source_type.h"

class SharePointST : public SourceType
{
public:
    bool DownloadFile(std::string file_name, std::string data_folder);
    std::vector<FileDetails> GetFilesDetails();
};

#endif // SHARE_POINT_ST_H