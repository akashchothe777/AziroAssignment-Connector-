#include "../../include/source/source_adapter.h"
#include <iostream>

bool SourceAdapter::DownloadFiles(std::vector<FileDetails> files, std::string data_folder)
{
    for(const auto& file: files)
    {
        if(source_type->DownloadFile(file.name, data_folder))
        {
            return false;
        }
    }

    return true;
}

std::vector<FileDetails> SourceAdapter::GetFileList()
{
    std::cout << "In SourceAdapter::GetFileList()" << std::endl;
    std::vector<FileDetails> files = source_type->GetFilesDetails();

    return files;
}