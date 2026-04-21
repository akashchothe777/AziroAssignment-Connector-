#include "source/source_adapter.h"
#include <iostream>
#include "utils/utils.h"
#include "metadata/file_metadata_utils.h"

bool SourceAdapter::DownloadFiles(std::vector<FileMetadata> files, std::string download_folder)
{
    std::cout << "Info: In SourceAdapter::DownloadFiles()" << std::endl;
    bool all_success{true};

    for(const auto& file: files)
    {
        std::string output_file{download_folder};

        output_file = output_file + "\\" + file.unique_id;

        bool success = ConnectorUtils::RetryFunction([&](){
            return source_type->DownloadFile(file, output_file);
        }, retry_count);

        if(!success)
        {
            std::cerr << "Error: Failed to download file: " << file.name << std::endl;
            all_success = false;
        }
        else
        {
            FileMetadataUtils::UpdateLocalDownloadInfo(file.unique_id, output_file);
            std::cout << "Info: File is downloaded successfully: " << file.name << std::endl;
        }
    }

    return all_success;
}

std::vector<FileMetadata> SourceAdapter::GetFileList()
{
    std::cout << "Info: In SourceAdapter::GetFileList()" << std::endl;
    std::vector<FileMetadata> files = source_type->GetFilesDetails();

    return files;
}