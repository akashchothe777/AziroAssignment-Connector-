#include "destination/destination_adapter.h"
#include <filesystem>
#include <iostream>
#include "utils/utils.h"

namespace fs = std::filesystem;

bool DestinationAdapter::UploadFiles(std::string download_folder)
{
    std::cout << "In DestinationAdapter::UploadFiles()" << std::endl;

    bool all_success{true};

    for (const auto& itr : fs::directory_iterator(download_folder)) 
    {
        if (fs::is_regular_file(itr.path())) 
        {
            bool success = ConnectorUtils::RetryFunction([&](){
                return destination_type->UploadFile(itr.path());
            }, retry_count);

            if(!success)
            {
                std::cout << "Failed to upload file: " << itr.path().filename().string() << std::endl;
                all_success = false;
            }
        }
    }

    return all_success;
}