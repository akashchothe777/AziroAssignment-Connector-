#include "source/source_adapter.h"
#include <iostream>
#include "utils/utils.h"

bool SourceAdapter::DownloadFiles(std::vector<FileMetadata> files, std::string download_folder)
{
    std::cout << "In SourceAdapter::DownloadFiles()" << std::endl;
    bool all_success{true};

    for(const auto& file: files)
    {
        bool success = ConnectorUtils::RetryFunction([&](){
            return source_type->DownloadFile(file, download_folder);
        }, retry_count);

        if(!success)
        {
            std::cout << "Failed to download file: " << file.source_path << std::endl;
            all_success = false;
        }
    }

    return all_success;
}

std::vector<FileMetadata> SourceAdapter::GetFileList()
{
    std::cout << "In SourceAdapter::GetFileList()" << std::endl;
    std::vector<FileMetadata> files = source_type->GetFilesDetails();

    return files;
}