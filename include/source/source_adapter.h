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
public:
    std::vector<FileDetails> GetFileList();
    bool DownloadFiles(std::vector<FileDetails> files, std::string data_folder);
    SourceAdapter(std::shared_ptr<SourceType> st) : source_type(st){};
    ~SourceAdapter(){};
};

#endif // SOURCE_ADAPTER_H