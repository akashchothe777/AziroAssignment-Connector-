#include "source/source_adapter.h"
#include <iostream>
#include "utils/utils.h"

bool SourceAdapter::DownloadFiles(std::vector<FileDetails> files, std::string download_folder)
{
    std::cout << "In SourceAdapter::DownloadFiles()" << std::endl;
    bool all_success{true};

    for(const auto& file: files)
    {
        bool success = ConnectorUtils::RetryFunction([&](){
            return source_type->DownloadFile(file.name, download_folder);
        }, retry_count);

        if(!success)
        {
            std::cout << "Failed to download file: " << file.file_path << std::endl;
            all_success = false;
        }
    }

    return all_success;
}

std::vector<FileDetails> SourceAdapter::GetFileList()
{
    std::cout << "In SourceAdapter::GetFileList()" << std::endl;
    std::vector<FileDetails> files = source_type->GetFilesDetails();

    return files;
}