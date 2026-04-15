#include "destination/file_system_dt.h"
#include <filesystem>
#include <iostream>
#include "metadata/file_metadata_utils.h"

namespace fs = std::filesystem;


bool FileSystemDT::UploadFile(fs::path file_path)
{
    std::cout << "Info: In FileSystemDT::UploadFile()" << std::endl;

    bool result = false;

    FileMetadata file_metadata = FileMetadataUtils::GetFileMetadataOfLocalFile(file_path.filename().string());

    fs::path dest_path = fs::path(address) / file_metadata.name;

    fs::path folder_path = dest_path.parent_path();
    
    if (!fs::exists(folder_path)) 
    {
        fs::create_directories(folder_path);
    }

    try
    {
        if(fs::copy_file(file_path, dest_path, fs::copy_options::overwrite_existing))
        {
            result = true;
            FileMetadataUtils::UpdateFileMetadataForUpload(file_metadata.unique_id, dest_path);

            fs::remove(file_path);
            std::cout << "Info: Copied file " << file_path << " to the destination "
                << dest_path << std::endl;
        }
        else
        {
            std::cerr << "Error: Copying file failed" << std::endl;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    
    return result;
}