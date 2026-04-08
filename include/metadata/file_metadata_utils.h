#ifndef FILE_METADATA_UTILS_H
#define FILE_METADATA_UTILS_H

#include "file_metadata.h"
#include "../file_details.h"
#include <vector>

class FileMetadataUtils
{
    static std::vector<FileMetadata> files_metadata;
public:
    static bool IsModified(const FileDetails& file);
    static bool IsNew(const FileDetails& file);
    static std::vector<FileDetails> GetListOfFilesToDownload(std::vector<FileDetails> available_files);
};

#endif // FILE_METADATA_UTILS_H