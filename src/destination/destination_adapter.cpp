#include "destination/destination_adapter.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

bool DestinationAdapter::UploadFiles(std::string data_folder)
{
    bool returnValue{false};

    for (const auto& itr : fs::directory_iterator(data_folder)) 
    {
        if (fs::is_regular_file(itr.path())) 
        {
            destination_type->UploadFile(itr.path());
        }
    }

    return returnValue;
}