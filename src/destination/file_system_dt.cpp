#include "destination/file_system_dt.h"
#include <filesystem>
#include <iostream>
#include "metadata/file_metadata_utils.h"

namespace fs = std::filesystem;

bool FileSystemDT::UploadFile(FileMetadata file_metadata)
{
    std::cout << "Info: In FileSystemDT::UploadFile()" << std::endl;

    bool result = false;

    fs::path dest_path = fs::path(address) / file_metadata.name;

    fs::path folder_path = dest_path.parent_path();
    
    if (!fs::exists(folder_path)) 
    {
        if(!fs::create_directories(folder_path))
            std::cerr << "Error: Failed to create directory - " << folder_path << std::endl;
    }

    try
    {
        if(fs::copy_file(file_metadata.local_path, dest_path, fs::copy_options::overwrite_existing))
        {
            result = true;
            FileMetadataUtils::UpdateFileMetadataForUpload(file_metadata.unique_id, dest_path);

            std::cout << "Info: Copied file " << file_metadata.local_path << " to the destination "
                << dest_path << std::endl;
        }
        else
        {
            std::cerr << "Error: Copying file failed - " << file_metadata.local_path << std::endl;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    
    return result;
}