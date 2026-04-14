#ifndef SOURCE_ADAPTER_H
#define SOURCE_ADAPTER_H

#include "./source_type.h"
#include "../file_details.h"
#include <vector>
#include <memory>

class SourceAdapter
{
private:
    std::shared_ptr<SourceType> source_type;
    unsigned int retry_count;
public:
    SourceAdapter(std::shared_ptr<SourceType> st, unsigned int rc) : source_type(st), retry_count(rc){};
    std::vector<FileDetails> GetFileList();
    bool DownloadFiles(std::vector<FileDetails> files, std::string download_folder);
    ~SourceAdapter(){};
};

#endif // SOURCE_ADAPTER_H