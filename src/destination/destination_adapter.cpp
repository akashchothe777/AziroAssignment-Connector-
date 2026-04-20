#include "destination/destination_adapter.h"
#include <filesystem>
#include <iostream>
#include "utils/utils.h"
#include "metadata/file_metadata_utils.h"

namespace fs = std::filesystem;

bool DestinationAdapter::UploadFiles(std::string download_folder, JsonHandler& json_handler)
{
    std::cout << "Info: In DestinationAdapter::UploadFiles()" << std::endl;

    bool all_success{true};

    for (const auto& itr : fs::directory_iterator(download_folder)) 
    {
        // Skip processing temporary file
        if(ConnectorUtils::has_ckpt_extension(itr))
            continue;

        FileMetadata file_metadata = FileMetadataUtils::GetFileMetadataOfLocalFile(itr.path().filename().string());

        bool success = destination_type->UploadFile(file_metadata);

        if(success)
        {
            FileMetadataUtils::UpdateFileMetadataForUpload(file_metadata.unique_id, file_metadata.name);
            json_handler.SaveToFile(FileMetadataUtils::file_id_to_metadata);
            fs::remove(itr.path());
        }
        else
        {
            std::cerr << "Error: Failed to upload file: " << itr.path().filename().string() << std::endl;
            all_success = false;
        }
    }

    return all_success;
}