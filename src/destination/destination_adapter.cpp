#include "destination/destination_adapter.h"
#include <filesystem>
#include <iostream>
#include "utils/utils.h"

namespace fs = std::filesystem;

bool DestinationAdapter::UploadFiles(std::string download_folder)
{
    std::cout << "Info: In DestinationAdapter::UploadFiles()" << std::endl;

    bool all_success{true};

    for (const auto& itr : fs::directory_iterator(download_folder)) 
    {
        bool success = ConnectorUtils::RetryFunction([&](){
            return destination_type->UploadFile(itr.path());
        }, retry_count);

        if(!success)
        {
            std::cerr << "Error: Failed to upload file: " << itr.path().filename().string() << std::endl;
            all_success = false;
        }
    }

    return all_success;
}