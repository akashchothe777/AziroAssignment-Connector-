#ifndef SHARE_POINT_ST_H
#define SHARE_POINT_ST_H

#include ".\source_type.h"

class SharePointST : public SourceType
{
public:
    bool DownloadFile(FileMetadata file_name, std::string download_folder);
    std::vector<FileMetadata> GetFilesDetails();
};

#endif // SHARE_POINT_ST_H